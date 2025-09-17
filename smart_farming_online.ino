/*
 * Smart Farming System - Online Version
 * ESP32 Smart Farming Demo with IoT Connectivity
 * 
 * This version extends the offline version with internet connectivity
 * Features: All offline features plus WiFi connectivity, cloud data transmission,
 * remote monitoring, web interface, and OTA updates
 * 
 * Hardware Requirements:
 * - ESP32 Development Board
 * - Soil Moisture Sensor (Analog)
 * - DHT11/DHT22 Temperature/Humidity Sensor
 * - Relay Module
 * - LCD 1602 with I2C Backpack
 * - LEDs for status indication
 * - WiFi connection
 * 
 * Author: Smart Farming Demo
 * Version: 1.0.0
 * Date: 2024
 */

#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <AdafruitIO_WiFi.h>
#include <esp_task_wdt.h>
#include <Arduino.h>

// Conditional library inclusions
#if DISPLAY_ENABLED
  #include <LiquidCrystal_I2C.h>
#endif

#if DHT_ENABLED
  #include <DHT.h>
#endif

// =============================================================================
// GLOBAL VARIABLES AND OBJECTS
// =============================================================================

// LCD Object (conditional)
#if DISPLAY_ENABLED
  LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
#endif

// DHT Sensor Object (conditional)
#if DHT_ENABLED
  DHT dht(DHT_PIN, DHT_TYPE);
#endif

// Web Server Object
WebServer server(WEB_SERVER_PORT);

// Adafruit IO Object
AdafruitIO_WiFi io(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY, WIFI_SSID, WIFI_PASSWORD);

// System State Variables
struct SystemState {
  float temperature = 0.0;
  float humidity = 0.0;
  int soilMoistureRaw = 0;
  int soilMoisturePercent = 0;
  bool pumpActive = false;
  bool systemOK = true;
  bool wifiConnected = false;
  bool emergencyStop = false;
  unsigned long lastIrrigation = 0;
  unsigned long pumpStartTime = 0;
  int dailyIrrigations = 0;
  unsigned long lastSensorRead = 0;
  unsigned long lastDisplayUpdate = 0;
  unsigned long lastDataTransmission = 0;
  unsigned long lastWiFiCheck = 0;
  int displayScreen = 0;
  int sensorErrors = 0;
  int transmissionErrors = 0;
  int adafruitIOErrors = 0;
  int recoveryAttempts = 0;
  unsigned long lastErrorCheck = 0;
  unsigned long lastWatchdogFeed = 0;
  String lastTransmissionStatus = "Not attempted";
  String lastAdafruitIOStatus = "Not attempted";
  
  // Control System State
  #if CONTROL_TYPE == CONTROL_ROTARY_ENCODER
    int currentMenu = 0;
    int currentParameter = 0;
    int encoderPosition = 0;
    bool encoderButtonPressed = false;
    unsigned long lastMenuActivity = 0;
    bool inMenuMode = false;
  #elif CONTROL_TYPE == CONTROL_POTENTIOMETER
    int potentiometerValue = 0;
    int adjustedThreshold = SOIL_MOISTURE_THRESHOLD;
    unsigned long lastPotentiometerRead = 0;
  #endif
} systemState;

// Sensor Validation Variables
struct SensorValidation {
  float lastTemperature = 0.0;
  float lastHumidity = 0.0;
  int lastSoilMoisture = 0;
  int temperatureReadings[SENSOR_CONSISTENCY_CHECKS] = {0};
  int humidityReadings[SENSOR_CONSISTENCY_CHECKS] = {0};
  int soilMoistureReadings[SENSOR_CONSISTENCY_CHECKS] = {0};
  int readingIndex = 0;
  bool temperatureValid = true;
  bool humidityValid = true;
  bool soilMoistureValid = true;
  int disconnectCount = 0;
} sensorValidation;

// WiFi and Network Variables
unsigned long wifiReconnectAttempts = 0;
const int maxWifiReconnectAttempts = 10;
bool otaEnabled = false;

// Adafruit IO Feed Objects
AdafruitIO_Feed *temperatureFeed;
AdafruitIO_Feed *humidityFeed;
AdafruitIO_Feed *soilMoistureFeed;
AdafruitIO_Feed *pumpStatusFeed;
AdafruitIO_Feed *irrigationCountFeed;

// Data Logging
struct DataLog {
  unsigned long timestamp;
  float temperature;
  float humidity;
  int soilMoisturePercent;
  bool pumpActive;
  int dailyIrrigations;
} dataLog[LOG_BUFFER_SIZE];

int logIndex = 0;
bool logBufferFull = false;

// Timing Variables
unsigned long currentTime = 0;
unsigned long lastHeartbeat = 0;

// =============================================================================
// FUNCTION DECLARATIONS
// =============================================================================

// Core System Functions
void initializeSystem();
void initializeSensors();
void initializeDisplay();
void initializeActuators();
void initializeWiFi();
void initializeAdafruitIO();
void initializeOTA();
void initializeWebServer();
void initializeWatchdog();

// Sensor and Control Functions
void readSensors();
void controlIrrigation();
void startIrrigation();
void stopIrrigation();

// Display Functions
void updateDisplay();
void displaySensorData();
void displaySystemStatus();
void displayIrrigationInfo();
void displayWiFiStatus();
void displayAllInfo();  // For LCD 2004
void outputToSerial();  // For no display mode

// Control Functions
void initializeControl();
void handleControl();
void handleRotaryEncoder();
void handlePotentiometer();
void displayMenu();
void displayParameterAdjustment();
void adjustParameter(int direction);
void saveSettings();
void loadSettings();

// Network Functions
void checkWiFiConnection();
void transmitDataToCloud();
void transmitDataToAdafruitIO();
void handleWebRequests();
void emergencyStop();
void feedWatchdog();
void validateSensorReadings();
bool isSensorReadingValid(float value, float minVal, float maxVal, bool enabled);
bool checkSensorConsistency(int readings[], int newReading);
void attemptSystemRecovery();
void checkPumpRuntime();
void handleRoot();
void handleAPI();
void handleControl();
void handleStatus();
void handleOTA();

// LED and Status Functions
void updateLEDs();
void checkSystemStatus();
void handleErrors();
void performHeartbeat();

// Data Management
void logSystemData();
void clearDataLog();
String getSystemStatusJSON();

// =============================================================================
// SETUP FUNCTION
// =============================================================================

void setup() {
  // Initialize Serial Communication
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  
  Serial.println("========================================");
  Serial.println("ESP32 Smart Farming System - Online");
  Serial.println("Version: " + String(FIRMWARE_VERSION));
  Serial.println("Build Date: " + String(BUILD_DATE) + " " + String(BUILD_TIME));
  Serial.println("========================================");
  
  // Initialize System Components
  initializeSystem();
  
  Serial.println("System initialization complete!");
  Serial.println("Starting main loop...");
  Serial.println("========================================");
}

// =============================================================================
// MAIN LOOP FUNCTION
// =============================================================================

void loop() {
  currentTime = millis();
  
  // Feed watchdog timer
  feedWatchdog();
  
  // Check for emergency stop
  if (EMERGENCY_STOP_ENABLED && systemState.emergencyStop) {
    emergencyStop();
    return;
  }
  
  // Check pump runtime protection
  if (PUMP_RUNTIME_PROTECTION && systemState.pumpActive) {
    checkPumpRuntime();
  }
  
  // Handle web server requests
  server.handleClient();
  
  // Handle OTA updates
  if (otaEnabled) {
    ArduinoOTA.handle();
  }
  
  // Read sensors at regular intervals
  if (currentTime - systemState.lastSensorRead >= SENSOR_READ_INTERVAL) {
    readSensors();
    systemState.lastSensorRead = currentTime;
  }
  
  // Update display at regular intervals
  if (currentTime - systemState.lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    updateDisplay();
    systemState.lastDisplayUpdate = currentTime;
  }
  
  // Handle control input
  if (CONTROL_ENABLED) {
    handleControl();
  }
  
  // Control irrigation based on sensor readings
  controlIrrigation();
  
  // Update LED status indicators
  updateLEDs();
  
  // Check WiFi connection
  if (currentTime - systemState.lastWiFiCheck >= WIFI_RECONNECT_INTERVAL) {
    checkWiFiConnection();
    systemState.lastWiFiCheck = currentTime;
  }
  
  // Transmit data to cloud services
  if (systemState.wifiConnected && 
      currentTime - systemState.lastDataTransmission >= DATA_TRANSMISSION_INTERVAL) {
    
    // Transmit to ThingSpeak
    if (THINGSPEAK_ENABLED) {
      transmitDataToCloud();
    }
    
    // Transmit to Adafruit IO
    if (ADAFRUIT_IO_ENABLED) {
      transmitDataToAdafruitIO();
    }
    
    systemState.lastDataTransmission = currentTime;
  }
  
  // Check system status and handle errors
  if (currentTime - systemState.lastErrorCheck >= STATUS_CHECK_INTERVAL) {
    checkSystemStatus();
    systemState.lastErrorCheck = currentTime;
  }
  
  // Attempt system recovery if needed
  if (AUTO_RECOVERY_ENABLED && !systemState.systemOK && systemState.recoveryAttempts < RECOVERY_ATTEMPTS) {
    attemptSystemRecovery();
  }
  
  // Perform system heartbeat
  if (currentTime - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    performHeartbeat();
    lastHeartbeat = currentTime;
  }
  
  // Log system data
  logSystemData();
  
  // Small delay to prevent overwhelming the system
  delay(100);
}

// =============================================================================
// INITIALIZATION FUNCTIONS
// =============================================================================

void initializeSystem() {
  Serial.println("Initializing system components...");
  
  // Initialize sensors
  initializeSensors();
  
  // Initialize display
  initializeDisplay();
  
  // Initialize actuators
  initializeActuators();
  
  // Initialize control system
  initializeControl();
  
  // Initialize WiFi
  initializeWiFi();
  
  // Initialize Adafruit IO
  initializeAdafruitIO();
  
  // Initialize OTA
  initializeOTA();
  
  // Initialize web server
  initializeWebServer();
  
  // Initialize Watchdog Timer
  initializeWatchdog();
  
  // Initialize timing variables
  systemState.lastSensorRead = 0;
  systemState.lastDisplayUpdate = 0;
  systemState.lastDataTransmission = 0;
  systemState.lastWiFiCheck = 0;
  systemState.lastErrorCheck = 0;
  lastHeartbeat = 0;
  
  // Clear data log
  clearDataLog();
  
  Serial.println("System components initialized successfully!");
}

void initializeSensors() {
  Serial.println("Initializing sensors...");
  
  // Initialize DHT sensor (if enabled)
  #if DHT_ENABLED
    dht.begin();
    delay(2000); // Allow sensor to stabilize
    
    // Test DHT sensor
    float testTemp = dht.readTemperature();
    float testHumidity = dht.readHumidity();
    
    if (isnan(testTemp) || isnan(testHumidity)) {
      Serial.println("Warning: DHT sensor initialization failed!");
      systemState.systemOK = false;
    } else {
      Serial.println("DHT sensor initialized successfully (" + String(DHT_SENSOR_TYPE == DHT11 ? "DHT11" : "DHT22") + ")");
      Serial.println("Test reading - Temperature: " + String(testTemp) + "°C, Humidity: " + String(testHumidity) + "%");
    }
  #else
    Serial.println("DHT sensor disabled - using default values");
  #endif
  
  // Initialize soil moisture sensor (analog)
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  Serial.println("Soil moisture sensor initialized");
  
  Serial.println("Sensor initialization complete!");
}

void initializeDisplay() {
  #if DISPLAY_ENABLED
    Serial.println("Initializing LCD display...");
    
    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    
    // Display startup message
    lcd.setCursor(0, 0);
    lcd.print("Smart Farming");
    #if DISPLAY_TYPE == DISPLAY_LCD_2004
      lcd.setCursor(0, 1);
      lcd.print("Online Mode");
      lcd.setCursor(0, 2);
      lcd.print("Initializing...");
    #else
      lcd.setCursor(0, 1);
      lcd.print("Online Mode");
    #endif
    
    delay(2000);
    
    // Clear display
    lcd.clear();
    
    Serial.println("LCD display initialized successfully!");
  #else
    Serial.println("No display configured - using serial output only");
  #endif
}

void initializeActuators() {
  Serial.println("Initializing actuators...");
  
  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Ensure pump is off initially
  
  // Initialize LED pins
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  
  // Turn off all LEDs initially
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_BLUE_PIN, LOW);
  
  Serial.println("Actuators initialized successfully!");
}

void initializeWiFi() {
  Serial.println("Initializing WiFi connection...");
  
  // Set WiFi mode
  WiFi.mode(WIFI_STA);
  
  // Begin WiFi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  // Wait for connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    systemState.wifiConnected = true;
    Serial.println();
    Serial.println("WiFi connected successfully!");
    Serial.println("IP address: " + WiFi.localIP().toString());
    Serial.println("Signal strength: " + String(WiFi.RSSI()) + " dBm");
  } else {
    systemState.wifiConnected = false;
    Serial.println();
    Serial.println("WiFi connection failed!");
    Serial.println("System will operate in offline mode");
  }
}

void initializeAdafruitIO() {
  if (!ADAFRUIT_IO_ENABLED) {
    Serial.println("Adafruit IO disabled in configuration");
    return;
  }
  
  Serial.println("Initializing Adafruit IO connection...");
  
  // Initialize Adafruit IO feeds
  temperatureFeed = io.feed(ADAFRUIT_IO_TEMPERATURE_FEED);
  humidityFeed = io.feed(ADAFRUIT_IO_HUMIDITY_FEED);
  soilMoistureFeed = io.feed(ADAFRUIT_IO_SOIL_MOISTURE_FEED);
  pumpStatusFeed = io.feed(ADAFRUIT_IO_PUMP_STATUS_FEED);
  irrigationCountFeed = io.feed(ADAFRUIT_IO_IRRIGATION_COUNT_FEED);
  
  // Connect to Adafruit IO
  Serial.println("Connecting to Adafruit IO...");
  io.connect();
  
  // Wait for connection
  int attempts = 0;
  while (io.status() < AIO_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (io.status() == AIO_CONNECTED) {
    Serial.println();
    Serial.println("Adafruit IO connected successfully!");
  } else {
    Serial.println();
    Serial.println("Adafruit IO connection failed!");
    Serial.println("Status: " + String(io.statusText()));
  }
}

void initializeOTA() {
  Serial.println("Initializing OTA updates...");
  
  // Configure OTA
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);
  
  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Start updating " + type);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("OTA Update");
    lcd.setCursor(0, 1);
    lcd.print("In Progress");
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("OTA Update");
    lcd.setCursor(0, 1);
    lcd.print("Complete");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  
  ArduinoOTA.begin();
  otaEnabled = true;
  
  Serial.println("OTA updates initialized successfully!");
}

void initializeWebServer() {
  Serial.println("Initializing web server...");
  
  // Configure web server routes
  server.on("/", handleRoot);
  server.on("/api", handleAPI);
  server.on("/control", HTTP_POST, handleControl);
  server.on("/status", handleStatus);
  server.on("/ota", handleOTA);
  
  // Start web server
  server.begin();
  
  Serial.println("Web server initialized successfully!");
  Serial.println("Web interface available at: http://" + WiFi.localIP().toString());
}

// =============================================================================
// SENSOR READING FUNCTIONS
// =============================================================================

void readSensors() {
  // Read DHT sensor (if enabled)
  float temperature = 0.0;
  float humidity = 0.0;
  
  #if DHT_ENABLED
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    // Check for DHT sensor errors
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Error: Failed to read DHT sensor!");
      systemState.sensorErrors++;
      sensorValidation.disconnectCount++;
      return;
    }
  #else
    // No DHT sensor - use default values
    temperature = 25.0;  // Default temperature
    humidity = 50.0;     // Default humidity
  #endif
  
  // Read soil moisture sensor
  int soilMoistureRaw = analogRead(SOIL_MOISTURE_PIN);
  
  // Convert raw reading to percentage
  int soilMoisturePercent = map(soilMoistureRaw, SOIL_MOISTURE_WET_VALUE, SOIL_MOISTURE_DRY_VALUE, 100, 0);
  soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);
  
  // Validate sensor readings
  validateSensorReadings(temperature, humidity, soilMoisturePercent);
  
  // Only update system state if readings are valid
  if (sensorValidation.temperatureValid && sensorValidation.humidityValid && sensorValidation.soilMoistureValid) {
    systemState.temperature = temperature;
    systemState.humidity = humidity;
    systemState.soilMoistureRaw = soilMoistureRaw;
    systemState.soilMoisturePercent = soilMoisturePercent;
    
    // Reset sensor error counter on successful reading
    systemState.sensorErrors = 0;
    sensorValidation.disconnectCount = 0;
  } else {
    systemState.sensorErrors++;
    Serial.println("Warning: Invalid sensor readings detected!");
  }
  
  // Debug output
  if (DEBUG_MODE) {
    Serial.println("Sensor Readings:");
    Serial.println("  Temperature: " + String(temperature, 1) + "°C (Valid: " + String(sensorValidation.temperatureValid ? "Yes" : "No") + ")");
    Serial.println("  Humidity: " + String(humidity, 1) + "% (Valid: " + String(sensorValidation.humidityValid ? "Yes" : "No") + ")");
    Serial.println("  Soil Moisture: " + String(soilMoisturePercent) + "% (Valid: " + String(sensorValidation.soilMoistureValid ? "Yes" : "No") + ")");
  }
}

// =============================================================================
// DISPLAY FUNCTIONS
// =============================================================================

void updateDisplay() {
  #if DISPLAY_ENABLED
    #if DISPLAY_TYPE == DISPLAY_LCD_2004
      // LCD 2004: Show all information on single screen
      displayAllInfo();
    #else
      // LCD 1602: Cycle through different screens
      switch (systemState.displayScreen) {
        case 0:
          displaySensorData();
          break;
        case 1:
          displaySystemStatus();
          break;
        case 2:
          displayIrrigationInfo();
          break;
        case 3:
          displayWiFiStatus();
          break;
        default:
          systemState.displayScreen = 0;
          break;
      }
      
      // Advance to next screen
      systemState.displayScreen = (systemState.displayScreen + 1) % DISPLAY_SCREEN_COUNT;
    #endif
  #else
    // No display: Output to serial
    outputToSerial();
  #endif
}

void displaySensorData() {
  #if DISPLAY_ENABLED
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: " + String(systemState.temperature, 1) + "C");
    lcd.setCursor(0, 1);
    lcd.print("Hum: " + String(systemState.humidity, 1) + "%");
    
    delay(DISPLAY_SCROLL_DELAY);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Soil Moisture:");
    lcd.setCursor(0, 1);
    lcd.print(String(systemState.soilMoisturePercent) + "%");
  #endif
}

void displaySystemStatus() {
  #if DISPLAY_ENABLED
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Status:");
    lcd.setCursor(0, 1);
    
    if (systemState.systemOK) {
      lcd.print("OK");
    } else {
      lcd.print("ERROR");
    }
    
    delay(DISPLAY_SCROLL_DELAY);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pump: ");
    lcd.print(systemState.pumpActive ? "ON" : "OFF");
    lcd.setCursor(0, 1);
    lcd.print("Daily: " + String(systemState.dailyIrrigations));
  #endif
}

void displayIrrigationInfo() {
  #if DISPLAY_ENABLED
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Irrigation Info:");
    lcd.setCursor(0, 1);
    
    if (systemState.lastIrrigation > 0) {
      unsigned long timeSinceLastIrrigation = (currentTime - systemState.lastIrrigation) / 1000;
      lcd.print("Last: " + String(timeSinceLastIrrigation) + "s");
    } else {
      lcd.print("No irrigation yet");
    }
  #endif
}

void displayWiFiStatus() {
  #if DISPLAY_ENABLED
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Status:");
    lcd.setCursor(0, 1);
    
    if (systemState.wifiConnected) {
      lcd.print("Connected");
    } else {
      lcd.print("Disconnected");
    }
    
    delay(DISPLAY_SCROLL_DELAY);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("IP: " + WiFi.localIP().toString());
  #endif
}

// =============================================================================
// IRRIGATION CONTROL FUNCTIONS
// =============================================================================

void controlIrrigation() {
  // Determine threshold to use
  int threshold = SOIL_MOISTURE_THRESHOLD;
  #if CONTROL_TYPE == CONTROL_POTENTIOMETER || CONTROL_TYPE == CONTROL_ROTARY_ENCODER
    threshold = systemState.adjustedThreshold;
  #endif
  
  // Check if irrigation is needed
  bool needsIrrigation = (systemState.soilMoisturePercent < threshold);
  
  // Check cooldown period
  bool cooldownExpired = (currentTime - systemState.lastIrrigation >= IRRIGATION_COOLDOWN);
  
  // Check daily irrigation limit
  bool withinDailyLimit = (systemState.dailyIrrigations < MAX_DAILY_IRRIGATIONS);
  
  // Check if system is OK
  bool systemHealthy = systemState.systemOK && (systemState.sensorErrors < MAX_SENSOR_ERRORS);
  
  // Start irrigation if all conditions are met
  if (needsIrrigation && cooldownExpired && withinDailyLimit && systemHealthy && !systemState.pumpActive) {
    startIrrigation();
  }
  
  // Stop irrigation if duration exceeded
  if (systemState.pumpActive && (currentTime - systemState.lastIrrigation >= IRRIGATION_DURATION)) {
    stopIrrigation();
  }
}

void startIrrigation() {
  Serial.println("Starting irrigation...");
  
  // Activate relay (pump)
  digitalWrite(RELAY_PIN, HIGH);
  systemState.pumpActive = true;
  systemState.pumpStartTime = currentTime; // Record pump start time for runtime protection
  
  // Update irrigation tracking
  systemState.lastIrrigation = currentTime;
  systemState.dailyIrrigations++;
  
  // Update display
  #if DISPLAY_ENABLED
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("IRRIGATION");
    lcd.setCursor(0, 1);
    lcd.print("ACTIVE");
  #endif
  
  Serial.println("Irrigation started. Duration: " + String(IRRIGATION_DURATION / 1000) + " seconds");
  Serial.println("Maximum runtime: " + String(MAX_PUMP_RUNTIME / 1000) + " seconds");
}

void stopIrrigation() {
  Serial.println("Stopping irrigation...");
  
  // Deactivate relay (pump)
  digitalWrite(RELAY_PIN, LOW);
  systemState.pumpActive = false;
  
  // Clear display
  #if DISPLAY_ENABLED
    lcd.clear();
  #endif
  
  Serial.println("Irrigation stopped.");
}

// =============================================================================
// NETWORK FUNCTIONS
// =============================================================================

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    if (systemState.wifiConnected) {
      Serial.println("WiFi connection lost! Attempting to reconnect...");
      systemState.wifiConnected = false;
    }
    
    // Attempt to reconnect
    if (wifiReconnectAttempts < maxWifiReconnectAttempts) {
      WiFi.reconnect();
      wifiReconnectAttempts++;
      
      // Wait a bit for connection
      delay(2000);
      
      if (WiFi.status() == WL_CONNECTED) {
        systemState.wifiConnected = true;
        wifiReconnectAttempts = 0;
        Serial.println("WiFi reconnected successfully!");
        Serial.println("IP address: " + WiFi.localIP().toString());
      }
    }
  } else {
    if (!systemState.wifiConnected) {
      systemState.wifiConnected = true;
      wifiReconnectAttempts = 0;
      Serial.println("WiFi connection restored!");
    }
  }
}

void transmitDataToCloud() {
  if (!systemState.wifiConnected) {
    systemState.lastTransmissionStatus = "WiFi not connected";
    return;
  }
  
  Serial.println("Transmitting data to cloud...");
  
  HTTPClient http;
  String url = "https://api.thingspeak.com/update";
  
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String data = "api_key=" + String(THINGSPEAK_API_KEY) +
                "&field1=" + String(systemState.temperature) +
                "&field2=" + String(systemState.humidity) +
                "&field3=" + String(systemState.soilMoisturePercent) +
                "&field4=" + String(systemState.pumpActive ? 1 : 0) +
                "&field5=" + String(systemState.dailyIrrigations);
  
  int httpResponseCode = http.POST(data);
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Data transmitted successfully!");
    Serial.println("Response code: " + String(httpResponseCode));
    Serial.println("Response: " + response);
    systemState.lastTransmissionStatus = "Success";
    systemState.transmissionErrors = 0;
  } else {
    Serial.println("Error transmitting data!");
    Serial.println("Response code: " + String(httpResponseCode));
    systemState.lastTransmissionStatus = "Failed: " + String(httpResponseCode);
    systemState.transmissionErrors++;
  }
  
  http.end();
}

void transmitDataToAdafruitIO() {
  if (!ADAFRUIT_IO_ENABLED) {
    systemState.lastAdafruitIOStatus = "Disabled";
    return;
  }
  
  if (!systemState.wifiConnected || io.status() != AIO_CONNECTED) {
    systemState.lastAdafruitIOStatus = "Not connected";
    return;
  }
  
  Serial.println("Transmitting data to Adafruit IO...");
  
  try {
    // Send temperature data
    temperatureFeed->save(systemState.temperature);
    
    // Send humidity data
    humidityFeed->save(systemState.humidity);
    
    // Send soil moisture data
    soilMoistureFeed->save(systemState.soilMoisturePercent);
    
    // Send pump status (1 for active, 0 for inactive)
    pumpStatusFeed->save(systemState.pumpActive ? 1 : 0);
    
    // Send irrigation count
    irrigationCountFeed->save(systemState.dailyIrrigations);
    
    Serial.println("Data transmitted to Adafruit IO successfully!");
    systemState.lastAdafruitIOStatus = "Success";
    systemState.adafruitIOErrors = 0;
    
  } catch (const std::exception& e) {
    Serial.println("Error transmitting data to Adafruit IO!");
    Serial.println("Error: " + String(e.what()));
    systemState.lastAdafruitIOStatus = "Failed: " + String(e.what());
    systemState.adafruitIOErrors++;
  }
}

// =============================================================================
// WEB SERVER HANDLERS
// =============================================================================

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>Smart Farming System</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:Arial;margin:20px;}";
  html += ".container{max-width:800px;margin:0 auto;}";
  html += ".card{background:#f4f4f4;padding:20px;margin:10px 0;border-radius:5px;}";
  html += ".status{display:inline-block;padding:5px 10px;border-radius:3px;color:white;}";
  html += ".ok{background:green;} .error{background:red;} .warning{background:orange;}";
  html += "button{padding:10px 20px;margin:5px;border:none;border-radius:3px;cursor:pointer;}";
  html += ".btn-primary{background:#007bff;color:white;}";
  html += ".btn-danger{background:#dc3545;color:white;}";
  html += "</style></head><body>";
  
  html += "<div class='container'>";
  html += "<h1>Smart Farming System</h1>";
  
  // System Status
  html += "<div class='card'>";
  html += "<h2>System Status</h2>";
  html += "<p>Temperature: " + String(systemState.temperature, 1) + "°C</p>";
  html += "<p>Humidity: " + String(systemState.humidity, 1) + "%</p>";
  html += "<p>Soil Moisture: " + String(systemState.soilMoisturePercent) + "%</p>";
  html += "<p>Pump Status: <span class='status " + String(systemState.pumpActive ? "warning" : "ok") + "'>" + String(systemState.pumpActive ? "ACTIVE" : "INACTIVE") + "</span></p>";
  html += "<p>Daily Irrigations: " + String(systemState.dailyIrrigations) + "</p>";
  html += "<p>System Status: <span class='status " + String(systemState.systemOK ? "ok" : "error") + "'>" + String(systemState.systemOK ? "OK" : "ERROR") + "</span></p>";
  html += "<p>WiFi Status: <span class='status " + String(systemState.wifiConnected ? "ok" : "error") + "'>" + String(systemState.wifiConnected ? "CONNECTED" : "DISCONNECTED") + "</span></p>";
  html += "</div>";
  
  // Cloud Services Status
  html += "<div class='card'>";
  html += "<h2>Cloud Services Status</h2>";
  html += "<p>ThingSpeak: <span class='status " + String(THINGSPEAK_ENABLED ? (systemState.lastTransmissionStatus == "Success" ? "ok" : "warning") : "warning") + "'>" + String(THINGSPEAK_ENABLED ? systemState.lastTransmissionStatus : "DISABLED") + "</span></p>";
  html += "<p>Adafruit IO: <span class='status " + String(ADAFRUIT_IO_ENABLED ? (systemState.lastAdafruitIOStatus == "Success" ? "ok" : "warning") : "warning") + "'>" + String(ADAFRUIT_IO_ENABLED ? systemState.lastAdafruitIOStatus : "DISABLED") + "</span></p>";
  html += "</div>";
  
  // Control Panel
  html += "<div class='card'>";
  html += "<h2>Control Panel</h2>";
  html += "<button class='btn-primary' onclick='startIrrigation()'>Start Irrigation</button>";
  html += "<button class='btn-danger' onclick='stopIrrigation()'>Stop Irrigation</button>";
  html += "<button class='btn-primary' onclick='refreshData()'>Refresh Data</button>";
  html += "</div>";
  
  // Network Info
  html += "<div class='card'>";
  html += "<h2>Network Information</h2>";
  html += "<p>IP Address: " + WiFi.localIP().toString() + "</p>";
  html += "<p>Signal Strength: " + String(WiFi.RSSI()) + " dBm</p>";
  html += "<p>Last Transmission: " + systemState.lastTransmissionStatus + "</p>";
  html += "</div>";
  
  html += "</div>";
  
  // JavaScript
  html += "<script>";
  html += "function startIrrigation(){fetch('/control?action=start',{method:'POST'}).then(()=>refreshData());}";
  html += "function stopIrrigation(){fetch('/control?action=stop',{method:'POST'}).then(()=>refreshData());}";
  html += "function refreshData(){location.reload();}";
  html += "setInterval(refreshData, 30000);"; // Auto-refresh every 30 seconds
  html += "</script>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleAPI() {
  String json = getSystemStatusJSON();
  server.send(200, "application/json", json);
}

void handleControl() {
  if (server.hasArg("action")) {
    String action = server.arg("action");
    
    if (action == "start") {
      startIrrigation();
      server.send(200, "text/plain", "Irrigation started");
    } else if (action == "stop") {
      stopIrrigation();
      server.send(200, "text/plain", "Irrigation stopped");
    } else {
      server.send(400, "text/plain", "Invalid action");
    }
  } else {
    server.send(400, "text/plain", "Missing action parameter");
  }
}

void handleStatus() {
  String json = getSystemStatusJSON();
  server.send(200, "application/json", json);
}

void handleOTA() {
  String html = "<!DOCTYPE html><html><head><title>OTA Update</title></head><body>";
  html += "<h1>OTA Update</h1>";
  html += "<p>OTA updates are enabled and ready.</p>";
  html += "<p>Use Arduino IDE or ESP32 OTA tools to upload new firmware.</p>";
  html += "<p>Hostname: " + String(OTA_HOSTNAME) + "</p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// =============================================================================
// LED CONTROL FUNCTIONS
// =============================================================================

void updateLEDs() {
  // Green LED: System OK
  digitalWrite(LED_GREEN_PIN, systemState.systemOK && systemState.sensorErrors < MAX_SENSOR_ERRORS);
  
  // Red LED: Pump Active
  digitalWrite(LED_RED_PIN, systemState.pumpActive);
  
  // Blue LED: WiFi Connected
  digitalWrite(LED_BLUE_PIN, systemState.wifiConnected);
}

// =============================================================================
// SYSTEM MONITORING FUNCTIONS
// =============================================================================

void checkSystemStatus() {
  // Check sensor errors
  if (systemState.sensorErrors >= MAX_SENSOR_ERRORS) {
    systemState.systemOK = false;
    Serial.println("Warning: Too many sensor errors detected!");
  } else if (systemState.sensorErrors == 0) {
    systemState.systemOK = true;
  }
  
  // Check memory usage
  if (currentTime % MEMORY_CHECK_INTERVAL < STATUS_CHECK_INTERVAL) {
    Serial.println("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
  }
}

void handleErrors() {
  if (!systemState.systemOK) {
    Serial.println("System error detected! Check sensors and connections.");
    
    // Turn off pump for safety
    if (systemState.pumpActive) {
      stopIrrigation();
    }
    
    // Display error on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SYSTEM ERROR");
    lcd.setCursor(0, 1);
    lcd.print("Check sensors");
  }
}

void performHeartbeat() {
  Serial.println("System heartbeat - All systems operational");
  Serial.println("  Temperature: " + String(systemState.temperature, 1) + "°C");
  Serial.println("  Humidity: " + String(systemState.humidity, 1) + "%");
  Serial.println("  Soil Moisture: " + String(systemState.soilMoisturePercent) + "%");
  Serial.println("  Pump Status: " + String(systemState.pumpActive ? "ON" : "OFF"));
  Serial.println("  Daily Irrigations: " + String(systemState.dailyIrrigations));
  Serial.println("  System Status: " + String(systemState.systemOK ? "OK" : "ERROR"));
  Serial.println("  WiFi Status: " + String(systemState.wifiConnected ? "CONNECTED" : "DISCONNECTED"));
  Serial.println("  ThingSpeak Status: " + systemState.lastTransmissionStatus);
  Serial.println("  Adafruit IO Status: " + systemState.lastAdafruitIOStatus);
}

// =============================================================================
// DATA MANAGEMENT FUNCTIONS
// =============================================================================

void logSystemData() {
  static unsigned long lastLogTime = 0;
  
  if (currentTime - lastLogTime >= LOG_INTERVAL) {
    // Store data in log buffer
    dataLog[logIndex].timestamp = currentTime;
    dataLog[logIndex].temperature = systemState.temperature;
    dataLog[logIndex].humidity = systemState.humidity;
    dataLog[logIndex].soilMoisturePercent = systemState.soilMoisturePercent;
    dataLog[logIndex].pumpActive = systemState.pumpActive;
    dataLog[logIndex].dailyIrrigations = systemState.dailyIrrigations;
    
    logIndex = (logIndex + 1) % LOG_BUFFER_SIZE;
    if (logIndex == 0) {
      logBufferFull = true;
    }
    
    // Serial output
    Serial.println("=== SYSTEM DATA LOG ===");
    Serial.println("Timestamp: " + String(currentTime));
    Serial.println("Temperature: " + String(systemState.temperature, 2) + "°C");
    Serial.println("Humidity: " + String(systemState.humidity, 2) + "%");
    Serial.println("Soil Moisture: " + String(systemState.soilMoisturePercent) + "%");
    Serial.println("Soil Moisture Raw: " + String(systemState.soilMoistureRaw));
    Serial.println("Pump Active: " + String(systemState.pumpActive ? "Yes" : "No"));
    Serial.println("Daily Irrigations: " + String(systemState.dailyIrrigations));
    Serial.println("System OK: " + String(systemState.systemOK ? "Yes" : "No"));
    Serial.println("WiFi Connected: " + String(systemState.wifiConnected ? "Yes" : "No"));
    Serial.println("Sensor Errors: " + String(systemState.sensorErrors));
    Serial.println("ThingSpeak Errors: " + String(systemState.transmissionErrors));
    Serial.println("Adafruit IO Errors: " + String(systemState.adafruitIOErrors));
    Serial.println("Free Heap: " + String(ESP.getFreeHeap()) + " bytes");
    Serial.println("========================");
    
    lastLogTime = currentTime;
  }
}

void clearDataLog() {
  for (int i = 0; i < LOG_BUFFER_SIZE; i++) {
    dataLog[i].timestamp = 0;
    dataLog[i].temperature = 0.0;
    dataLog[i].humidity = 0.0;
    dataLog[i].soilMoisturePercent = 0;
    dataLog[i].pumpActive = false;
    dataLog[i].dailyIrrigations = 0;
  }
  logIndex = 0;
  logBufferFull = false;
}

String getSystemStatusJSON() {
  DynamicJsonDocument doc(1024);
  
  doc["timestamp"] = currentTime;
  doc["temperature"] = systemState.temperature;
  doc["humidity"] = systemState.humidity;
  doc["soilMoisture"] = systemState.soilMoisturePercent;
  doc["soilMoistureRaw"] = systemState.soilMoistureRaw;
  doc["pumpActive"] = systemState.pumpActive;
  doc["dailyIrrigations"] = systemState.dailyIrrigations;
  doc["systemOK"] = systemState.systemOK;
  doc["wifiConnected"] = systemState.wifiConnected;
  doc["sensorErrors"] = systemState.sensorErrors;
  doc["transmissionErrors"] = systemState.transmissionErrors;
  doc["adafruitIOErrors"] = systemState.adafruitIOErrors;
  doc["lastTransmissionStatus"] = systemState.lastTransmissionStatus;
  doc["lastAdafruitIOStatus"] = systemState.lastAdafruitIOStatus;
  doc["thingSpeakEnabled"] = THINGSPEAK_ENABLED;
  doc["adafruitIOEnabled"] = ADAFRUIT_IO_ENABLED;
  doc["freeHeap"] = ESP.getFreeHeap();
  doc["uptime"] = millis();
  
  String json;
  serializeJson(doc, json);
  return json;
}

// =============================================================================
// SAFETY AND FAIL-SAFE FUNCTIONS
// =============================================================================

void initializeWatchdog() {
  if (WATCHDOG_ENABLED) {
    esp_task_wdt_init(WATCHDOG_TIMEOUT, true);
    esp_task_wdt_add(NULL);
    Serial.println("Watchdog timer initialized (" + String(WATCHDOG_TIMEOUT) + " seconds)");
  }
}

void feedWatchdog() {
  if (WATCHDOG_ENABLED) {
    esp_task_wdt_reset();
    systemState.lastWatchdogFeed = currentTime;
  }
}

void emergencyStop() {
  Serial.println("EMERGENCY STOP ACTIVATED!");
  
  // Stop irrigation immediately
  if (systemState.pumpActive) {
    stopIrrigation();
  }
  
  // Set emergency stop flag
  systemState.emergencyStop = true;
  systemState.systemOK = false;
  
  // Display emergency message
  #if DISPLAY_ENABLED
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("EMERGENCY STOP");
    lcd.setCursor(0, 1);
    lcd.print("SYSTEM HALTED");
  #endif
  
  // Turn off all LEDs except red (emergency indicator)
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_BLUE_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);
  
  Serial.println("System halted due to emergency stop!");
}

void validateSensorReadings(float temperature, float humidity, int soilMoisture) {
  // Validate temperature
  sensorValidation.temperatureValid = isSensorReadingValid(temperature, MIN_TEMPERATURE, MAX_TEMPERATURE, TEMPERATURE_VALIDATION);
  
  // Validate humidity
  sensorValidation.humidityValid = isSensorReadingValid(humidity, MIN_HUMIDITY, MAX_HUMIDITY, HUMIDITY_VALIDATION);
  
  // Validate soil moisture
  sensorValidation.soilMoistureValid = isSensorReadingValid(soilMoisture, MIN_SOIL_MOISTURE, MAX_SOIL_MOISTURE, SOIL_MOISTURE_VALIDATION);
  
  // Check for sudden changes in soil moisture
  if (SOIL_MOISTURE_VALIDATION && sensorValidation.soilMoistureValid) {
    int change = abs(soilMoisture - sensorValidation.lastSoilMoisture);
    if (change > MAX_SOIL_MOISTURE_CHANGE) {
      Serial.println("Warning: Sudden soil moisture change detected: " + String(change) + "%");
      sensorValidation.soilMoistureValid = false;
    }
  }
  
  // Check sensor consistency
  if (CONSISTENCY_VALIDATION) {
    sensorValidation.temperatureValid &= checkSensorConsistency(sensorValidation.temperatureReadings, (int)(temperature * 10));
    sensorValidation.humidityValid &= checkSensorConsistency(sensorValidation.humidityReadings, (int)(humidity * 10));
    sensorValidation.soilMoistureValid &= checkSensorConsistency(sensorValidation.soilMoistureReadings, soilMoisture);
  }
  
  // Update last readings
  sensorValidation.lastTemperature = temperature;
  sensorValidation.lastHumidity = humidity;
  sensorValidation.lastSoilMoisture = soilMoisture;
  
  // Update reading arrays for consistency checking
  sensorValidation.temperatureReadings[sensorValidation.readingIndex] = (int)(temperature * 10);
  sensorValidation.humidityReadings[sensorValidation.readingIndex] = (int)(humidity * 10);
  sensorValidation.soilMoistureReadings[sensorValidation.readingIndex] = soilMoisture;
  sensorValidation.readingIndex = (sensorValidation.readingIndex + 1) % SENSOR_CONSISTENCY_CHECKS;
}

bool isSensorReadingValid(float value, float minVal, float maxVal, bool enabled) {
  if (!enabled) return true;
  return (value >= minVal && value <= maxVal);
}

bool checkSensorConsistency(int readings[], int newReading) {
  // Calculate average of previous readings
  int sum = 0;
  for (int i = 0; i < SENSOR_CONSISTENCY_CHECKS; i++) {
    sum += readings[i];
  }
  int average = sum / SENSOR_CONSISTENCY_CHECKS;
  
  // Check if new reading is within threshold
  int deviation = abs(newReading - average);
  return (deviation <= SENSOR_CONSISTENCY_THRESHOLD);
}

void attemptSystemRecovery() {
  Serial.println("Attempting system recovery...");
  
  systemState.recoveryAttempts++;
  
  // Reset sensor validation
  sensorValidation.temperatureValid = true;
  sensorValidation.humidityValid = true;
  sensorValidation.soilMoistureValid = true;
  sensorValidation.disconnectCount = 0;
  
  // Reset error counters
  systemState.sensorErrors = 0;
  
  // Reinitialize sensors
  dht.begin();
  delay(2000);
  
  // Test sensor readings
  float testTemp = dht.readTemperature();
  float testHumidity = dht.readHumidity();
  
  if (!isnan(testTemp) && !isnan(testHumidity)) {
    systemState.systemOK = true;
    systemState.recoveryAttempts = 0;
    Serial.println("System recovery successful!");
  } else {
    Serial.println("System recovery failed. Attempt " + String(systemState.recoveryAttempts) + "/" + String(RECOVERY_ATTEMPTS));
    delay(RECOVERY_DELAY);
  }
}

void checkPumpRuntime() {
  if (systemState.pumpActive && (currentTime - systemState.pumpStartTime >= MAX_PUMP_RUNTIME)) {
    Serial.println("Warning: Maximum pump runtime exceeded! Stopping irrigation for safety.");
    stopIrrigation();
    
    // Display warning on LCD
    #if DISPLAY_ENABLED
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PUMP TIMEOUT");
      lcd.setCursor(0, 1);
      lcd.print("SAFETY STOP");
    #endif
  }
}

// =============================================================================
// MODULAR DISPLAY FUNCTIONS
// =============================================================================

void displayAllInfo() {
  #if DISPLAY_TYPE == DISPLAY_LCD_2004
    lcd.clear();
    
    // Line 1: Temperature and Humidity
    lcd.setCursor(0, 0);
    lcd.print("T:" + String(systemState.temperature, 1) + "C H:" + String(systemState.humidity, 1) + "%");
    
    // Line 2: Soil Moisture and Pump Status
    lcd.setCursor(0, 1);
    lcd.print("Soil:" + String(systemState.soilMoisturePercent) + "% Pump:" + String(systemState.pumpActive ? "ON" : "OFF"));
    
    // Line 3: System Status and WiFi
    lcd.setCursor(0, 2);
    lcd.print("Status:" + String(systemState.systemOK ? "OK" : "ERROR") + " WiFi:" + String(systemState.wifiConnected ? "ON" : "OFF"));
    
    // Line 4: Daily Irrigations and Cloud Status
    lcd.setCursor(0, 3);
    String cloudStatus = "TS:" + String(THINGSPEAK_ENABLED ? "ON" : "OFF") + " AIO:" + String(ADAFRUIT_IO_ENABLED ? "ON" : "OFF");
    lcd.print("Daily:" + String(systemState.dailyIrrigations) + " " + cloudStatus);
  #endif
}

void outputToSerial() {
  #if SERIAL_OUTPUT_ENABLED
    static unsigned long lastSerialOutput = 0;
    
    if (currentTime - lastSerialOutput >= DISPLAY_UPDATE_INTERVAL) {
      Serial.println("=== SMART FARMING STATUS (ONLINE) ===");
      Serial.println("Temperature: " + String(systemState.temperature, 1) + "°C");
      Serial.println("Humidity: " + String(systemState.humidity, 1) + "%");
      Serial.println("Soil Moisture: " + String(systemState.soilMoisturePercent) + "%");
      Serial.println("Pump Status: " + String(systemState.pumpActive ? "ACTIVE" : "INACTIVE"));
      Serial.println("System Status: " + String(systemState.systemOK ? "OK" : "ERROR"));
      Serial.println("WiFi Status: " + String(systemState.wifiConnected ? "CONNECTED" : "DISCONNECTED"));
      Serial.println("Daily Irrigations: " + String(systemState.dailyIrrigations));
      Serial.println("ThingSpeak Status: " + systemState.lastTransmissionStatus);
      Serial.println("Adafruit IO Status: " + systemState.lastAdafruitIOStatus);
      Serial.println("Sensor Errors: " + String(systemState.sensorErrors));
      Serial.println("Uptime: " + String(currentTime / 1000) + " seconds");
      Serial.println("=====================================");
      
      lastSerialOutput = currentTime;
    }
  #endif
}

// =============================================================================
// CONTROL SYSTEM FUNCTIONS
// =============================================================================

void initializeControl() {
  #if CONTROL_TYPE == CONTROL_ROTARY_ENCODER
    Serial.println("Initializing rotary encoder control...");
    
    // Set encoder pins as inputs with pullup
    pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
    pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
    pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
    
    // Initialize encoder state
    systemState.currentMenu = 0;
    systemState.currentParameter = 0;
    systemState.encoderPosition = 0;
    systemState.encoderButtonPressed = false;
    systemState.lastMenuActivity = currentTime;
    systemState.inMenuMode = false;
    
    Serial.println("Rotary encoder control initialized");
    
  #elif CONTROL_TYPE == CONTROL_POTENTIOMETER
    Serial.println("Initializing potentiometer control...");
    
    // Initialize potentiometer state
    systemState.potentiometerValue = 0;
    systemState.adjustedThreshold = SOIL_MOISTURE_THRESHOLD;
    systemState.lastPotentiometerRead = currentTime;
    
    Serial.println("Potentiometer control initialized");
    
  #else
    Serial.println("No manual control configured - fully automated mode");
  #endif
  
  // Load saved settings
  loadSettings();
}

void handleControl() {
  #if CONTROL_TYPE == CONTROL_ROTARY_ENCODER
    handleRotaryEncoder();
  #elif CONTROL_TYPE == CONTROL_POTENTIOMETER
    handlePotentiometer();
  #endif
}

void handleRotaryEncoder() {
  #if CONTROL_TYPE == CONTROL_ROTARY_ENCODER
    // Read encoder button
    bool buttonState = !digitalRead(ENCODER_SW_PIN); // Inverted due to pullup
    
    // Detect button press
    if (buttonState && !systemState.encoderButtonPressed) {
      systemState.encoderButtonPressed = true;
      systemState.lastMenuActivity = currentTime;
      
      if (!systemState.inMenuMode) {
        // Enter menu mode
        systemState.inMenuMode = true;
        systemState.currentMenu = 0;
        Serial.println("Entered menu mode");
      } else {
        // Process menu selection
        if (systemState.currentMenu < MENU_ITEMS) {
          // Enter parameter adjustment mode
          systemState.currentParameter = systemState.currentMenu;
          Serial.println("Entered parameter adjustment mode: " + String(systemState.currentParameter));
        } else {
          // Exit menu mode
          systemState.inMenuMode = false;
          saveSettings();
          Serial.println("Exited menu mode");
        }
      }
    } else if (!buttonState && systemState.encoderButtonPressed) {
      systemState.encoderButtonPressed = false;
    }
    
    // Read encoder rotation
    static int lastCLK = HIGH;
    int currentCLK = digitalRead(ENCODER_CLK_PIN);
    
    if (currentCLK != lastCLK) {
      if (digitalRead(ENCODER_DT_PIN) != currentCLK) {
        // Clockwise rotation
        systemState.encoderPosition++;
        systemState.lastMenuActivity = currentTime;
        
        if (systemState.inMenuMode) {
          if (systemState.currentParameter >= 0) {
            // Adjusting parameter
            adjustParameter(1);
          } else {
            // Navigating menu
            systemState.currentMenu = (systemState.currentMenu + 1) % (MENU_ITEMS + 1);
          }
        }
      } else {
        // Counter-clockwise rotation
        systemState.encoderPosition--;
        systemState.lastMenuActivity = currentTime;
        
        if (systemState.inMenuMode) {
          if (systemState.currentParameter >= 0) {
            // Adjusting parameter
            adjustParameter(-1);
          } else {
            // Navigating menu
            systemState.currentMenu = (systemState.currentMenu - 1 + MENU_ITEMS + 1) % (MENU_ITEMS + 1);
          }
        }
      }
    }
    lastCLK = currentCLK;
    
    // Check for menu timeout
    if (systemState.inMenuMode && (currentTime - systemState.lastMenuActivity > MENU_TIMEOUT)) {
      systemState.inMenuMode = false;
      systemState.currentParameter = -1;
      saveSettings();
      Serial.println("Menu timeout - returned to normal mode");
    }
    
    // Update display for menu
    if (systemState.inMenuMode) {
      if (systemState.currentParameter >= 0) {
        displayParameterAdjustment();
      } else {
        displayMenu();
      }
    }
  #endif
}

void handlePotentiometer() {
  #if CONTROL_TYPE == CONTROL_POTENTIOMETER
    if (currentTime - systemState.lastPotentiometerRead >= POTENTIOMETER_UPDATE_INTERVAL) {
      // Read potentiometer value (0-4095)
      systemState.potentiometerValue = analogRead(POTENTIOMETER_PIN);
      
      // Convert to threshold percentage
      systemState.adjustedThreshold = map(systemState.potentiometerValue, 0, 4095, 
                                        POTENTIOMETER_MIN_THRESHOLD, POTENTIOMETER_MAX_THRESHOLD);
      
      systemState.lastPotentiometerRead = currentTime;
      
      // Update display to show current threshold
      #if DISPLAY_ENABLED
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Threshold: " + String(systemState.adjustedThreshold) + "%");
        lcd.setCursor(0, 1);
        lcd.print("Pot: " + String(systemState.potentiometerValue));
      #endif
      
      Serial.println("Potentiometer threshold: " + String(systemState.adjustedThreshold) + "%");
    }
  #endif
}

void displayMenu() {
  #if DISPLAY_ENABLED && CONTROL_TYPE == CONTROL_ROTARY_ENCODER
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Menu:");
    
    String menuItems[] = {
      "Soil Threshold",
      "Irrigation Time", 
      "Display Speed",
      "WiFi Settings",
      "Save & Exit"
    };
    
    if (systemState.currentMenu < MENU_ITEMS) {
      lcd.setCursor(0, 1);
      lcd.print(menuItems[systemState.currentMenu]);
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Exit Menu");
    }
  #endif
}

void displayParameterAdjustment() {
  #if DISPLAY_ENABLED && CONTROL_TYPE == CONTROL_ROTARY_ENCODER
    lcd.clear();
    lcd.setCursor(0, 0);
    
    switch (systemState.currentParameter) {
      case 0: // Soil Threshold
        lcd.print("Soil Threshold:");
        lcd.setCursor(0, 1);
        lcd.print(String(systemState.adjustedThreshold) + "%");
        break;
        
      case 1: // Irrigation Time
        lcd.print("Irrigation Time:");
        lcd.setCursor(0, 1);
        lcd.print(String(IRRIGATION_DURATION / 1000) + "s");
        break;
        
      case 2: // Display Speed
        lcd.print("Display Speed:");
        lcd.setCursor(0, 1);
        lcd.print(String(DISPLAY_SCROLL_DELAY / 1000) + "s");
        break;
        
      case 3: // WiFi Settings
        lcd.print("WiFi Status:");
        lcd.setCursor(0, 1);
        lcd.print(systemState.wifiConnected ? "Connected" : "Disconnected");
        break;
    }
  #endif
}

void adjustParameter(int direction) {
  #if CONTROL_TYPE == CONTROL_ROTARY_ENCODER
    switch (systemState.currentParameter) {
      case 0: // Soil Threshold
        systemState.adjustedThreshold += direction * ENCODER_STEP_SIZE;
        systemState.adjustedThreshold = constrain(systemState.adjustedThreshold, 
                                                   POTENTIOMETER_MIN_THRESHOLD, 
                                                   POTENTIOMETER_MAX_THRESHOLD);
        break;
        
      case 1: // Irrigation Time (read-only for now)
        // Could be made adjustable in future versions
        break;
        
      case 2: // Display Speed (read-only for now)
        // Could be made adjustable in future versions
        break;
        
      case 3: // WiFi Settings (read-only)
        break;
    }
  #endif
}

void saveSettings() {
  #if CONTROL_TYPE == CONTROL_ROTARY_ENCODER || CONTROL_TYPE == CONTROL_POTENTIOMETER
    // In a real implementation, this would save to EEPROM
    // For now, we'll just log the settings
    Serial.println("Settings saved:");
    Serial.println("  Soil Threshold: " + String(systemState.adjustedThreshold) + "%");
  #endif
}

void loadSettings() {
  #if CONTROL_TYPE == CONTROL_ROTARY_ENCODER || CONTROL_TYPE == CONTROL_POTENTIOMETER
    // In a real implementation, this would load from EEPROM
    // For now, we'll use default values
    systemState.adjustedThreshold = SOIL_MOISTURE_THRESHOLD;
    Serial.println("Settings loaded with defaults");
  #endif
}
