/*
 * ===============================================================================
 * ESP32 SMART FARMING SYSTEM - OFFLINE VERSION CONFIGURATION
 * ===============================================================================
 * 
 * This file contains all configuration settings for your smart farming system.
 * Follow the step-by-step guide below to configure your system properly.
 * 
 * QUICK START GUIDE:
 * 1. Choose your setup type in STEP 1 (Basic/Advanced/Professional)
 * 2. Configure your hardware connections in STEP 2
 * 3. Adjust irrigation settings in STEP 3
 * 4. Upload the code to your ESP32
 * 
 * Author: Azzar Budiyanto
 * Version: 1.8.0
 * Date: 2024
 * ===============================================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

// ===============================================================================
// STEP 1: CHOOSE YOUR SETUP TYPE (Select ONE)
// ===============================================================================

/*
 * Uncomment ONE of the following setup types based on your hardware:
 * 
 * BASIC_SETUP:        Soil moisture sensor only, no display, fully automatic
 * STANDARD_SETUP:     Soil moisture + DHT22 + LCD 1602 display
 * POTENTIOMETER_SETUP: Standard + potentiometer for threshold control
 * PROFESSIONAL_SETUP: All sensors + LCD 2004 + rotary encoder control
 * CUSTOM_SETUP:       Configure everything manually below
 */

// #define BASIC_SETUP
// #define STANDARD_SETUP  
#define POTENTIOMETER_SETUP
// #define PROFESSIONAL_SETUP
// #define CUSTOM_SETUP

// ===============================================================================
// STEP 2: HARDWARE PIN CONFIGURATION
// ===============================================================================

/*
 * ESP32 Pin Connection Diagram:
 * 
 *    ESP32 Dev Board
 *    ┌─────────────────┐
 *    │  3.3V  ●    ●   │ VIN
 *    │   GND  ●    ●   │ GND  
 *    │  GPIO2 ●    ●   │ GPIO4  <- Encoder Button (Professional)
 *    │  GPIO5 ●    ●   │ GPIO16 <- Encoder CLK (Professional)
 *    │ GPIO18 ●    ●   │ GPIO17 <- Encoder DT (Professional)
 *    │ GPIO19 ●    ●   │ GPIO21 <- LCD SDA
 *    │ GPIO21 ●    ●   │ GPIO22 <- LCD SCL
 *    │ GPIO22 ●    ●   │ GPIO23 <- LED Red
 *    │ GPIO23 ●    ●   │ GPIO34 <- Potentiometer
 *    │ GPIO36 ●    ●   │ GPIO39 <- LDR Sensor
 *    └─────────────────┘
 *    
 * Required Connections:
 * - Soil Moisture Sensor → GPIO36 (always required)
 * - Water Pump Relay     → GPIO19 (always required) 
 * - Green Status LED     → GPIO18 (recommended)
 * - Red Pump LED         → GPIO23 (recommended)
 * 
 * Optional Connections (based on your setup):
 * - DHT22 Sensor         → GPIO5
 * - LCD Display SDA      → GPIO21
 * - LCD Display SCL      → GPIO22
 * - Potentiometer        → GPIO34
 * - LDR Light Sensor     → GPIO39
 * - Rotary Encoder CLK   → GPIO16
 * - Rotary Encoder DT    → GPIO17
 * - Rotary Encoder SW    → GPIO4
 */

// Essential Hardware Pins (DO NOT CHANGE unless you know what you're doing)
#define SOIL_MOISTURE_PIN 36        // Soil moisture sensor (ADC1_CH0)
#define RELAY_PIN 19               // Water pump relay control
#define LED_GREEN_PIN 18           // System status LED (green = OK)
#define LED_RED_PIN 23             // Pump status LED (red = pumping)
#define LED_BLUE_PIN 2             // General purpose LED

// Sensor Pins (change if needed)
#define DHT_PIN 5                  // DHT22/DHT11 temperature/humidity sensor
#define LDR_PIN 39                 // LDR light sensor (ADC1_CH3)

// Display Pins (I2C LCD)
#define LCD_SDA_PIN 21             // LCD data line
#define LCD_SCL_PIN 22             // LCD clock line
#define LCD_ADDRESS 0x27           // LCD I2C address (try 0x3F if 0x27 doesn't work)

// Control Pins
#define POTENTIOMETER_PIN 34       // Potentiometer for threshold control (ADC1_CH6)
#define ENCODER_CLK_PIN 16         // Rotary encoder clock
#define ENCODER_DT_PIN 17          // Rotary encoder data
#define ENCODER_SW_PIN 4           // Rotary encoder switch/button

// ===============================================================================
// STEP 3: IRRIGATION SETTINGS (IMPORTANT!)
// ===============================================================================

/*
 * SOIL MOISTURE THRESHOLD:
 * - Lower value = drier soil before watering (5-15% for succulents)
 * - Higher value = moister soil before watering (30-50% for vegetables)
 * 
 * Common Values:
 * - Succulents/Cacti:     10-15%
 * - Herbs:                20-25%
 * - Vegetables:           30-40%
 * - Tropical Plants:      40-50%
 */
#define SOIL_MOISTURE_THRESHOLD 30    // When to start watering (0-100%)

/*
 * IRRIGATION TIMING:
 * - IRRIGATION_DURATION: How long to run the pump (milliseconds)
 * - IRRIGATION_COOLDOWN: Minimum time between waterings (milliseconds)
 * 
 * Recommended Values:
 * - Small plants: 3-5 seconds (3000-5000 ms)
 * - Large plants: 10-15 seconds (10000-15000 ms)
 * - Cooldown: 5-30 minutes (300000-1800000 ms)
 */
#define IRRIGATION_DURATION 5000      // Pump runtime per irrigation (5 seconds)
#define IRRIGATION_COOLDOWN 300000    // Wait time between irrigations (5 minutes)
#define MAX_DAILY_IRRIGATIONS 10     // Maximum waterings per day (safety limit)

// ===============================================================================
// AUTOMATIC CONFIGURATION BASED ON SETUP TYPE
// ===============================================================================

#ifdef BASIC_SETUP
  // Minimal setup: Soil sensor only, no display, automatic operation
  #define DHT_SENSOR_TYPE DHT_NONE
  #define LDR_SENSOR_TYPE LDR_NONE
  #define DISPLAY_TYPE DISPLAY_NONE
  #define CONTROL_TYPE CONTROL_NONE
  #define SERIAL_OUTPUT_ENABLED true
  #define DEBUG_MODE false
  
#elif defined(STANDARD_SETUP)
  // Standard setup: Soil + DHT22 + LCD 1602
  #define DHT_SENSOR_TYPE DHT_TYPE_22
  #define LDR_SENSOR_TYPE LDR_NONE
  #define DISPLAY_TYPE DISPLAY_LCD_1602
  #define CONTROL_TYPE CONTROL_NONE
  #define SERIAL_OUTPUT_ENABLED true
  #define DEBUG_MODE false
  
#elif defined(POTENTIOMETER_SETUP)
  // Potentiometer setup: Standard + potentiometer control
  #define DHT_SENSOR_TYPE DHT_TYPE_22
  #define LDR_SENSOR_TYPE LDR_NONE
  #define DISPLAY_TYPE DISPLAY_LCD_1602
  #define CONTROL_TYPE CONTROL_POTENTIOMETER
  #define SERIAL_OUTPUT_ENABLED true
  #define DEBUG_MODE true
  
#elif defined(PROFESSIONAL_SETUP)
  // Professional setup: All sensors + LCD 2004 + rotary encoder
  #define DHT_SENSOR_TYPE DHT_TYPE_22
  #define LDR_SENSOR_TYPE LDR_TYPE_ENABLED
  #define DISPLAY_TYPE DISPLAY_LCD_2004
  #define CONTROL_TYPE CONTROL_ROTARY_ENCODER
  #define SERIAL_OUTPUT_ENABLED true
  #define DEBUG_MODE true
  
#elif defined(CUSTOM_SETUP)
  // Custom setup: Configure everything manually below
  #define DHT_SENSOR_TYPE DHT_TYPE_22        // DHT_NONE, DHT_TYPE_11, DHT_TYPE_22
  #define LDR_SENSOR_TYPE LDR_NONE           // LDR_NONE, LDR_TYPE_ENABLED
  #define DISPLAY_TYPE DISPLAY_LCD_1602      // DISPLAY_NONE, DISPLAY_LCD_1602, DISPLAY_LCD_2004
  #define CONTROL_TYPE CONTROL_POTENTIOMETER // CONTROL_NONE, CONTROL_POTENTIOMETER, CONTROL_ROTARY_ENCODER
  #define SERIAL_OUTPUT_ENABLED true         // true = enable serial output, false = disable
  #define DEBUG_MODE false                   // true = detailed debug info, false = basic info
  
#else
  #error "Please select a setup type in STEP 1 above!"
#endif

// ===============================================================================
// SENSOR TYPE DEFINITIONS (DO NOT MODIFY)
// ===============================================================================

// DHT Sensor Types
#define DHT_NONE 0
#define DHT_TYPE_11 1
#define DHT_TYPE_22 2

// LDR Sensor Types  
#define LDR_NONE 0
#define LDR_TYPE_ENABLED 1

// Display Types
#define DISPLAY_NONE 0
#define DISPLAY_LCD_1602 1
#define DISPLAY_LCD_2004 2

// Control Types
#define CONTROL_NONE 0
#define CONTROL_POTENTIOMETER 1
#define CONTROL_ROTARY_ENCODER 2

// ===============================================================================
// SENSOR CONFIGURATION (AUTOMATIC)
// ===============================================================================

// DHT Sensor Configuration
#if DHT_SENSOR_TYPE == DHT_TYPE_11
  #define DHT_TYPE DHT11
#elif DHT_SENSOR_TYPE == DHT_TYPE_22
  #define DHT_TYPE DHT22
#else
  #define DHT_TYPE 0  // Not used
#endif

// Sensor Feature Flags
#define DHT_ENABLED (DHT_SENSOR_TYPE != DHT_NONE)
#define LDR_ENABLED (LDR_SENSOR_TYPE != LDR_NONE)
#define TEMPERATURE_MONITORING_ENABLED DHT_ENABLED
#define HUMIDITY_MONITORING_ENABLED DHT_ENABLED
#define LIGHT_MONITORING_ENABLED LDR_ENABLED

// ===============================================================================
// DISPLAY CONFIGURATION (AUTOMATIC)
// ===============================================================================

// Display-specific Configuration
#if DISPLAY_TYPE == DISPLAY_LCD_1602
  #define LCD_COLS 16
  #define LCD_ROWS 2
  #define DISPLAY_SCREEN_COUNT 3        // Number of screens to cycle through
#elif DISPLAY_TYPE == DISPLAY_LCD_2004
  #define LCD_COLS 20
  #define LCD_ROWS 4
  #define DISPLAY_SCREEN_COUNT 1        // Single screen with all info
#else
  #define LCD_COLS 0
  #define LCD_ROWS 0
  #define DISPLAY_SCREEN_COUNT 0
#endif

// Display Feature Flags
#define DISPLAY_ENABLED (DISPLAY_TYPE != DISPLAY_NONE)
#define DISPLAY_CYCLING_ENABLED (DISPLAY_TYPE == DISPLAY_LCD_1602)

// Display Timing
#define DISPLAY_UPDATE_INTERVAL 1000    // How often to update display (ms)
#define DISPLAY_SCROLL_DELAY 4000       // Time between screens on LCD 1602 (ms) - slower for better readability

// ===============================================================================
// CONTROL SYSTEM CONFIGURATION (AUTOMATIC)
// ===============================================================================

// Control Feature Flags
#define CONTROL_ENABLED (CONTROL_TYPE != CONTROL_NONE)
#define MENU_SYSTEM_ENABLED (CONTROL_TYPE == CONTROL_ROTARY_ENCODER)
#define REAL_TIME_CONTROL_ENABLED (CONTROL_TYPE == CONTROL_POTENTIOMETER)

// Potentiometer Settings (when CONTROL_POTENTIOMETER is selected)
#if CONTROL_TYPE == CONTROL_POTENTIOMETER
  #define POTENTIOMETER_UPDATE_INTERVAL 100    // How often to read potentiometer (ms)
  #define POTENTIOMETER_SMOOTHING_SAMPLES 5    // Number of readings to average
  #define POTENTIOMETER_HYSTERESIS 2           // Prevent jittery switching (%)
  #define POTENTIOMETER_DEADBAND 50            // Ignore small ADC fluctuations
  #define POTENTIOMETER_MIN_THRESHOLD 5        // Minimum threshold setting (%)
  #define POTENTIOMETER_MAX_THRESHOLD 50       // Maximum threshold setting (%)
#endif

// Rotary Encoder Settings (when CONTROL_ROTARY_ENCODER is selected)
#if CONTROL_TYPE == CONTROL_ROTARY_ENCODER
  #define ENCODER_DEBOUNCE_TIME 50      // Button debounce time (ms)
  #define ENCODER_STEP_SIZE 1           // Threshold adjustment step size
  #define MENU_TIMEOUT 30000            // Menu timeout (ms)
  #define MENU_ITEMS 5                  // Number of menu items
#endif

// ===============================================================================
// SENSOR CALIBRATION AND VALIDATION
// ===============================================================================

/*
 * SOIL MOISTURE SENSOR CALIBRATION:
 * 
 * To calibrate your soil moisture sensor:
 * 1. Put sensor in dry air → note the value → set as SOIL_MOISTURE_DRY_VALUE
 * 2. Put sensor in water → note the value → set as SOIL_MOISTURE_WET_VALUE
 * 
 * Default values work for most capacitive sensors:
 * - Dry (in air): 4095 (maximum ADC value)
 * - Wet (in water): 0-500 (very low ADC value)
 */
#define SOIL_MOISTURE_DRY_VALUE 4095    // Sensor reading when completely dry
#define SOIL_MOISTURE_WET_VALUE 0       // Sensor reading when completely wet

// DHT Sensor Settings
#define DHT_READ_INTERVAL 2000          // Minimum time between DHT readings (ms)

// LDR Sensor Calibration (if enabled)
#define LDR_DARK_VALUE 4095             // Sensor reading in complete darkness
#define LDR_BRIGHT_VALUE 0              // Sensor reading in bright light
#define LDR_READ_INTERVAL 1000          // Time between LDR readings (ms)
#define LDR_LOW_LIGHT_THRESHOLD 20      // Percentage threshold for low light detection (0-100%)
#define LDR_HIGH_LIGHT_THRESHOLD 80     // Percentage threshold for high light detection (0-100%)

// Sensor Validation Ranges (for error detection)
#define MIN_TEMPERATURE -10.0           // Minimum valid temperature (°C)
#define MAX_TEMPERATURE 60.0            // Maximum valid temperature (°C)
#define MIN_HUMIDITY 0.0                // Minimum valid humidity (%)
#define MAX_HUMIDITY 100.0              // Maximum valid humidity (%)
#define MIN_SOIL_MOISTURE 0             // Minimum valid soil moisture (%)
#define MAX_SOIL_MOISTURE 100           // Maximum valid soil moisture (%)
#define MIN_LIGHT_LEVEL 0               // Minimum valid light level (%)
#define MAX_LIGHT_LEVEL 100             // Maximum valid light level (%)

// Sensor Validation Settings
#define TEMPERATURE_VALIDATION true     // Enable temperature range checking
#define HUMIDITY_VALIDATION true        // Enable humidity range checking
#define SOIL_MOISTURE_VALIDATION true   // Enable soil moisture range checking
#define LIGHT_VALIDATION true           // Enable light level range checking
#define MAX_SOIL_MOISTURE_CHANGE 20     // Maximum change between readings (%)
#define MAX_LIGHT_CHANGE 30             // Maximum light change between readings (%)

// Sensor Consistency Checking
#define SENSOR_CONSISTENCY_CHECKS 3     // Number of readings to compare
#define SENSOR_CONSISTENCY_THRESHOLD 5  // Maximum deviation for consistency
#define CONSISTENCY_VALIDATION true     // Enable consistency checking

// ===============================================================================
// SYSTEM BEHAVIOR AND TIMING
// ===============================================================================

// Main System Timing
#define SENSOR_READ_INTERVAL 5000       // How often to read all sensors (ms)
#define STATUS_CHECK_INTERVAL 1000      // How often to check system status (ms)
#define HEARTBEAT_INTERVAL 60000        // System heartbeat message interval (ms)

// Error Handling
#define MAX_SENSOR_ERRORS 5             // Max consecutive sensor errors before alert
#define SENSOR_ERROR_TIMEOUT 10000      // Timeout for sensor error recovery (ms)

// Serial Communication
#define SERIAL_BAUD_RATE 115200         // Serial monitor baud rate

// ===============================================================================
// SAFETY AND PROTECTION FEATURES
// ===============================================================================

/*
 * SAFETY FEATURES:
 * These features protect your system and plants from damage.
 * It's recommended to keep these enabled unless you have a specific reason.
 */

// Watchdog Timer (prevents system freezing)
#define WATCHDOG_ENABLED true           // Enable automatic system restart if frozen
#define WATCHDOG_TIMEOUT 30             // Watchdog timeout in seconds

// Pump Protection (prevents pump damage)
#define PUMP_RUNTIME_PROTECTION true    // Enable maximum pump runtime protection
#define MAX_PUMP_RUNTIME 300000         // Maximum continuous pump runtime (5 minutes)

// Emergency Stop (manual system shutdown)
#define EMERGENCY_STOP_ENABLED true     // Enable emergency stop functionality
#define EMERGENCY_STOP_PIN 0            // GPIO pin for emergency stop button (optional)

// Automatic Recovery (self-healing system)
#define AUTO_RECOVERY_ENABLED true      // Enable automatic error recovery
#define RECOVERY_ATTEMPTS 3             // Maximum recovery attempts
#define RECOVERY_DELAY 5000             // Delay between recovery attempts (ms)

// Sensor Disconnection Detection
#define DISCONNECT_DETECTION true       // Enable sensor disconnection detection
#define SENSOR_DISCONNECT_THRESHOLD 10  // Readings before marking sensor disconnected

// ===============================================================================
// ADVANCED SETTINGS (FOR EXPERIENCED USERS ONLY)
// ===============================================================================

/*
 * WARNING: Only modify these settings if you understand their impact!
 * Incorrect values may cause system instability or poor performance.
 */

// Memory and Performance
#define LOG_BUFFER_SIZE 50              // Number of readings to store in memory
#define LOG_INTERVAL 300000             // Data logging interval (5 minutes)
#define MEMORY_CHECK_INTERVAL 300000    // Memory usage check interval (ms)
#define SYSTEM_STARTUP_DELAY 2000       // Startup delay for sensor stabilization (ms)

// Power Management (experimental)
#define SLEEP_MODE_ENABLED false        // Enable deep sleep mode (saves power)
#define SLEEP_DURATION 300000           // Sleep duration in milliseconds

// Manual Irrigation (for testing)
#define MANUAL_IRRIGATION_DURATION 10000  // Duration for manual irrigation (10 seconds)

// Parameter Adjustment (for rotary encoder)
#define PARAMETER_MIN_STEP 1            // Minimum parameter adjustment step
#define PARAMETER_MAX_STEP 10           // Maximum parameter adjustment step

// ===============================================================================
// CALCULATED VALUES (DO NOT MODIFY)
// ===============================================================================

// These values are automatically calculated based on your settings above
#define SOIL_MOISTURE_RANGE (SOIL_MOISTURE_DRY_VALUE - SOIL_MOISTURE_WET_VALUE)

// ===============================================================================
// VERSION INFORMATION
// ===============================================================================

#define FIRMWARE_VERSION "1.0.0"
#define HARDWARE_VERSION "1.0"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

// ===============================================================================
// CONFIGURATION VALIDATION
// ===============================================================================

// Validate essential settings
#if !defined(SOIL_MOISTURE_THRESHOLD) || SOIL_MOISTURE_THRESHOLD < 0 || SOIL_MOISTURE_THRESHOLD > 100
  #error "SOIL_MOISTURE_THRESHOLD must be between 0 and 100!"
#endif

#if !defined(IRRIGATION_DURATION) || IRRIGATION_DURATION < 1000
  #error "IRRIGATION_DURATION must be at least 1000ms (1 second)!"
#endif

#if !defined(IRRIGATION_COOLDOWN) || IRRIGATION_COOLDOWN < 60000
  #warning "IRRIGATION_COOLDOWN is less than 1 minute. This may cause overwatering!"
#endif

// ===============================================================================
// SETUP COMPLETE
// ===============================================================================

/*
 * CONGRATULATIONS! Your smart farming system is now configured.
 * 
 * NEXT STEPS:
 * 1. Double-check your hardware connections match the pin assignments above
 * 2. Install required libraries (see LIBRARY_SETUP.md)
 * 3. Upload this code to your ESP32
 * 4. Open Serial Monitor (115200 baud) to see system status
 * 5. Test your system with a small amount of water first!
 * 
 * TROUBLESHOOTING:
 * - If sensors show invalid readings, check your wiring
 * - If pump doesn't activate, verify relay connections and power supply
 * - If LCD doesn't work, try changing LCD_ADDRESS from 0x27 to 0x3F
 * - For more help, see the documentation or enable DEBUG_MODE
 * 
 * SAFETY REMINDER:
 * - Always test with small amounts of water first
 * - Monitor your system for the first few days
 * - Ensure proper drainage to prevent overwatering
 * - Use appropriate power supplies for your pump
 */

#endif // CONFIG_H