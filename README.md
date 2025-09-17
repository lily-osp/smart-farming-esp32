# Smart Farming System for ESP32

A comprehensive IoT-based smart farming solution that automates irrigation based on soil moisture, temperature, and humidity readings. The system supports both offline and online operation modes with modular display options and dual cloud service integration.

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Installation Guide](#installation-guide)
- [Configuration Guide](#configuration-guide)
- [Display Options](#display-options)
- [Cloud Services Setup](#cloud-services-setup)
- [Usage Guide](#usage-guide)
- [Advanced Features](#advanced-features)
- [Troubleshooting](#troubleshooting)
- [Safety &amp; Best Practices](#safety--best-practices)
- [Performance Optimization](#performance-optimization)
- [Development Guide](#development-guide)
- [FAQ](#faq)
- [Contributing](#contributing)

## Project Structure

```
smart_farming/
├── MainCode/                    # Main smart farming system
│   ├── smart_farming_offline.ino    # Standalone version
│   ├── smart_farming_online.ino     # IoT version with cloud
│   ├── config.h                     # Configuration file
│   └── README.md                    # Main code documentation
├── TestCode/                    # Hardware testing
│   ├── hardware_test.ino            # Comprehensive hardware test
│   └── README.md                    # Test code documentation
├── docs/                        # Web documentation
│   ├── index.html               # Main documentation page
│   ├── styles.css               # Custom CSS styles
│   ├── script.js                # Interactive JavaScript
│   └── README.md                # Documentation guide
├── wiring_diagram.md           # Wiring instructions with Mermaid
├── libraries.txt              # Required Arduino libraries
├── LICENSE                     # MIT License
├── .gitignore                 # Git ignore rules
└── README.md                  # This file
```

## Project Overview

This smart farming system is designed to automate irrigation processes while providing comprehensive monitoring capabilities. It combines local sensor data with cloud connectivity to create a robust, scalable solution for modern agriculture.

### Key Benefits

- **Automated Irrigation**: Intelligent watering based on soil moisture levels
- **Real-time Monitoring**: Continuous sensor data collection and analysis
- **Cloud Integration**: Data logging and remote monitoring via ThingSpeak and Adafruit IO
- **Modular Design**: Flexible display options and configurable features
- **Fail-safe Operation**: Built-in safety mechanisms and error recovery
- **Remote Control**: Web-based interface for system management
- **OTA Updates**: Over-the-air firmware updates for remote maintenance

## Features

### Core Functionality

- **Soil Moisture Monitoring**: Analog sensor with configurable thresholds
- **Environmental Sensing**: DHT11/DHT22 temperature and humidity monitoring
- **Automated Irrigation**: Relay-controlled water pump with safety timers
- **Visual Feedback**: LED status indicators and optional LCD displays
- **Data Logging**: Comprehensive system state tracking

### Online Version Features

- **WiFi Connectivity**: Automatic connection and reconnection
- **Dual Cloud Services**: ThingSpeak and Adafruit IO integration
- **Web Interface**: Remote monitoring and control dashboard
- **OTA Updates**: Over-the-air firmware updates
- **JSON API**: RESTful API for system integration
- **Configurable Cloud Services**: Enable/disable individual services

### Safety & Reliability Features

- **Watchdog Timer**: Prevents system hangs and crashes
- **Emergency Stop**: Manual emergency shutdown capability
- **Pump Runtime Protection**: Prevents pump damage from continuous operation
- **Sensor Validation**: Range checking, consistency validation, and disconnection detection
- **Automatic Recovery**: Self-healing after system errors
- **Error Logging**: Comprehensive error tracking and reporting

## Hardware Requirements

### Essential Components

| Component                         | Specification             | Purpose                 | Notes                               |
| --------------------------------- | ------------------------- | ----------------------- | ----------------------------------- |
| **ESP32 Development Board** | ESP32-WROOM-32 or similar | Main controller         | Must support WiFi and ADC           |
| **Soil Moisture Sensor**    | Analog capacitive sensor  | Soil moisture detection | 0-3.3V output range                 |
| **DHT Sensor**              | DHT11 or DHT22            | Temperature/humidity    | Digital sensor with 1-wire protocol |
| **Relay Module**            | 5V SPDT relay             | Pump control            | Must handle pump current            |
| **Water Pump**              | 12V DC submersible        | Irrigation              | Match relay specifications          |
| **Power Supply**            | 5V/3.3V adapter           | System power            | Minimum 2A capacity                 |

### Optional Components

| Component              | Specification       | Purpose           | Notes                          |
| ---------------------- | ------------------- | ----------------- | ------------------------------ |
| **LCD Display**  | 1602 or 2004 I2C    | Visual monitoring | Optional - see Display Options |
| **LEDs**         | 5mm standard        | Status indication | Green, Red, Blue               |
| **Resistors**    | 220Ω, 10kΩ        | Current limiting  | For LEDs and pull-ups          |
| **Breadboard**   | Half-size           | Prototyping       | For development                |
| **Jumper Wires** | Male-to-male/female | Connections       | Various lengths                |

### Pin Connections

| Component                | ESP32 Pin | Notes                          | Alternative Pins |
| ------------------------ | --------- | ------------------------------ | ---------------- |
| **LCD SDA**        | GPIO 21   | I2C Data (if display enabled)  | GPIO 4, GPIO 16  |
| **LCD SCL**        | GPIO 22   | I2C Clock (if display enabled) | GPIO 5, GPIO 17  |
| **DHT Data**       | GPIO 5    | Digital input                  | GPIO 4, GPIO 18  |
| **Soil Moisture**  | GPIO 36   | Analog input (ADC1_CH0)        | GPIO 39, GPIO 34 |
| **Relay Control**  | GPIO 19   | Digital output                 | GPIO 18, GPIO 23 |
| **Green LED**      | GPIO 18   | Status indicator               | GPIO 2, GPIO 4   |
| **Red LED**        | GPIO 23   | Pump active indicator          | GPIO 2, GPIO 4   |
| **Blue LED**       | GPIO 2    | WiFi status (online version)   | GPIO 4, GPIO 5   |
| **Emergency Stop** | GPIO 0    | Optional emergency button      | GPIO 35, GPIO 34 |
| **Control System** |           |                                |                 |
| **Rotary Encoder CLK** | GPIO 16 | Clock pin (if encoder enabled) | GPIO 4, GPIO 5   |
| **Rotary Encoder DT**  | GPIO 17 | Data pin (if encoder enabled)  | GPIO 4, GPIO 5   |
| **Rotary Encoder SW**  | GPIO 4  | Switch/Button pin (if encoder) | GPIO 0, GPIO 35  |
| **Potentiometer**      | GPIO 34 | Analog input (if pot enabled)  | GPIO 35, GPIO 36 |

## Installation Guide

### Step 1: Software Setup

#### Arduino IDE Installation

1. **Download Arduino IDE** (version 1.8.19 or later)

   - [Official Arduino IDE](https://www.arduino.cc/en/software)
   - [Arduino IDE 2.0](https://www.arduino.cc/en/software) (recommended)
2. **Install ESP32 Board Support**

   ```bash
   # Add ESP32 board URL to Arduino IDE:
   # File → Preferences → Additional Board Manager URLs
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```

   Then install ESP32 board package:

   - Tools → Board → Boards Manager
   - Search for "ESP32"
   - Install "ESP32 by Espressif Systems"
3. **Install Required Libraries**

   ```bash
   # Install libraries via Arduino IDE Library Manager:
   # Tools → Manage Libraries → Search and install:

   - DHT sensor library (by Adafruit) v1.4.4+
   - LiquidCrystal I2C (by Frank de Brabander) v1.1.2+
   - ArduinoJson (by Benoit Blanchon) v6.19.4+
   - Adafruit IO Arduino (by Adafruit) v5.0.0+
   ```

#### PlatformIO Installation (Alternative)

```ini
# platformio.ini configuration
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = 
    adafruit/DHT sensor library@^1.4.4
    marcoschwartz/LiquidCrystal_I2C@^1.1.2
    bblanchon/ArduinoJson@^6.19.4
    adafruit/Adafruit IO Arduino@^5.0.0
```

### Step 2: Hardware Assembly

#### Basic Wiring Diagram

```
ESP32                    Components
├── GPIO 21 ──────────── LCD SDA (I2C)
├── GPIO 22 ──────────── LCD SCL (I2C)
├── GPIO 5 ───────────── DHT Data Pin
├── GPIO 36 ──────────── Soil Moisture Sensor (Analog)
├── GPIO 19 ──────────── Relay Control Pin
├── GPIO 18 ──────────── Green LED (220Ω resistor)
├── GPIO 23 ──────────── Red LED (220Ω resistor)
├── GPIO 2 ───────────── Blue LED (220Ω resistor)
├── 3.3V ─────────────── Sensor VCC, Relay VCC
├── GND ──────────────── Common Ground
└── 5V ───────────────── Relay VCC (if 5V relay)
```

#### Detailed Wiring Instructions

1. **Power Connections**

   - Connect ESP32 3.3V to sensor VCC pins
   - Connect ESP32 GND to all component ground pins
   - Use external 5V supply for relay if needed
2. **Sensor Connections**

   - **DHT Sensor**: Data pin to GPIO 5, VCC to 3.3V, GND to ground
   - **Soil Moisture**: Signal to GPIO 36, VCC to 3.3V, GND to ground
3. **Display Connections** (if using LCD)

   - **LCD I2C**: SDA to GPIO 21, SCL to GPIO 22, VCC to 3.3V, GND to ground
4. **Actuator Connections**

   - **Relay**: Control pin to GPIO 19, VCC to 5V, GND to ground
   - **Pump**: Connect to relay NO (Normally Open) terminal
5. **Status LEDs**

   - **Green LED**: Anode to GPIO 18, Cathode to ground (via 220Ω resistor)
   - **Red LED**: Anode to GPIO 23, Cathode to ground (via 220Ω resistor)
   - **Blue LED**: Anode to GPIO 2, Cathode to ground (via 220Ω resistor)

### Step 3: Code Upload

1. **Download Project Files**

   ```bash
   git clone <repository-url>
   cd smart_farming
   ```
2. **Configure System**

   - Edit `config.h` with your specific settings
   - Choose display type and cloud services
   - Set WiFi credentials (online version)
3. **Upload Code**

   - Select ESP32 board: Tools → Board → ESP32 Arduino → ESP32 Dev Module
   - Select COM port: Tools → Port → [Your ESP32 Port]
   - Upload: Sketch → Upload

## Configuration Guide

### Basic Configuration (`config.h`)

#### System Settings

```cpp
// System Operation Mode
#define DEBUG_MODE true                    // Enable debug output
#define SERIAL_BAUD_RATE 115200           // Serial communication speed

// Timing Configuration
#define SENSOR_READ_INTERVAL 2000         // Sensor reading interval (ms)
#define IRRIGATION_DURATION 10000         // Irrigation duration (ms)
#define MIN_IRRIGATION_INTERVAL 300000    // Minimum time between irrigations (ms)
```

#### Sensor Configuration

```cpp
// Soil Moisture Sensor
#define SOIL_MOISTURE_THRESHOLD 30        // Irrigation threshold (%)
#define SOIL_MOISTURE_WET_VALUE 4095      // Wet reading (in water)
#define SOIL_MOISTURE_DRY_VALUE 0         // Dry reading (in air)

// DHT Sensor
#define DHT_TYPE DHT22                    // DHT11 or DHT22
#define DHT_READ_INTERVAL 2000           // DHT reading interval (ms)
```

#### Display Configuration

```cpp
// Display Type Selection
#define DISPLAY_TYPE DISPLAY_LCD_1602     // Options: DISPLAY_NONE, DISPLAY_LCD_1602, DISPLAY_LCD_2004

// Display Settings
#define DISPLAY_UPDATE_INTERVAL 1000      // Display update interval (ms)
#define DISPLAY_SCROLL_DELAY 2000         // Screen cycling delay (ms)
#define SERIAL_OUTPUT_ENABLED true        // Enable serial output

// Control Type Selection
#define CONTROL_TYPE CONTROL_ROTARY_ENCODER  // Options: CONTROL_NONE, CONTROL_POTENTIOMETER, CONTROL_ROTARY_ENCODER

// Control Settings
#define MENU_TIMEOUT 30000               // Menu timeout (ms) - Rotary Encoder
#define ENCODER_STEP_SIZE 1              // Step size for parameter adjustment
#define POTENTIOMETER_UPDATE_INTERVAL 500 // Update interval (ms) - Potentiometer
```

### Advanced Configuration

#### Safety & Fail-safe Mechanisms

```cpp
// Watchdog Timer
#define WATCHDOG_TIMEOUT 30               // Watchdog timeout (seconds)
#define WATCHDOG_ENABLED true             // Enable watchdog timer

// Emergency Stop
#define EMERGENCY_STOP_ENABLED true       // Enable emergency stop
#define EMERGENCY_STOP_PIN 0              // Emergency stop button pin

// Pump Protection
#define MAX_PUMP_RUNTIME 300000           // Maximum pump runtime (ms)
#define PUMP_RUNTIME_PROTECTION true      // Enable pump protection

// System Recovery
#define AUTO_RECOVERY_ENABLED true        // Enable automatic recovery
#define RECOVERY_ATTEMPTS 3               // Maximum recovery attempts
#define RECOVERY_DELAY 5000               // Recovery delay (ms)
```

#### Sensor Validation

```cpp
// Temperature Validation
#define MIN_TEMPERATURE -10.0             // Minimum valid temperature (°C)
#define MAX_TEMPERATURE 60.0              // Maximum valid temperature (°C)
#define TEMPERATURE_VALIDATION true        // Enable temperature validation

// Humidity Validation
#define MIN_HUMIDITY 0.0                  // Minimum valid humidity (%)
#define MAX_HUMIDITY 100.0                // Maximum valid humidity (%)
#define HUMIDITY_VALIDATION true          // Enable humidity validation

// Soil Moisture Validation
#define MIN_SOIL_MOISTURE 0               // Minimum valid soil moisture (%)
#define MAX_SOIL_MOISTURE 100             // Maximum valid soil moisture (%)
#define SOIL_MOISTURE_VALIDATION true     // Enable soil moisture validation
#define MAX_SOIL_MOISTURE_CHANGE 20       // Maximum change between readings (%)

// Sensor Consistency
#define SENSOR_CONSISTENCY_CHECKS 3       // Readings for consistency check
#define SENSOR_CONSISTENCY_THRESHOLD 5    // Maximum deviation for consistent readings
#define CONSISTENCY_VALIDATION true       // Enable consistency validation

// Disconnection Detection
#define SENSOR_DISCONNECT_THRESHOLD 10    // Readings before marking disconnected
#define DISCONNECT_DETECTION true         // Enable disconnection detection
```

## Display Options

The system supports three display configurations for maximum flexibility:

### 1. LCD 1602 (16x2 characters) - Default

- **Configuration**: `#define DISPLAY_TYPE DISPLAY_LCD_1602`
- **Features**:
  - Cycling screens (3 screens)
  - Sensor data, system status, irrigation info
  - Compact 2-line display
  - Automatic screen cycling
- **Best for**: Basic monitoring, compact installations, educational projects
- **Wiring**: I2C connection to GPIO 21 (SDA) and GPIO 22 (SCL)
- **Screen Layout**:
  ```
  Screen 1: Temperature & Humidity → Soil Moisture
  Screen 2: System Status → Pump Status & Daily Count
  Screen 3: Irrigation Info → Last Irrigation Time
  ```

### 2. LCD 2004 (20x4 characters) - Enhanced

- **Configuration**: `#define DISPLAY_TYPE DISPLAY_LCD_2004`
- **Features**:
  - Single screen with all information
  - 4 lines of comprehensive data
  - Temperature, humidity, soil moisture, pump status
  - System status, WiFi status, daily irrigations
  - Cloud service status (online version)
- **Best for**: Professional installations, detailed monitoring, control rooms
- **Wiring**: I2C connection to GPIO 21 (SDA) and GPIO 22 (SCL)
- **Screen Layout**:
  ```
  Line 1: T:25.3C H:65.2% Soil:45% Pump:OFF
  Line 2: Status:OK WiFi:ON Errors:0
  Line 3: Daily:3 TS:ON AIO:ON
  Line 4: Up:60m Last:120s
  ```

### 3. No Display - Headless Mode

- **Configuration**: `#define DISPLAY_TYPE DISPLAY_NONE`
- **Features**:
  - Serial output only
  - Web interface (online version)
  - Comprehensive status logging
  - Lower power consumption
  - Smaller footprint
- **Best for**: Remote installations, cost optimization, IoT deployments, automated systems

## Control Options

The system supports three control configurations for maximum flexibility:

### 1. Rotary Encoder - Advanced Control (Default)
- **Configuration**: `#define CONTROL_TYPE CONTROL_ROTARY_ENCODER`
- **Features**: 
  - Interactive menu system
  - Real-time parameter adjustment
  - Push button for menu navigation
  - Rotary dial for value changes
  - Menu timeout protection
- **Best for**: Professional installations, user-friendly operation
- **Wiring**: CLK (GPIO 16), DT (GPIO 17), SW (GPIO 4)

#### Menu System
- **Soil Threshold**: Adjust irrigation trigger point (10-80%)
- **Irrigation Time**: View current irrigation duration
- **Display Speed**: View current display update speed
- **System Status**: View system health (offline) / WiFi Status (online)
- **Save & Exit**: Save settings and return to normal operation

### 2. Potentiometer - Simple Control
- **Configuration**: `#define CONTROL_TYPE CONTROL_POTENTIOMETER`
- **Features**:
  - Real-time threshold adjustment
  - Analog control (0-100%)
  - Visual feedback on display
  - Smooth value changes
- **Best for**: Simple installations, basic threshold adjustment
- **Wiring**: Center pin to GPIO 34, outer pins to 3.3V and GND

### 3. No Control - Fully Automated
- **Configuration**: `#define CONTROL_TYPE CONTROL_NONE`
- **Features**:
  - Fully automated operation
  - No manual intervention needed
  - Uses default thresholds
  - Clean, minimal interface
- **Best for**: Set-and-forget installations, remote monitoring
- **Benefits**: 
  - Lower cost
  - Simpler wiring
  - Reduced complexity
  - Reduced power consumption
  - Smaller physical footprint
  - Ideal for hidden installations

### Display Configuration Examples

#### For Basic Home Garden Setup

```cpp
#define DISPLAY_TYPE DISPLAY_LCD_1602
#define DISPLAY_SCROLL_DELAY 3000        // Slower cycling for easy reading
#define SERIAL_OUTPUT_ENABLED true       // Keep serial for debugging
```

#### For Professional Farm Installation

```cpp
#define DISPLAY_TYPE DISPLAY_LCD_2004
#define DISPLAY_UPDATE_INTERVAL 500      // Faster updates
#define SERIAL_OUTPUT_ENABLED true       // Logging for maintenance
```

#### For Remote IoT Deployment

```cpp
#define DISPLAY_TYPE DISPLAY_NONE
#define SERIAL_OUTPUT_ENABLED true       // Essential for remote monitoring
#define DEBUG_MODE false                 // Reduce serial output
```

## Cloud Services Setup

### ThingSpeak Configuration

#### Step 1: Create ThingSpeak Account

1. Visit [ThingSpeak.com](https://thingspeak.com)
2. Sign up for a free account
3. Verify your email address

#### Step 2: Create Channel

1. Click "New Channel"
2. Configure channel settings:
   ```
   Name: Smart Farming System
   Description: ESP32-based irrigation control
   Field 1: Temperature (°C)
   Field 2: Humidity (%)
   Field 3: Soil Moisture (%)
   Field 4: Pump Status (0/1)
   Field 5: Daily Irrigations
   Field 6: System Errors
   Field 7: WiFi Signal Strength
   Field 8: Uptime (minutes)
   ```
3. Save channel and note the Channel ID

#### Step 3: Get API Key

1. Go to "API Keys" tab
2. Copy the "Write API Key"
3. Update `config.h`:
   ```cpp
   #define THINGSPEAK_ENABLED true
   #define THINGSPEAK_API_KEY "YOUR_WRITE_API_KEY"
   #define THINGSPEAK_CHANNEL_ID 1234567
   ```

### Adafruit IO Configuration

#### Step 1: Create Adafruit IO Account

1. Visit [Adafruit IO](https://io.adafruit.com)
2. Sign up for a free account
3. Verify your email address

#### Step 2: Create Feeds

Create the following feeds:

- `temperature` (Number feed)
- `humidity` (Number feed)
- `soil-moisture` (Number feed)
- `pump-status` (Number feed)
- `irrigation-count` (Number feed)
- `system-status` (Number feed)
- `sensor-errors` (Number feed)

#### Step 3: Get Credentials

1. Go to "Adafruit IO Key" section
2. Copy your Username and Active Key
3. Update `config.h`:
   ```cpp
   #define ADAFRUIT_IO_ENABLED true
   #define ADAFRUIT_IO_USERNAME "your_username"
   #define ADAFRUIT_IO_KEY "your_active_key"
   ```

### Cloud Service Options

#### ThingSpeak Only

```cpp
#define THINGSPEAK_ENABLED true
#define ADAFRUIT_IO_ENABLED false
```

#### Adafruit IO Only

```cpp
#define THINGSPEAK_ENABLED false
#define ADAFRUIT_IO_ENABLED true
```

#### Both Services (Redundancy)

```cpp
#define THINGSPEAK_ENABLED true
#define ADAFRUIT_IO_ENABLED true
```

#### Offline Mode

```cpp
#define THINGSPEAK_ENABLED false
#define ADAFRUIT_IO_ENABLED false
```

## Usage Guide

### Offline Mode Operation

#### Initial Setup

1. **Power On**: Connect power supply to ESP32
2. **Serial Monitor**: Open Arduino IDE Serial Monitor (115200 baud)
3. **System Check**: Verify all sensors are detected
4. **Calibration**: Test soil moisture sensor in air and water

#### Daily Operation

1. **Monitoring**: Check LCD display or serial output
2. **Status LEDs**:
   - Green: System OK
   - Red: Pump active or error
   - Blue: Not used in offline mode
3. **Automatic Irrigation**: System waters when soil moisture < threshold
4. **Manual Override**: Use emergency stop if needed

#### Maintenance

- **Weekly**: Check sensor readings and pump operation
- **Monthly**: Clean sensors and check connections
- **Seasonally**: Recalibrate soil moisture sensor

### Online Mode Operation

#### Initial Setup

1. **WiFi Configuration**: Update WiFi credentials in `config.h`
2. **Cloud Setup**: Configure ThingSpeak and/or Adafruit IO
3. **Upload Code**: Flash ESP32 with online version
4. **Network Test**: Verify WiFi connection and cloud data transmission

#### Web Interface Access

1. **Find IP Address**: Check serial output for assigned IP
2. **Open Browser**: Navigate to `http://[ESP32_IP]`
3. **Dashboard**: View real-time sensor data and system status
4. **Control**: Start/stop irrigation manually
5. **Settings**: Adjust thresholds and intervals

#### Remote Monitoring

1. **ThingSpeak Dashboard**: View historical data and charts
2. **Adafruit IO Dashboard**: Monitor feeds and create alerts
3. **Mobile Access**: Use mobile browsers for remote monitoring
4. **API Integration**: Use JSON API for custom applications

### Display Modes

#### LCD 1602 Operation

- **Automatic Cycling**: Screens change every 2 seconds
- **Information Displayed**:
  - Temperature and humidity
  - Soil moisture percentage
  - System status (OK/ERROR)
  - Pump status (ON/OFF)
  - Daily irrigation count
  - Last irrigation time

#### LCD 2004 Operation

- **Static Display**: All information visible simultaneously
- **Information Displayed**:
  - Line 1: Temperature, humidity, soil moisture, pump status
  - Line 2: System status, WiFi status, error count
  - Line 3: Daily irrigations, cloud service status
  - Line 4: Uptime, last irrigation time

#### Headless Mode Operation

- **Serial Output**: Comprehensive status every second
- **Web Interface**: Full dashboard (online version)
- **API Access**: JSON endpoints for integration
- **Logging**: Detailed system logs for troubleshooting

### Control Modes

#### Rotary Encoder Operation
- **Menu Navigation**: 
  - Press button to enter menu mode
  - Rotate encoder to navigate through menu items
  - Press button to select item or enter parameter adjustment
- **Parameter Adjustment**:
  - Rotate encoder to increase/decrease values
  - Real-time display of current setting
  - Automatic save when exiting menu
- **Menu Items**:
  - Soil Threshold (10-80%)
  - Irrigation Time (view only)
  - Display Speed (view only)
  - System Status (view only)
  - Save & Exit

#### Potentiometer Operation
- **Real-time Control**: 
  - Turn knob to adjust irrigation threshold
  - Immediate effect on system behavior
  - Visual feedback on display
- **Range**: 10-80% soil moisture threshold
- **Update Rate**: Every 500ms for smooth operation

#### No Control Operation
- **Fully Automated**: 
  - Uses default soil moisture threshold (30%)
  - No manual intervention required
  - Ideal for remote installations
- **Benefits**: 
  - Lower cost
  - Simpler wiring
  - Reduced complexity

## Advanced Features

### Fail-safe Mechanisms

#### Watchdog Timer

- **Purpose**: Prevents system hangs and crashes
- **Configuration**: 30-second timeout (configurable)
- **Operation**: Automatically resets system if main loop stops
- **Benefits**: Ensures continuous operation in field conditions

#### Emergency Stop

- **Purpose**: Immediate system shutdown capability
- **Implementation**: GPIO pin monitoring for emergency button
- **Safety**: Stops irrigation immediately when activated
- **Recovery**: Requires manual reset after emergency stop

#### Pump Runtime Protection

- **Purpose**: Prevents pump damage from continuous operation
- **Configuration**: Maximum 5-minute runtime (configurable)
- **Safety**: Automatic shutdown if runtime exceeded
- **Monitoring**: Tracks pump start time and duration

#### Automatic System Recovery

- **Purpose**: Self-healing after system errors
- **Implementation**: Automatic sensor reinitialization
- **Recovery**: Up to 3 recovery attempts with delays
- **Logging**: Records recovery attempts and success/failure

### Sensor Validation System

#### Range Validation

- **Temperature**: -10°C to 60°C (configurable)
- **Humidity**: 0% to 100%
- **Soil Moisture**: 0% to 100%
- **Purpose**: Prevents erroneous readings from affecting irrigation

#### Consistency Validation

- **Method**: Compares current reading with recent history
- **Threshold**: 5% maximum deviation (configurable)
- **Checks**: 3 consecutive readings for consistency
- **Benefits**: Filters out sensor noise and temporary errors

#### Sudden Change Detection

- **Soil Moisture**: Maximum 20% change between readings
- **Purpose**: Detects sensor disconnection or malfunction
- **Response**: Marks reading as invalid, increments error counter
- **Recovery**: Automatic sensor reinitialization

#### Disconnection Detection

- **Method**: Tracks consecutive invalid readings
- **Threshold**: 10 consecutive errors (configurable)
- **Response**: Marks sensor as disconnected
- **Recovery**: Attempts automatic reconnection

### OTA (Over-the-Air) Updates

#### Setup

```cpp
// In config.h
#define OTA_ENABLED true
#define OTA_HOSTNAME "smart-farming-esp32"
#define OTA_PASSWORD "your_ota_password"
```

#### Usage

1. **Upload Initial Code**: Include OTA functionality
2. **Network Access**: Ensure ESP32 is on WiFi network
3. **Arduino IDE**: Use "Upload via Network" option
4. **Remote Updates**: Update firmware without physical access

#### Benefits

- **Remote Maintenance**: Update firmware from anywhere
- **No Physical Access**: Ideal for remote installations
- **Rollback Capability**: Revert to previous version if needed
- **Version Tracking**: Monitor firmware versions remotely

### Web Server Features

#### Dashboard Interface

- **Real-time Data**: Live sensor readings and system status
- **Control Panel**: Manual irrigation start/stop
- **Settings**: Adjustable thresholds and intervals
- **Status Indicators**: Visual system health indicators

#### JSON API Endpoints

```javascript
// Get system status
GET /api/status
Response: {
  "temperature": 25.3,
  "humidity": 65.2,
  "soilMoisture": 45,
  "pumpActive": false,
  "systemOK": true,
  "wifiConnected": true,
  "dailyIrrigations": 3,
  "sensorErrors": 0,
  "uptime": 3600
}

// Start irrigation
POST /api/irrigation/start
Response: {"success": true, "message": "Irrigation started"}

// Stop irrigation
POST /api/irrigation/stop
Response: {"success": true, "message": "Irrigation stopped"}
```

#### Mobile Responsiveness

- **Responsive Design**: Works on phones and tablets
- **Touch Controls**: Easy mobile interaction
- **Offline Capability**: Works without internet connection
- **Progressive Web App**: Can be installed on mobile devices

## Troubleshooting

### Common Issues and Solutions

#### Display Problems

**LCD not displaying**

- **Check**: I2C connections (SDA/SCL)
- **Verify**: LCD address (usually 0x27 or 0x3F)
- **Solution**: Use I2C scanner to find correct address
- **Config**: Ensure `DISPLAY_TYPE` matches your hardware

**LCD shows garbled text**

- **Check**: Power supply voltage (3.3V or 5V)
- **Verify**: Contrast adjustment on LCD
- **Solution**: Check wiring and power connections

**LCD backlight not working**

- **Check**: Backlight power connection
- **Verify**: Backlight control in code
- **Solution**: Ensure proper I2C backpack connection

#### Sensor Issues

**DHT sensor not reading**

- **Check**: Data pin connection (GPIO 5)
- **Verify**: Power supply (3.3V)
- **Solution**: Test with simple DHT example code
- **Alternative**: Try different GPIO pin

**Soil moisture sensor readings erratic**

- **Check**: Analog pin connection (GPIO 36)
- **Verify**: Power supply stability
- **Solution**: Add capacitor for power filtering
- **Calibration**: Test in air (dry) and water (wet)

**Sensor validation errors**

- **Check**: Sensor range settings in config.h
- **Verify**: Physical sensor condition
- **Solution**: Adjust validation thresholds
- **Recovery**: Enable automatic recovery in config

#### Network Issues

**WiFi not connecting**

- **Check**: WiFi credentials in config.h
- **Verify**: Signal strength and network availability
- **Solution**: Test with simple WiFi example
- **Alternative**: Use mobile hotspot for testing

**Cloud data not transmitting**

- **Check**: API keys and channel IDs
- **Verify**: Internet connectivity
- **Solution**: Test cloud services individually
- **Debug**: Enable debug mode for detailed logs

**Web interface not accessible**

- **Check**: ESP32 IP address in serial output
- **Verify**: Same network as ESP32
- **Solution**: Try different browser or clear cache
- **Alternative**: Use IP address instead of hostname

#### System Performance Issues

**System hangs or crashes**

- **Check**: Watchdog timer configuration
- **Verify**: Power supply stability
- **Solution**: Enable watchdog timer
- **Debug**: Monitor serial output for error patterns

**Pump not responding**

- **Check**: Relay connections and power
- **Verify**: Pump power supply
- **Solution**: Test relay with simple on/off code
- **Safety**: Check pump runtime protection settings

**Memory issues**

- **Check**: Available heap memory in serial output
- **Verify**: Library versions and compatibility
- **Solution**: Update to latest library versions
- **Optimization**: Reduce debug output or disable unused features

### Calibration Procedures

#### Soil Moisture Sensor Calibration

1. **Dry Calibration**: Place sensor in air, record reading
2. **Wet Calibration**: Place sensor in water, record reading
3. **Update Config**: Set `SOIL_MOISTURE_DRY_VALUE` and `SOIL_MOISTURE_WET_VALUE`
4. **Test**: Verify readings match expected percentages

#### DHT Sensor Calibration

1. **Temperature**: Compare with known thermometer
2. **Humidity**: Compare with hygrometer
3. **Offset**: Apply calibration offsets if needed
4. **Validation**: Enable sensor validation for error detection

#### System Timing Calibration

1. **Sensor Intervals**: Adjust based on required response time
2. **Irrigation Duration**: Test actual watering time needed
3. **Display Updates**: Balance between responsiveness and power consumption
4. **Cloud Transmission**: Optimize for data usage and reliability

### Debugging Techniques

#### Serial Monitor Usage

```cpp
// Enable debug mode
#define DEBUG_MODE true

// Monitor system output
Serial.println("System Status: " + String(systemOK ? "OK" : "ERROR"));
Serial.println("Sensor Errors: " + String(sensorErrors));
Serial.println("Free Heap: " + String(ESP.getFreeHeap()));
```

#### LED Status Indicators

- **Green LED**: System OK, sensors reading normally
- **Red LED**: Pump active or system error
- **Blue LED**: WiFi connected (online version)
- **Blinking Patterns**: Different error conditions

#### Web Interface Debugging

- **Status Page**: Real-time system information
- **Logs**: Historical error and event logs
- **Settings**: Current configuration values
- **Test Functions**: Manual sensor and actuator testing

## Safety & Best Practices

### Electrical Safety

#### Power Supply Considerations

- **Voltage**: Use appropriate voltage for each component
- **Current**: Ensure sufficient current capacity (minimum 2A)
- **Protection**: Use fuses or circuit breakers for protection
- **Grounding**: Proper grounding for safety

#### Wiring Safety

- **Insulation**: Use proper insulated wires
- **Connections**: Secure all connections to prevent shorts
- **Protection**: Use enclosures for outdoor installations
- **Moisture**: Protect connections from water damage

### Operational Safety

#### Pump Safety

- **Runtime Limits**: Never exceed maximum pump runtime
- **Power Supply**: Ensure adequate power for pump operation
- **Protection**: Use appropriate relay ratings for pump current
- **Monitoring**: Watch for pump overheating or unusual noise

#### System Safety

- **Emergency Stop**: Always provide emergency stop capability
- **Fail-safe**: Design for safe failure modes
- **Monitoring**: Regular system health checks
- **Backup**: Manual override capability for critical functions

### Environmental Considerations

#### Outdoor Installation

- **Weatherproofing**: Protect electronics from weather
- **Temperature**: Consider operating temperature range
- **Moisture**: Protect from rain and humidity
- **Sunlight**: Shield from direct sunlight

#### Maintenance Access

- **Location**: Install in accessible location for maintenance
- **Documentation**: Keep wiring diagrams and configuration records
- **Spare Parts**: Keep critical spare parts available
- **Testing**: Regular system testing and calibration

### Data Security

#### Network Security

- **WiFi**: Use strong passwords and WPA2/WPA3 encryption
- **OTA**: Use strong OTA passwords
- **API**: Implement API authentication if needed
- **Updates**: Keep firmware updated for security patches

#### Data Privacy

- **Cloud Services**: Understand data storage and privacy policies
- **Local Data**: Secure local data storage
- **Access Control**: Limit access to system controls
- **Monitoring**: Monitor for unauthorized access

## Performance Optimization

### Power Management

#### Sleep Modes

```cpp
// Deep sleep between readings (advanced)
esp_deep_sleep_start();

// Light sleep for power saving
esp_light_sleep_start();
```

#### Component Optimization

- **Display**: Reduce update frequency for power saving
- **Sensors**: Optimize reading intervals
- **WiFi**: Use power save mode when possible
- **CPU**: Reduce clock speed for lower power consumption

### Memory Optimization

#### RAM Usage

```cpp
// Monitor heap usage
Serial.println("Free Heap: " + String(ESP.getFreeHeap()));

// Use PROGMEM for constants
const char PROGMEM statusMessage[] = "System OK";
```

#### Flash Usage

- **Libraries**: Use only required libraries
- **Features**: Disable unused features
- **Debug**: Remove debug code in production
- **Optimization**: Use compiler optimization flags

### Network Optimization

#### WiFi Efficiency

```cpp
// WiFi power save mode
WiFi.setSleep(true);

// Optimize connection parameters
WiFi.setAutoReconnect(true);
WiFi.setAutoConnect(true);
```

#### Data Transmission

- **Compression**: Compress data before transmission
- **Batching**: Send multiple readings together
- **Frequency**: Optimize transmission intervals
- **Retry Logic**: Implement efficient retry mechanisms

### System Performance

#### Timing Optimization

- **Intervals**: Balance responsiveness with efficiency
- **Priorities**: Prioritize critical functions
- **Scheduling**: Use efficient task scheduling
- **Interrupts**: Use interrupts for time-critical functions

#### Error Handling

- **Recovery**: Fast error recovery mechanisms
- **Logging**: Efficient error logging
- **Monitoring**: Proactive system monitoring
- **Prevention**: Prevent errors through validation

## Development Guide

### Customization Options

#### Adding New Sensors

```cpp
// Example: Adding a light sensor
#define LIGHT_SENSOR_PIN 39
#define LIGHT_THRESHOLD 500

void readLightSensor() {
  int lightValue = analogRead(LIGHT_SENSOR_PIN);
  // Process light reading
}
```

#### Custom Display Layouts

```cpp
void customDisplayLayout() {
  #if DISPLAY_TYPE == DISPLAY_LCD_2004
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Custom Layout");
    // Add custom display logic
  #endif
}
```

#### Additional Cloud Services

```cpp
// Example: Adding MQTT support
#include <PubSubClient.h>

void publishToMQTT() {
  // MQTT publishing logic
}
```

### Extending Functionality

#### New Irrigation Modes

```cpp
// Example: Scheduled irrigation
void scheduledIrrigation() {
  // Check time-based irrigation
  if (hour() == 6 && minute() == 0) { // 6:00 AM
    startIrrigation();
  }
}
```

#### Advanced Analytics

```cpp
// Example: Data analysis
void analyzeTrends() {
  // Analyze sensor data trends
  // Predict irrigation needs
  // Optimize watering schedule
}
```

#### Integration with Other Systems

```cpp
// Example: Weather API integration
void checkWeatherForecast() {
  // Get weather data
  // Adjust irrigation based on forecast
}
```

### Code Structure

#### Modular Design

- **Configuration**: Centralized in `config.h`
- **Functions**: Organized by functionality
- **Libraries**: Separated by purpose
- **Documentation**: Comprehensive code comments

#### Best Practices

- **Naming**: Use descriptive variable and function names
- **Comments**: Document complex logic
- **Error Handling**: Comprehensive error handling
- **Testing**: Test individual components

### Contributing Guidelines

#### Code Standards

- **Formatting**: Follow Arduino coding standards
- **Documentation**: Document all functions and variables
- **Testing**: Test changes thoroughly
- **Compatibility**: Ensure backward compatibility

#### Pull Request Process

1. **Fork**: Fork the repository
2. **Branch**: Create feature branch
3. **Develop**: Implement changes with tests
4. **Document**: Update documentation
5. **Submit**: Submit pull request with description

## FAQ

### General Questions

**Q: Can I use this system without any display?**
A: Yes! Set `DISPLAY_TYPE` to `DISPLAY_NONE` for headless operation. The system will work perfectly with just serial output and web interface (online version).

**Q: What's the difference between offline and online versions?**
A: Offline version works standalone with local monitoring. Online version adds WiFi connectivity, cloud data logging, web interface, and OTA updates.

**Q: Can I use both ThingSpeak and Adafruit IO simultaneously?**
A: Yes! The system supports dual cloud services for redundancy. Enable both in `config.h` for maximum reliability.

**Q: How long can the system run without maintenance?**
A: With proper setup, the system can run for months. Regular maintenance includes checking sensors, cleaning connections, and monitoring system health.

### Hardware Questions

**Q: Can I use a different ESP32 board?**
A: Yes, any ESP32 board with WiFi and ADC support will work. Adjust pin assignments in `config.h` if needed.

**Q: What if my LCD has a different I2C address?**
A: Use an I2C scanner to find your LCD's address, then update `LCD_ADDRESS` in `config.h`.

**Q: Can I use a 5V relay instead of 3.3V?**
A: Yes, but you'll need a 5V power supply for the relay. The ESP32 GPIO pins are 3.3V, so ensure compatibility.

**Q: What pump should I use?**
A: Any 12V DC submersible pump that matches your relay's current rating. Consider flow rate and pressure requirements for your application.

### Software Questions

**Q: How do I update the firmware remotely?**
A: Use the OTA (Over-the-Air) update feature in the online version. Upload code via Arduino IDE's "Upload via Network" option.

**Q: Can I customize the web interface?**
A: Yes! The web interface is built with HTML/CSS/JavaScript. Modify the web server code to customize the interface.

**Q: How do I add more sensors?**
A: Add sensor code to the appropriate functions, update `config.h` with new pin definitions, and modify the display and cloud transmission functions.

**Q: Can I integrate this with home automation systems?**
A: Yes! Use the JSON API endpoints to integrate with systems like Home Assistant, OpenHAB, or custom automation scripts.

### Troubleshooting Questions

**Q: My sensors aren't reading correctly. What should I check?**
A: Check wiring connections, power supply voltage, sensor calibration, and validation settings in `config.h`.

**Q: The system keeps restarting. What's wrong?**
A: Check power supply stability, enable watchdog timer, review error logs, and ensure proper grounding.

**Q: Cloud data isn't transmitting. How do I debug?**
A: Check WiFi connection, API keys, internet connectivity, and enable debug mode to see detailed transmission logs.

**Q: The pump isn't turning on. What should I check?**
A: Verify relay connections, pump power supply, relay control pin, and check for safety mechanisms that might be preventing operation.

### Performance Questions

**Q: How can I reduce power consumption?**
A: Use headless mode, increase sensor reading intervals, enable WiFi power save mode, and use deep sleep between operations.

**Q: Can I run multiple systems on the same network?**
A: Yes, but ensure each system has a unique hostname and consider using different cloud channels for data separation.

**Q: How much data does the system use?**
A: Depends on transmission frequency. Typical usage is 1-5MB per month for basic monitoring with 15-minute intervals.

**Q: What's the maximum range for the WiFi connection?**
A: Depends on your router and environment. Typical range is 50-100 meters indoors, 100-300 meters outdoors with good antennas.

## Contributing

We welcome contributions to improve the Smart Farming System! Here's how you can help:

### Ways to Contribute

#### Code Contributions

- **Bug Fixes**: Report and fix bugs
- **New Features**: Add new functionality
- **Optimizations**: Improve performance and efficiency
- **Documentation**: Improve code documentation

#### Documentation Improvements

- **Tutorials**: Create step-by-step guides
- **Examples**: Provide usage examples
- **Translations**: Translate documentation to other languages
- **Video Guides**: Create video tutorials

#### Testing and Feedback

- **Testing**: Test on different hardware configurations
- **Feedback**: Report issues and suggest improvements
- **Use Cases**: Share your implementation stories
- **Performance**: Report performance characteristics

### Development Setup

#### Prerequisites

- Arduino IDE or PlatformIO
- ESP32 development board
- Required libraries (see libraries.txt)
- Git for version control

#### Getting Started

1. **Fork** the repository
2. **Clone** your fork locally
3. **Create** a feature branch
4. **Make** your changes
5. **Test** thoroughly
6. **Submit** a pull request

### Code Guidelines

#### Style Guidelines

- Follow Arduino coding standards
- Use meaningful variable and function names
- Comment complex logic
- Keep functions focused and small

#### Testing Requirements

- Test on actual hardware
- Verify all configurations work
- Test error conditions
- Ensure backward compatibility

#### Documentation Requirements

- Update README for new features
- Document configuration options
- Provide usage examples
- Include troubleshooting information

### Community

#### Getting Help

- **Issues**: Use GitHub issues for bug reports
- **Discussions**: Use GitHub discussions for questions
- **Wiki**: Check the wiki for additional documentation
- **Examples**: Look at example implementations

#### Sharing Your Work

- **Showcase**: Share your implementations
- **Tutorials**: Create tutorials for others
- **Improvements**: Suggest system improvements
- **Feedback**: Provide feedback on the project

---

## License

This project is open source and available under the MIT License. See the LICENSE file for details.

## Acknowledgments

- **ESP32 Community**: For excellent documentation and support
- **Arduino Community**: For the platform and libraries
- **Sensor Manufacturers**: For reliable sensor components
- **Open Source Contributors**: For the libraries and tools used

---

**Happy Farming!** 

For questions, issues, or contributions, please visit our [GitHub repository](https://github.com/lily-osp/smart-farming-esp32).
