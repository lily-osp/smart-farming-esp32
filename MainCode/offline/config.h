/*
 * Smart Farming Configuration File
 * ESP32 Modular Smart Farming
 * 
 * This file contains all configurable parameters for the smart farming system
 * Modify these values according to your hardware setup and requirements
 */

#ifndef CONFIG_H
#define CONFIG_H

// =============================================================================
// HARDWARE PIN CONFIGURATION
// =============================================================================

// =============================================================================
// DISPLAY CONFIGURATION (Modular Display Options)
// =============================================================================

// Display Type Selection
// Options: DISPLAY_NONE, DISPLAY_LCD_1602, DISPLAY_LCD_2004
#define DISPLAY_TYPE DISPLAY_LCD_1602

// Display Type Definitions
#define DISPLAY_NONE 0
#define DISPLAY_LCD_1602 1
#define DISPLAY_LCD_2004 2

// LCD I2C Configuration
#define LCD_SDA_PIN 21
#define LCD_SCL_PIN 22
#define LCD_ADDRESS 0x27  // Common I2C address for LCD with backpack

// Display-specific Configuration
#if DISPLAY_TYPE == DISPLAY_LCD_1602
  #define LCD_COLS 16
  #define LCD_ROWS 2
  #define DISPLAY_SCREEN_COUNT 3  // Number of screens to cycle through
#elif DISPLAY_TYPE == DISPLAY_LCD_2004
  #define LCD_COLS 20
  #define LCD_ROWS 4
  #define DISPLAY_SCREEN_COUNT 1  // Single screen with all info
#else
  #define LCD_COLS 0
  #define LCD_ROWS 0
  #define DISPLAY_SCREEN_COUNT 0
#endif

// Display Features
#define DISPLAY_ENABLED (DISPLAY_TYPE != DISPLAY_NONE)
#define DISPLAY_CYCLING_ENABLED (DISPLAY_TYPE == DISPLAY_LCD_1602)

// =============================================================================
// CONTROL CONFIGURATION (Modular Control Options)
// =============================================================================

// Control Type Selection
// Options: CONTROL_NONE, CONTROL_POTENTIOMETER, CONTROL_ROTARY_ENCODER
#define CONTROL_TYPE CONTROL_ROTARY_ENCODER

// Control Type Definitions
#define CONTROL_NONE 0
#define CONTROL_POTENTIOMETER 1
#define CONTROL_ROTARY_ENCODER 2

// Control-specific Configuration
#if CONTROL_TYPE == CONTROL_POTENTIOMETER
  #define POTENTIOMETER_PIN 34  // ADC1_CH6 - GPIO34 for potentiometer
  #define POTENTIOMETER_UPDATE_INTERVAL 500  // Update interval (ms)
#elif CONTROL_TYPE == CONTROL_ROTARY_ENCODER
  #define ENCODER_CLK_PIN 16    // GPIO16 - Clock pin
  #define ENCODER_DT_PIN 17     // GPIO17 - Data pin  
  #define ENCODER_SW_PIN 4      // GPIO4 - Switch pin (push button)
  #define ENCODER_DEBOUNCE_TIME 50  // Debounce time (ms)
  #define ENCODER_STEP_SIZE 1    // Step size for parameter adjustment
  #define MENU_TIMEOUT 30000     // Menu timeout (ms)
#endif

// Common threshold limits for all control types
#define POTENTIOMETER_MIN_THRESHOLD 10   // Minimum soil moisture threshold (%)
#define POTENTIOMETER_MAX_THRESHOLD 80  // Maximum soil moisture threshold (%)

// Control Features
#define CONTROL_ENABLED (CONTROL_TYPE != CONTROL_NONE)
#define MENU_SYSTEM_ENABLED (CONTROL_TYPE == CONTROL_ROTARY_ENCODER)
#define REAL_TIME_CONTROL_ENABLED (CONTROL_TYPE == CONTROL_POTENTIOMETER)

// =============================================================================
// SENSOR CONFIGURATION (Modular Sensor Options)
// =============================================================================

// DHT Sensor Selection
// Options: DHT_NONE, DHT_TYPE_11, DHT_TYPE_22
#define DHT_SENSOR_TYPE DHT_TYPE_22

// DHT Sensor Type Definitions
#define DHT_NONE 0
#define DHT_TYPE_11 1
#define DHT_TYPE_22 2

// LDR Sensor Selection
// Options: LDR_NONE, LDR_TYPE_ENABLED
#define LDR_SENSOR_TYPE LDR_NONE

// LDR Sensor Type Definitions
#define LDR_NONE 0
#define LDR_TYPE_ENABLED 1

// Sensor Pins
#define SOIL_MOISTURE_PIN 36  // ADC1_CH0
#define LDR_PIN 39            // ADC1_CH3 - GPIO39 for LDR sensor

// DHT Sensor Configuration (conditional)
#if DHT_SENSOR_TYPE == DHT_TYPE_11
  #define DHT_PIN 5
  #define DHT_TYPE DHT11
#elif DHT_SENSOR_TYPE == DHT_TYPE_22
  #define DHT_PIN 5
  #define DHT_TYPE DHT22
#else
  #define DHT_PIN 0  // Not used
  #define DHT_TYPE 0  // Not used
#endif

// Sensor Features
#define DHT_ENABLED (DHT_SENSOR_TYPE != DHT_NONE)
#define LDR_ENABLED (LDR_SENSOR_TYPE != LDR_NONE)
#define TEMPERATURE_MONITORING_ENABLED DHT_ENABLED
#define HUMIDITY_MONITORING_ENABLED DHT_ENABLED
#define LIGHT_MONITORING_ENABLED LDR_ENABLED

// Actuator Pins
#define RELAY_PIN 19

// LED Status Pins
#define LED_GREEN_PIN 18   // System OK
#define LED_RED_PIN 23     // Pump Active
#define LED_BLUE_PIN 2     // WiFi Connected (online version only)

// =============================================================================
// SENSOR CALIBRATION
// =============================================================================

// Soil Moisture Sensor Calibration
#define SOIL_MOISTURE_DRY_VALUE 4095    // Value when sensor is in air (dry)
#define SOIL_MOISTURE_WET_VALUE 0       // Value when sensor is in water (wet)
#define SOIL_MOISTURE_THRESHOLD 30      // Percentage threshold for irrigation (0-100%)

// LDR Sensor Calibration
#define LDR_DARK_VALUE 4095             // Value when sensor is in complete darkness
#define LDR_BRIGHT_VALUE 0              // Value when sensor is in bright light
#define LDR_READ_INTERVAL 1000          // Minimum interval between LDR readings (ms)
#define LDR_LOW_LIGHT_THRESHOLD 20      // Percentage threshold for low light detection (0-100%)
#define LDR_HIGH_LIGHT_THRESHOLD 80     // Percentage threshold for high light detection (0-100%)

// DHT Sensor Settings
#define DHT_READ_INTERVAL 2000  // Minimum interval between DHT readings (ms)

// =============================================================================
// IRRIGATION CONTROL
// =============================================================================

// Irrigation Settings
#define IRRIGATION_DURATION 5000        // Duration of irrigation in milliseconds
#define IRRIGATION_COOLDOWN 300000       // Cooldown period between irrigations (5 minutes)
#define MAX_DAILY_IRRIGATIONS 10        // Maximum irrigations per day
#define MANUAL_IRRIGATION_DURATION 10000 // Duration for manual irrigation (10 seconds)

// =============================================================================
// DISPLAY AND USER INTERFACE
// =============================================================================

// Display Settings
#define DISPLAY_UPDATE_INTERVAL 1000    // Display update interval (ms)
#define DISPLAY_SCROLL_DELAY 2000       // Delay between display screens (ms) - LCD 1602 only
#define SERIAL_OUTPUT_ENABLED true      // Enable serial output (always recommended)

// Menu System (for Rotary Encoder)
#define MENU_ITEMS 5                    // Number of menu items
#define PARAMETER_MIN_STEP 1           // Minimum step size for parameters
#define PARAMETER_MAX_STEP 10          // Maximum step size for parameters

// =============================================================================
// SYSTEM BEHAVIOR
// =============================================================================

// Timing Configuration
#define SENSOR_READ_INTERVAL 5000       // Main sensor reading interval (ms)
#define STATUS_CHECK_INTERVAL 1000      // Status check interval (ms)
#define SYSTEM_STARTUP_DELAY 2000       // Startup delay for sensor initialization

// Error Handling
#define MAX_SENSOR_ERRORS 5             // Maximum consecutive sensor errors before alert
#define SENSOR_ERROR_TIMEOUT 10000      // Timeout for sensor error recovery (ms)

// =============================================================================
// SAFETY AND FAIL-SAFE MECHANISMS
// =============================================================================

// Watchdog Timer
#define WATCHDOG_TIMEOUT 30             // Watchdog timeout in seconds
#define WATCHDOG_ENABLED true           // Enable/disable watchdog timer

// Emergency Stop
#define EMERGENCY_STOP_ENABLED true     // Enable emergency stop functionality
#define EMERGENCY_STOP_PIN 0            // GPIO pin for emergency stop button (optional)

// Pump Protection
#define MAX_PUMP_RUNTIME 300000         // Maximum continuous pump runtime (5 minutes)
#define PUMP_RUNTIME_PROTECTION true    // Enable pump runtime protection

// System Recovery
#define AUTO_RECOVERY_ENABLED true      // Enable automatic system recovery
#define RECOVERY_ATTEMPTS 3             // Maximum recovery attempts
#define RECOVERY_DELAY 5000             // Delay between recovery attempts (ms)

// =============================================================================
// SENSOR VALIDATION AND CALIBRATION
// =============================================================================

// Temperature Validation
#define MIN_TEMPERATURE -10.0           // Minimum valid temperature (°C)
#define MAX_TEMPERATURE 60.0            // Maximum valid temperature (°C)
#define TEMPERATURE_VALIDATION true     // Enable temperature validation

// Humidity Validation
#define MIN_HUMIDITY 0.0                // Minimum valid humidity (%)
#define MAX_HUMIDITY 100.0              // Maximum valid humidity (%)
#define HUMIDITY_VALIDATION true        // Enable humidity validation

// Soil Moisture Validation
#define MIN_SOIL_MOISTURE 0             // Minimum valid soil moisture (%)
#define MAX_SOIL_MOISTURE 100           // Maximum valid soil moisture (%)
#define SOIL_MOISTURE_VALIDATION true   // Enable soil moisture validation
#define MAX_SOIL_MOISTURE_CHANGE 20     // Maximum change between readings (%)

// LDR Sensor Validation
#define MIN_LIGHT_LEVEL 0               // Minimum valid light level (%)
#define MAX_LIGHT_LEVEL 100             // Maximum valid light level (%)
#define LIGHT_VALIDATION true           // Enable light level validation
#define MAX_LIGHT_CHANGE 30             // Maximum change between readings (%)

// Sensor Consistency
#define SENSOR_CONSISTENCY_CHECKS 3     // Number of readings for consistency check
#define SENSOR_CONSISTENCY_THRESHOLD 5  // Maximum deviation for consistent readings
#define CONSISTENCY_VALIDATION true     // Enable sensor consistency validation

// Sensor Disconnection Detection
#define SENSOR_DISCONNECT_THRESHOLD 10  // Readings before marking sensor disconnected
#define DISCONNECT_DETECTION true       // Enable sensor disconnection detection

// =============================================================================
// WIFI CONFIGURATION (Online Version Only)
// =============================================================================

// WiFi Settings
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define WIFI_TIMEOUT 10000              // WiFi connection timeout (ms)
#define WIFI_RECONNECT_INTERVAL 30000   // WiFi reconnection interval (ms)

// ThingSpeak Configuration
#define THINGSPEAK_ENABLED true            // Enable/disable ThingSpeak
#define THINGSPEAK_API_KEY "YOUR_THINGSPEAK_API_KEY"
#define THINGSPEAK_CHANNEL_ID "YOUR_CHANNEL_ID"
#define THINGSPEAK_UPDATE_INTERVAL 60000  // Data upload interval (ms)

// Adafruit IO Configuration
#define ADAFRUIT_IO_ENABLED true           // Enable/disable Adafruit IO
#define ADAFRUIT_IO_USERNAME "YOUR_ADAFRUIT_IO_USERNAME"
#define ADAFRUIT_IO_KEY "YOUR_ADAFRUIT_IO_KEY"
#define ADAFRUIT_IO_UPDATE_INTERVAL 60000  // Data upload interval (ms)

// Adafruit IO Feed Names
#define ADAFRUIT_IO_TEMPERATURE_FEED "temperature"
#define ADAFRUIT_IO_HUMIDITY_FEED "humidity"
#define ADAFRUIT_IO_SOIL_MOISTURE_FEED "soil-moisture"
#define ADAFRUIT_IO_LIGHT_LEVEL_FEED "light-level"
#define ADAFRUIT_IO_PUMP_STATUS_FEED "pump-status"
#define ADAFRUIT_IO_IRRIGATION_COUNT_FEED "irrigation-count"

// Web Server Configuration
#define WEB_SERVER_PORT 80
#define WEB_SERVER_TIMEOUT 5000

// OTA Update Configuration
#define OTA_HOSTNAME "smart-farming-esp32"
#define OTA_PASSWORD "your_ota_password"

// =============================================================================
// DATA LOGGING
// =============================================================================

// Data Logging Settings
#define LOG_BUFFER_SIZE 50              // Number of readings to store locally
#define LOG_INTERVAL 300000             // Local logging interval (5 minutes)

// Data Transmission
#define DATA_TRANSMISSION_INTERVAL 300000  // Data transmission interval (ms)
#define MAX_TRANSMISSION_RETRIES 3       // Maximum retries for data transmission

// =============================================================================
// DEBUGGING AND MONITORING
// =============================================================================

// Serial Communication
#define SERIAL_BAUD_RATE 115200
#define DEBUG_MODE true                 // Enable/disable debug output

// System Monitoring
#define HEARTBEAT_INTERVAL 60000        // System heartbeat interval (ms)
#define MEMORY_CHECK_INTERVAL 300000    // Memory check interval (ms)

// =============================================================================
// ADVANCED FEATURES
// =============================================================================

// Power Management
#define SLEEP_MODE_ENABLED false        // Enable sleep mode for power saving
#define SLEEP_DURATION 300000           // Sleep duration in milliseconds

// Security
#define ENABLE_WEB_AUTH true           // Enable web interface authentication
#define WEB_USERNAME "admin"
#define WEB_PASSWORD "smartfarm123"

// =============================================================================
// CALIBRATION VALUES (Auto-calculated)
// =============================================================================

// These values are calculated automatically based on sensor readings
// Do not modify unless you know what you're doing
#define SOIL_MOISTURE_RANGE (SOIL_MOISTURE_DRY_VALUE - SOIL_MOISTURE_WET_VALUE)

// =============================================================================
// VERSION INFORMATION
// =============================================================================

#define FIRMWARE_VERSION "1.0.0"
#define HARDWARE_VERSION "1.0"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

#endif // CONFIG_H
