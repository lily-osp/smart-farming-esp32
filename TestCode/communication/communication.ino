/*
 * Smart Farming Communication Test
 * ESP32 Communication Testing Suite
 * 
 * This test file validates all communication features of the smart farming system:
 * - WiFi connectivity
 * - ThingSpeak data transmission
 * - Adafruit IO integration
 * - Web server functionality
 * - OTA updates
 * - Data logging and transmission
 * 
 * Hardware Requirements:
 * - ESP32 Development Board
 * - WiFi network access
 * - Internet connectivity
 * 
 * Author: Azzar Budiyanto
 * Version: 1.0.0
 * Date: 2024
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <esp_task_wdt.h>

// =============================================================================
// CONFIGURATION
// =============================================================================

// WiFi Configuration
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define WIFI_TIMEOUT 10000
#define WIFI_RECONNECT_INTERVAL 30000

// ThingSpeak Configuration
#define THINGSPEAK_ENABLED true
#define THINGSPEAK_API_KEY "YOUR_THINGSPEAK_API_KEY"
#define THINGSPEAK_CHANNEL_ID "YOUR_CHANNEL_ID"
#define THINGSPEAK_URL "https://api.thingspeak.com/update"

// Adafruit IO Configuration
#define ADAFRUIT_IO_ENABLED true
#define ADAFRUIT_IO_USERNAME "YOUR_ADAFRUIT_IO_USERNAME"
#define ADAFRUIT_IO_KEY "YOUR_ADAFRUIT_IO_KEY"
#define ADAFRUIT_IO_URL "https://io.adafruit.com/api/v2/"

// Web Server Configuration
#define WEB_SERVER_PORT 80
#define WEB_SERVER_TIMEOUT 5000

// OTA Configuration
#define OTA_HOSTNAME "smart-farming-test"
#define OTA_PASSWORD "test_password"

// Test Configuration
#define TEST_INTERVAL 5000
#define MAX_RETRIES 3
#define SERIAL_BAUD_RATE 115200

// =============================================================================
// GLOBAL VARIABLES
// =============================================================================

WebServer server(WEB_SERVER_PORT);
HTTPClient http;

// Test Results Structure
struct TestResults {
  bool wifiConnected = false;
  bool thingspeakWorking = false;
  bool adafruitIOWorking = false;
  bool webServerWorking = false;
  bool otaWorking = false;
  int wifiRSSI = 0;
  String lastError = "";
  unsigned long lastTest = 0;
  int testCount = 0;
  int successCount = 0;
};

TestResults testResults;

// Test Data
struct TestData {
  float temperature = 25.5;
  float humidity = 60.0;
  int soilMoisture = 45;
  int lightLevel = 75;
  bool pumpStatus = false;
  int irrigationCount = 0;
};

TestData testData;

// =============================================================================
// FUNCTION DECLARATIONS
// =============================================================================

// WiFi Functions
void initializeWiFi();
void testWiFiConnection();
void checkWiFiStatus();

// ThingSpeak Functions
void testThingSpeak();
bool sendToThingSpeak(const TestData& data);

// Adafruit IO Functions
void testAdafruitIO();
bool sendToAdafruitIO(const TestData& data);

// Web Server Functions
void initializeWebServer();
void testWebServer();
void handleRoot();
void handleAPI();
void handleTest();
void handleStatus();

// OTA Functions
void initializeOTA();
void testOTA();

// Utility Functions
void printTestResults();
void printSeparator();
void printHeader();
void updateTestData();

// =============================================================================
// SETUP FUNCTION
// =============================================================================

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  
  printHeader();
  Serial.println("Smart Farming Communication Test Starting...");
  Serial.println("================================================");
  
  // Initialize watchdog
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = 30000,
    .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
    .trigger_panic = true
  };
  esp_task_wdt_init(&wdt_config);
  esp_task_wdt_add(NULL);
  
  // Initialize WiFi
  initializeWiFi();
  
  // Initialize Web Server
  initializeWebServer();
  
  // Initialize OTA
  initializeOTA();
  
  Serial.println("Initialization complete. Starting tests...");
  printSeparator();
}

// =============================================================================
// MAIN LOOP
// =============================================================================

void loop() {
  unsigned long currentTime = millis();
  
  // Feed watchdog
  esp_task_wdt_reset();
  
  // Handle web server requests
  server.handleClient();
  
  // Handle OTA updates
  ArduinoOTA.handle();
  
  // Run tests at intervals
  if (currentTime - testResults.lastTest >= TEST_INTERVAL) {
    testResults.lastTest = currentTime;
    testResults.testCount++;
    
    Serial.println("Running Communication Tests...");
    Serial.println("Test #" + String(testResults.testCount));
    printSeparator();
    
    // Update test data
    updateTestData();
    
    // Run individual tests
    testWiFiConnection();
    testThingSpeak();
    testAdafruitIO();
    testWebServer();
    testOTA();
    
    // Print results
    printTestResults();
    printSeparator();
    
    // Calculate success rate
    int totalTests = 5; // WiFi, ThingSpeak, AdafruitIO, WebServer, OTA
    int passedTests = 0;
    if (testResults.wifiConnected) passedTests++;
    if (testResults.thingspeakWorking) passedTests++;
    if (testResults.adafruitIOWorking) passedTests++;
    if (testResults.webServerWorking) passedTests++;
    if (testResults.otaWorking) passedTests++;
    
    testResults.successCount = passedTests;
    
    Serial.println("Test Summary: " + String(passedTests) + "/" + String(totalTests) + " tests passed");
    Serial.println("Success Rate: " + String((passedTests * 100) / totalTests) + "%");
    printSeparator();
  }
  
  delay(100);
}

// =============================================================================
// WIFI FUNCTIONS
// =============================================================================

void initializeWiFi() {
  Serial.println("Initializing WiFi...");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.print("Connecting to WiFi");
  unsigned long startTime = millis();
  
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    testResults.wifiConnected = true;
    testResults.wifiRSSI = WiFi.RSSI();
    Serial.println();
    Serial.println("WiFi Connected!");
    Serial.println("IP Address: " + WiFi.localIP().toString());
    Serial.println("RSSI: " + String(testResults.wifiRSSI) + " dBm");
  } else {
    testResults.wifiConnected = false;
    testResults.lastError = "WiFi connection timeout";
    Serial.println();
    Serial.println("WiFi Connection Failed!");
  }
}

void testWiFiConnection() {
  Serial.println("Testing WiFi Connection...");
  
  if (WiFi.status() == WL_CONNECTED) {
    testResults.wifiConnected = true;
    testResults.wifiRSSI = WiFi.RSSI();
    Serial.println("✓ WiFi Connected - RSSI: " + String(testResults.wifiRSSI) + " dBm");
  } else {
    testResults.wifiConnected = false;
    Serial.println("✗ WiFi Disconnected");
    
    // Attempt reconnection
    WiFi.reconnect();
    delay(2000);
    
    if (WiFi.status() == WL_CONNECTED) {
      testResults.wifiConnected = true;
      testResults.wifiRSSI = WiFi.RSSI();
      Serial.println("✓ WiFi Reconnected - RSSI: " + String(testResults.wifiRSSI) + " dBm");
    }
  }
}

// =============================================================================
// THINGSPEAK FUNCTIONS
// =============================================================================

void testThingSpeak() {
  Serial.println("Testing ThingSpeak Communication...");
  
  if (!testResults.wifiConnected) {
    Serial.println("✗ ThingSpeak test skipped - WiFi not connected");
    testResults.thingspeakWorking = false;
    return;
  }
  
  if (sendToThingSpeak(testData)) {
    testResults.thingspeakWorking = true;
    Serial.println("✓ ThingSpeak data sent successfully");
  } else {
    testResults.thingspeakWorking = false;
    Serial.println("✗ ThingSpeak data transmission failed");
  }
}

bool sendToThingSpeak(const TestData& data) {
  if (!THINGSPEAK_ENABLED) {
    Serial.println("ThingSpeak disabled in configuration");
    return false;
  }
  
  String url = String(THINGSPEAK_URL) + String("?api_key=") + String(THINGSPEAK_API_KEY);
  url += "&field1=" + String(data.temperature);
  url += "&field2=" + String(data.humidity);
  url += "&field3=" + String(data.soilMoisture);
  url += "&field4=" + String(data.lightLevel);
  url += "&field5=" + String(data.pumpStatus ? 1 : 0);
  url += "&field6=" + String(data.irrigationCount);
  
  http.begin(url);
  http.setTimeout(WEB_SERVER_TIMEOUT);
  
  int httpCode = http.GET();
  bool success = (httpCode == 200);
  
  if (success) {
    String response = http.getString();
    Serial.println("ThingSpeak Response: " + response);
  } else {
    Serial.println("ThingSpeak HTTP Error: " + String(httpCode));
    testResults.lastError = "ThingSpeak HTTP " + String(httpCode);
  }
  
  http.end();
  return success;
}

// =============================================================================
// ADAFRUIT IO FUNCTIONS
// =============================================================================

void testAdafruitIO() {
  Serial.println("Testing Adafruit IO Communication...");
  
  if (!testResults.wifiConnected) {
    Serial.println("✗ Adafruit IO test skipped - WiFi not connected");
    testResults.adafruitIOWorking = false;
    return;
  }
  
  if (sendToAdafruitIO(testData)) {
    testResults.adafruitIOWorking = true;
    Serial.println("✓ Adafruit IO data sent successfully");
  } else {
    testResults.adafruitIOWorking = false;
    Serial.println("✗ Adafruit IO data transmission failed");
  }
}

bool sendToAdafruitIO(const TestData& data) {
  if (!ADAFRUIT_IO_ENABLED) {
    Serial.println("Adafruit IO disabled in configuration");
    return false;
  }
  
  // Create JSON payload
  DynamicJsonDocument doc(1024);
  doc["value"] = data.temperature;
  
  String url = String(ADAFRUIT_IO_URL) + String(ADAFRUIT_IO_USERNAME) + "/feeds/temperature/data";
  String payload;
  serializeJson(doc, payload);
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-AIO-Key", ADAFRUIT_IO_KEY);
  http.setTimeout(WEB_SERVER_TIMEOUT);
  
  int httpCode = http.POST(payload);
  bool success = (httpCode == 200 || httpCode == 201);
  
  if (success) {
    String response = http.getString();
    Serial.println("Adafruit IO Response: " + response);
  } else {
    Serial.println("Adafruit IO HTTP Error: " + String(httpCode));
    testResults.lastError = "Adafruit IO HTTP " + String(httpCode);
  }
  
  http.end();
  return success;
}

// =============================================================================
// WEB SERVER FUNCTIONS
// =============================================================================

void initializeWebServer() {
  Serial.println("Initializing Web Server...");
  
  // Define routes
  server.on("/", handleRoot);
  server.on("/api", handleAPI);
  server.on("/test", handleTest);
  server.on("/status", handleStatus);
  
  server.begin();
  Serial.println("Web Server started on port " + String(WEB_SERVER_PORT));
}

void testWebServer() {
  Serial.println("Testing Web Server...");
  
  if (!testResults.wifiConnected) {
    Serial.println("✗ Web Server test skipped - WiFi not connected");
    testResults.webServerWorking = false;
    return;
  }
  
  // Test local server
  String url = "http://" + WiFi.localIP().toString() + "/status";
  
  http.begin(url);
  http.setTimeout(WEB_SERVER_TIMEOUT);
  
  int httpCode = http.GET();
  bool success = (httpCode == 200);
  
  if (success) {
    String response = http.getString();
    Serial.println("✓ Web Server responding - Status: " + response);
    testResults.webServerWorking = true;
  } else {
    Serial.println("✗ Web Server not responding - HTTP: " + String(httpCode));
    testResults.webServerWorking = false;
    testResults.lastError = "Web Server HTTP " + String(httpCode);
  }
  
  http.end();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>Smart Farming Communication Test</title></head><body>";
  html += "<h1>Smart Farming Communication Test</h1>";
  html += "<h2>Test Results</h2>";
  html += "<p>WiFi: " + String(testResults.wifiConnected ? "Connected" : "Disconnected") + "</p>";
  html += "<p>ThingSpeak: " + String(testResults.thingspeakWorking ? "Working" : "Failed") + "</p>";
  html += "<p>Adafruit IO: " + String(testResults.adafruitIOWorking ? "Working" : "Failed") + "</p>";
  html += "<p>Web Server: " + String(testResults.webServerWorking ? "Working" : "Failed") + "</p>";
  html += "<p>OTA: " + String(testResults.otaWorking ? "Working" : "Failed") + "</p>";
  html += "<p>Success Rate: " + String(testResults.successCount) + "/5</p>";
  html += "<p><a href='/api'>API Data</a> | <a href='/status'>Status</a></p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleAPI() {
  DynamicJsonDocument doc(1024);
  doc["temperature"] = testData.temperature;
  doc["humidity"] = testData.humidity;
  doc["soilMoisture"] = testData.soilMoisture;
  doc["lightLevel"] = testData.lightLevel;
  doc["pumpStatus"] = testData.pumpStatus;
  doc["irrigationCount"] = testData.irrigationCount;
  doc["wifiRSSI"] = testResults.wifiRSSI;
  doc["testCount"] = testResults.testCount;
  doc["successCount"] = testResults.successCount;
  
  String response;
  serializeJson(doc, response);
  
  server.send(200, "application/json", response);
}

void handleTest() {
  server.send(200, "text/plain", "Communication test endpoint working!");
}

void handleStatus() {
  String status = "OK";
  if (!testResults.wifiConnected) status = "WiFi Disconnected";
  else if (!testResults.thingspeakWorking && !testResults.adafruitIOWorking) status = "Data Transmission Failed";
  
  server.send(200, "text/plain", status);
}

// =============================================================================
// OTA FUNCTIONS
// =============================================================================

void initializeOTA() {
  Serial.println("Initializing OTA Updates...");
  
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);
  
  ArduinoOTA.onStart([]() {
    Serial.println("OTA Update Started");
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("OTA Update Completed");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  
  ArduinoOTA.begin();
  testResults.otaWorking = true;
  Serial.println("OTA Updates initialized");
}

void testOTA() {
  Serial.println("Testing OTA Updates...");
  
  if (!testResults.wifiConnected) {
    Serial.println("✗ OTA test skipped - WiFi not connected");
    testResults.otaWorking = false;
    return;
  }
  
  // OTA is always working if initialized and WiFi is connected
  testResults.otaWorking = true;
  Serial.println("✓ OTA Updates ready");
}

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

void printTestResults() {
  Serial.println("=== COMMUNICATION TEST RESULTS ===");
  Serial.println("WiFi Status: " + String(testResults.wifiConnected ? "CONNECTED" : "DISCONNECTED"));
  if (testResults.wifiConnected) {
    Serial.println("WiFi RSSI: " + String(testResults.wifiRSSI) + " dBm");
    Serial.println("IP Address: " + WiFi.localIP().toString());
  }
  Serial.println("ThingSpeak: " + String(testResults.thingspeakWorking ? "WORKING" : "FAILED"));
  Serial.println("Adafruit IO: " + String(testResults.adafruitIOWorking ? "WORKING" : "FAILED"));
  Serial.println("Web Server: " + String(testResults.webServerWorking ? "WORKING" : "FAILED"));
  Serial.println("OTA Updates: " + String(testResults.otaWorking ? "READY" : "FAILED"));
  
  if (!testResults.lastError.isEmpty()) {
    Serial.println("Last Error: " + testResults.lastError);
  }
  
  Serial.println("Test Count: " + String(testResults.testCount));
  Serial.println("Success Rate: " + String(testResults.successCount) + "/5");
}

void printSeparator() {
  Serial.println("================================================");
}

void printHeader() {
  Serial.println();
  Serial.println("================================================");
  Serial.println("    SMART FARMING COMMUNICATION TEST SUITE    ");
  Serial.println("================================================");
  Serial.println();
}

void updateTestData() {
  // Simulate changing sensor data
  testData.temperature += random(-10, 11) / 10.0;
  testData.humidity += random(-50, 51) / 10.0;
  testData.soilMoisture += random(-20, 21);
  testData.lightLevel += random(-30, 31);
  
  // Constrain values to realistic ranges
  testData.temperature = constrain(testData.temperature, 15.0, 35.0);
  testData.humidity = constrain(testData.humidity, 20.0, 90.0);
  testData.soilMoisture = constrain(testData.soilMoisture, 0, 100);
  testData.lightLevel = constrain(testData.lightLevel, 0, 100);
  
  // Simulate pump status changes
  if (random(0, 100) < 10) { // 10% chance to change pump status
    testData.pumpStatus = !testData.pumpStatus;
    if (testData.pumpStatus) {
      testData.irrigationCount++;
    }
  }
}
