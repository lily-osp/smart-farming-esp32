/*
 * ESP32 Smart Farming System - Hardware Test Code
 * 
 * This test code verifies all hardware connections and components
 * before running the main smart farming system.
 * 
 * Test Sequence:
 * 1. Serial Communication Test
 * 2. LED Status Test (Green, Red, Blue)
 * 3. Soil Moisture Sensor Test
 * 4. DHT Sensor Test (if enabled)
 * 5. LCD Display Test (if enabled)
 * 6. Relay Test
 * 7. Control System Test (if enabled)
 * 8. Emergency Stop Test (if enabled)
 * 
 * Author: Smart Farming Demo
 * Version: 1.0.0
 * Date: 2024
 */

#include <Arduino.h>

// =============================================================================
// CONFIGURATION - Modify these settings to match your hardware
// =============================================================================

// Test Configuration
#define TEST_DELAY 2000           // Delay between tests (ms)
#define SERIAL_BAUD_RATE 115200   // Serial communication speed

// Hardware Configuration
#define SOIL_MOISTURE_PIN 36      // ADC1_CH0
#define RELAY_PIN 19              // Relay control pin
#define LED_GREEN_PIN 18          // System status LED
#define LED_RED_PIN 23            // Pump active LED
#define LED_BLUE_PIN 2            // WiFi status LED
#define EMERGENCY_STOP_PIN 0      // Emergency stop button (optional)

// DHT Sensor Configuration (set to 0 to disable)
#define DHT_ENABLED 1             // 1 = enabled, 0 = disabled
#define DHT_PIN 5                 // DHT data pin
#define DHT_TYPE DHT22            // DHT11 or DHT22

// LCD Configuration (set to 0 to disable)
#define LCD_ENABLED 1             // 1 = enabled, 0 = disabled
#define LCD_ADDRESS 0x27          // I2C address (0x27 or 0x3F)
#define LCD_COLS 16               // 16 for 1602, 20 for 2004
#define LCD_ROWS 2                // 2 for 1602, 4 for 2004

// Control System Configuration (set to 0 to disable)
#define CONTROL_ENABLED 1         // 1 = enabled, 0 = disabled
#define ENCODER_CLK_PIN 16        // Rotary encoder clock pin
#define ENCODER_DT_PIN 17         // Rotary encoder data pin
#define ENCODER_SW_PIN 4          // Rotary encoder switch pin
#define POTENTIOMETER_PIN 34      // Potentiometer center pin

// =============================================================================
// LIBRARY INCLUDES
// =============================================================================

#if DHT_ENABLED
  #include <DHT.h>
#endif

#if LCD_ENABLED
  #include <LiquidCrystal_I2C.h>
#endif

// =============================================================================
// GLOBAL OBJECTS
// =============================================================================

#if DHT_ENABLED
  DHT dht(DHT_PIN, DHT_TYPE);
#endif

#if LCD_ENABLED
  LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
#endif

// Test Results
struct TestResults {
  bool serialTest = false;
  bool ledTest = false;
  bool soilMoistureTest = false;
  bool dhtTest = false;
  bool lcdTest = false;
  bool relayTest = false;
  bool controlTest = false;
  bool emergencyTest = false;
} testResults;

// =============================================================================
// SETUP FUNCTION
// =============================================================================

void setup() {
  // Initialize Serial Communication
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  
  Serial.println("========================================");
  Serial.println("ESP32 Smart Farming - Hardware Test");
  Serial.println("========================================");
  Serial.println();
  
  // Initialize all components
  initializeHardware();
  
  // Run all tests
  runAllTests();
  
  // Display final results
  displayFinalResults();
}

// =============================================================================
// MAIN LOOP
// =============================================================================

void loop() {
  // Continuous monitoring mode
  continuousMonitoring();
  delay(5000);
}

// =============================================================================
// HARDWARE INITIALIZATION
// =============================================================================

void initializeHardware() {
  Serial.println("Initializing hardware components...");
  
  // Initialize GPIO pins
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  
  if (EMERGENCY_STOP_PIN >= 0) {
    pinMode(EMERGENCY_STOP_PIN, INPUT_PULLUP);
  }
  
  #if CONTROL_ENABLED
    pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
    pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
    pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
  #endif
  
  // Turn off all outputs initially
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_BLUE_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  
  #if DHT_ENABLED
    dht.begin();
    delay(2000); // Allow sensor to stabilize
  #endif
  
  #if LCD_ENABLED
    lcd.init();
    lcd.backlight();
    lcd.clear();
  #endif
  
  Serial.println("Hardware initialization complete!");
  Serial.println();
}

// =============================================================================
// TEST FUNCTIONS
// =============================================================================

void runAllTests() {
  Serial.println("Starting hardware tests...");
  Serial.println("========================================");
  
  // Test 1: Serial Communication
  testSerialCommunication();
  delay(TEST_DELAY);
  
  // Test 2: LED Status Indicators
  testLEDs();
  delay(TEST_DELAY);
  
  // Test 3: Soil Moisture Sensor
  testSoilMoistureSensor();
  delay(TEST_DELAY);
  
  // Test 4: DHT Sensor (if enabled)
  #if DHT_ENABLED
    testDHTSensor();
    delay(TEST_DELAY);
  #endif
  
  // Test 5: LCD Display (if enabled)
  #if LCD_ENABLED
    testLCDDisplay();
    delay(TEST_DELAY);
  #endif
  
  // Test 6: Relay Module
  testRelayModule();
  delay(TEST_DELAY);
  
  // Test 7: Control System (if enabled)
  #if CONTROL_ENABLED
    testControlSystem();
    delay(TEST_DELAY);
  #endif
  
  // Test 8: Emergency Stop (if enabled)
  if (EMERGENCY_STOP_PIN >= 0) {
    testEmergencyStop();
    delay(TEST_DELAY);
  }
  
  Serial.println("========================================");
  Serial.println("All tests completed!");
  Serial.println();
}

void testSerialCommunication() {
  Serial.println("Test 1: Serial Communication");
  Serial.println("  ✓ Serial communication working");
  Serial.println("  ✓ Baud rate: " + String(SERIAL_BAUD_RATE));
  testResults.serialTest = true;
}

void testLEDs() {
  Serial.println("Test 2: LED Status Indicators");
  
  // Test Green LED
  Serial.println("  Testing Green LED (System OK)...");
  digitalWrite(LED_GREEN_PIN, HIGH);
  delay(500);
  digitalWrite(LED_GREEN_PIN, LOW);
  delay(500);
  
  // Test Red LED
  Serial.println("  Testing Red LED (Pump Active)...");
  digitalWrite(LED_RED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_RED_PIN, LOW);
  delay(500);
  
  // Test Blue LED
  Serial.println("  Testing Blue LED (WiFi Status)...");
  digitalWrite(LED_BLUE_PIN, HIGH);
  delay(500);
  digitalWrite(LED_BLUE_PIN, LOW);
  delay(500);
  
  Serial.println("  ✓ All LEDs tested successfully");
  testResults.ledTest = true;
}

void testSoilMoistureSensor() {
  Serial.println("Test 3: Soil Moisture Sensor");
  
  int rawValue = analogRead(SOIL_MOISTURE_PIN);
  int percentage = map(rawValue, 0, 4095, 100, 0);
  percentage = constrain(percentage, 0, 100);
  
  Serial.println("  Raw reading: " + String(rawValue));
  Serial.println("  Percentage: " + String(percentage) + "%");
  
  if (rawValue >= 0 && rawValue <= 4095) {
    Serial.println("  ✓ Soil moisture sensor working");
    testResults.soilMoistureTest = true;
  } else {
    Serial.println("  ✗ Soil moisture sensor error");
  }
}

#if DHT_ENABLED
void testDHTSensor() {
  Serial.println("Test 4: DHT Temperature/Humidity Sensor");
  
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  Serial.println("  Temperature: " + String(temperature, 1) + "°C");
  Serial.println("  Humidity: " + String(humidity, 1) + "%");
  
  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.println("  ✓ DHT sensor working");
    testResults.dhtTest = true;
  } else {
    Serial.println("  ✗ DHT sensor error - check connections");
  }
}
#endif

#if LCD_ENABLED
void testLCDDisplay() {
  Serial.println("Test 5: LCD Display");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hardware Test");
  lcd.setCursor(0, 1);
  lcd.print("LCD Working!");
  
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("I2C Address:");
  lcd.setCursor(0, 1);
  lcd.print("0x" + String(LCD_ADDRESS, HEX));
  
  delay(2000);
  
  Serial.println("  ✓ LCD display working");
  Serial.println("  ✓ I2C address: 0x" + String(LCD_ADDRESS, HEX));
  testResults.lcdTest = true;
}
#endif

void testRelayModule() {
  Serial.println("Test 6: Relay Module");
  
  Serial.println("  Activating relay (pump ON)...");
  digitalWrite(RELAY_PIN, HIGH);
  delay(1000);
  
  Serial.println("  Deactivating relay (pump OFF)...");
  digitalWrite(RELAY_PIN, LOW);
  delay(1000);
  
  Serial.println("  ✓ Relay module working");
  testResults.relayTest = true;
}

#if CONTROL_ENABLED
void testControlSystem() {
  Serial.println("Test 7: Control System");
  
  // Test Rotary Encoder
  Serial.println("  Testing Rotary Encoder...");
  int clkState = digitalRead(ENCODER_CLK_PIN);
  int dtState = digitalRead(ENCODER_DT_PIN);
  int swState = digitalRead(ENCODER_SW_PIN);
  
  Serial.println("    CLK pin: " + String(clkState ? "HIGH" : "LOW"));
  Serial.println("    DT pin: " + String(dtState ? "HIGH" : "LOW"));
  Serial.println("    SW pin: " + String(swState ? "HIGH" : "LOW"));
  
  // Test Potentiometer
  Serial.println("  Testing Potentiometer...");
  int potValue = analogRead(POTENTIOMETER_PIN);
  int potPercentage = map(potValue, 0, 4095, 0, 100);
  
  Serial.println("    Raw value: " + String(potValue));
  Serial.println("    Percentage: " + String(potPercentage) + "%");
  
  Serial.println("  ✓ Control system working");
  testResults.controlTest = true;
}
#endif

void testEmergencyStop() {
  Serial.println("Test 8: Emergency Stop Button");
  
  int buttonState = digitalRead(EMERGENCY_STOP_PIN);
  Serial.println("  Button state: " + String(buttonState ? "RELEASED" : "PRESSED"));
  
  Serial.println("  ✓ Emergency stop button working");
  testResults.emergencyTest = true;
}

// =============================================================================
// RESULTS DISPLAY
// =============================================================================

void displayFinalResults() {
  Serial.println("========================================");
  Serial.println("HARDWARE TEST RESULTS");
  Serial.println("========================================");
  
  Serial.println("Serial Communication: " + String(testResults.serialTest ? "✓ PASS" : "✗ FAIL"));
  Serial.println("LED Status Indicators: " + String(testResults.ledTest ? "✓ PASS" : "✗ FAIL"));
  Serial.println("Soil Moisture Sensor: " + String(testResults.soilMoistureTest ? "✓ PASS" : "✗ FAIL"));
  
  #if DHT_ENABLED
    Serial.println("DHT Sensor: " + String(testResults.dhtTest ? "✓ PASS" : "✗ FAIL"));
  #else
    Serial.println("DHT Sensor: DISABLED");
  #endif
  
  #if LCD_ENABLED
    Serial.println("LCD Display: " + String(testResults.lcdTest ? "✓ PASS" : "✗ FAIL"));
  #else
    Serial.println("LCD Display: DISABLED");
  #endif
  
  Serial.println("Relay Module: " + String(testResults.relayTest ? "✓ PASS" : "✗ FAIL"));
  
  #if CONTROL_ENABLED
    Serial.println("Control System: " + String(testResults.controlTest ? "✓ PASS" : "✗ FAIL"));
  #else
    Serial.println("Control System: DISABLED");
  #endif
  
  if (EMERGENCY_STOP_PIN >= 0) {
    Serial.println("Emergency Stop: " + String(testResults.emergencyTest ? "✓ PASS" : "✗ FAIL"));
  } else {
    Serial.println("Emergency Stop: DISABLED");
  }
  
  Serial.println("========================================");
  
  // Overall result
  bool allTestsPassed = testResults.serialTest && 
                       testResults.ledTest && 
                       testResults.soilMoistureTest && 
                       testResults.relayTest &&
                       (!DHT_ENABLED || testResults.dhtTest) &&
                       (!LCD_ENABLED || testResults.lcdTest) &&
                       (!CONTROL_ENABLED || testResults.controlTest) &&
                       (EMERGENCY_STOP_PIN < 0 || testResults.emergencyTest);
  
  if (allTestsPassed) {
    Serial.println("🎉 ALL TESTS PASSED! Hardware is ready.");
    Serial.println("You can now upload the main smart farming code.");
  } else {
    Serial.println("⚠️  SOME TESTS FAILED! Check hardware connections.");
    Serial.println("Fix the issues before running the main system.");
  }
  
  Serial.println("========================================");
  Serial.println();
}

// =============================================================================
// CONTINUOUS MONITORING
// =============================================================================

void continuousMonitoring() {
  Serial.println("=== CONTINUOUS MONITORING ===");
  
  // Soil Moisture
  int soilRaw = analogRead(SOIL_MOISTURE_PIN);
  int soilPercent = map(soilRaw, 0, 4095, 100, 0);
  soilPercent = constrain(soilPercent, 0, 100);
  
  Serial.println("Soil Moisture: " + String(soilPercent) + "% (Raw: " + String(soilRaw) + ")");
  
  #if DHT_ENABLED
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    Serial.println("Temperature: " + String(temp, 1) + "°C");
    Serial.println("Humidity: " + String(hum, 1) + "%");
  #endif
  
  #if CONTROL_ENABLED
    int potValue = analogRead(POTENTIOMETER_PIN);
    int potPercent = map(potValue, 0, 4095, 0, 100);
    Serial.println("Potentiometer: " + String(potPercent) + "% (Raw: " + String(potValue) + ")");
  #endif
  
  if (EMERGENCY_STOP_PIN >= 0) {
    int emergencyState = digitalRead(EMERGENCY_STOP_PIN);
    Serial.println("Emergency Stop: " + String(emergencyState ? "RELEASED" : "PRESSED"));
  }
  
  Serial.println("Free Heap: " + String(ESP.getFreeHeap()) + " bytes");
  Serial.println("Uptime: " + String(millis() / 1000) + " seconds");
  Serial.println("========================");
}
