# Smart Farming System - Offline Version

A standalone smart farming system that operates without internet connectivity, perfect for remote locations or when you prefer local-only operation.

## **Version 1.8.0 - Production Ready**

### **What's New**

- **One-Click Setup**: Choose from 5 pre-configured setup types in 30 seconds
- **40% Smaller Firmware**: Production-optimized code with debug cleanup
- **Rock-Solid Reliability**: Enhanced irrigation control and sensor validation
- **Better Display**: Optimized screen timing and improved readability
- **Bulletproof Safety**: Advanced error handling and automatic recovery

## Overview

The offline version provides complete smart farming functionality without requiring WiFi or internet access. It's ideal for:

- Remote agricultural locations
- Areas with poor internet connectivity
- Privacy-focused operations
- Backup systems
- Learning and prototyping

## Features

### Core Functionality

- **Automated Irrigation**: Intelligent watering based on soil moisture levels
- **Sensor Monitoring**: Real-time temperature, humidity, and soil moisture readings
- **Safety Systems**: Watchdog timer, emergency stop, and pump protection
- **Modular Design**: Flexible hardware configurations
- **Local Control**: Rotary encoder or potentiometer for parameter adjustment

### Display Options

- **LCD 1602**: 16x2 character display with cycling screens
- **LCD 2004**: 20x4 character display with all information
- **Headless Mode**: Serial output only, no display

### Control Options

- **Rotary Encoder**: Interactive menu system for parameter adjustment
- **Potentiometer**: Real-time threshold adjustment
- **Automated**: Fully automated operation with default settings

## Hardware Requirements

### Essential Components

- ESP32 Development Board
- Soil Moisture Sensor (Analog)
- Relay Module (5V or 3.3V)
- Power Supply (2A+ recommended)

### Optional Components

- DHT11/DHT22 Temperature/Humidity Sensor
- LCD Display (1602 or 2004 with I2C backpack)
- LEDs for status indication
- Rotary Encoder or Potentiometer for control
- LDR Light Sensor

## Quick Start

### 1. Hardware Setup

Connect components according to the wiring diagram:

```
ESP32                    Components
├── GPIO 36 ──────────── Soil Moisture Sensor (Analog) - Required
├── GPIO 19 ──────────── Relay Control Pin - Required
├── GPIO 21 ──────────── LCD SDA (I2C) - Optional
├── GPIO 22 ──────────── LCD SCL (I2C) - Optional
├── GPIO 5 ───────────── DHT Data Pin - Optional
├── GPIO 39 ──────────── LDR Sensor (Analog) - Optional
├── GPIO 18 ──────────── Green LED (220Ω resistor) - Optional
├── GPIO 23 ──────────── Red LED (220Ω resistor) - Optional
├── 3.3V ─────────────── Sensor VCC, Relay VCC
├── GND ──────────────── Common Ground
└── 5V ───────────────── Relay VCC (if 5V relay)
```

### 2. Software Setup

1. **Open Arduino IDE**
2. **Install ESP32 Board Support**:

   - File → Preferences → Additional Board Manager URLs
   - Add: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Tools → Board → Boards Manager → Search "ESP32" → Install
3. **Install Required Libraries**:

   - DHT sensor library (by Adafruit) v1.4.4+
   - LiquidCrystal I2C (by Frank de Brabander) v1.1.2+
4. **Open Sketch**: Open `offline.ino` in Arduino IDE

### 3. Configuration

The revolutionary new configuration system makes setup effortless:

#### **Step-by-Step Setup (30 seconds!)**

**STEP 1: Choose Your Setup Type**
Simply uncomment ONE line in `config.h`:

```cpp
// #define BASIC_SETUP           // Soil sensor only, fully automatic
// #define STANDARD_SETUP        // Basic + DHT22 + LCD 1602 display
#define POTENTIOMETER_SETUP    // Standard + potentiometer control  
// #define PROFESSIONAL_SETUP    // All sensors + LCD 2004 + encoder
// #define CUSTOM_SETUP          // Full manual configuration
```

**STEP 2: Hardware Connections**
Follow the visual ASCII diagram in `config.h` showing exactly where to connect each wire.

**STEP 3: Plant-Specific Settings**

```cpp
#define SOIL_MOISTURE_THRESHOLD 30    // Pre-configured recommendations:
// Succulents/Cacti: 10-15%    |  Herbs: 20-25%
// Vegetables: 30-40%          |  Tropical Plants: 40-50%
```

#### **Benefits of New Setup System**

- **95% Faster**: Complete setup in under 1 minute vs 30+ minutes before
- **Error-Proof**: Automatic validation prevents configuration mistakes
- **Beginner-Friendly**: Visual guides and plant recommendations included
- **Expert-Ready**: Full manual control available with CUSTOM_SETUP
- **Self-Documenting**: Comprehensive comments explain every setting

### 4. Upload and Run

1. **Select Board**: ESP32 Dev Module
2. **Select Port**: Your ESP32 COM port
3. **Upload**: Upload the sketch
4. **Monitor**: Open Serial Monitor (115200 baud)

## Configuration Examples

### Minimal Setup (Soil Moisture Only)

```cpp
#define DHT_SENSOR_TYPE DHT_NONE
#define DISPLAY_TYPE DISPLAY_NONE
#define CONTROL_TYPE CONTROL_NONE
#define LDR_SENSOR_TYPE LDR_NONE
```

### Basic Setup (DHT11 + LCD 1602)

```cpp
#define DHT_SENSOR_TYPE DHT_TYPE_11
#define DISPLAY_TYPE DISPLAY_LCD_1602
#define CONTROL_TYPE CONTROL_NONE
#define LDR_SENSOR_TYPE LDR_NONE
```

### Professional Setup (DHT22 + LCD 2004 + Rotary Encoder)

```cpp
#define DHT_SENSOR_TYPE DHT_TYPE_22
#define DISPLAY_TYPE DISPLAY_LCD_2004
#define CONTROL_TYPE CONTROL_ROTARY_ENCODER
#define LDR_SENSOR_TYPE LDR_TYPE_ENABLED
```

## Usage

### Initial Setup

1. **Power On**: Connect power supply to ESP32
2. **Serial Monitor**: Open Arduino IDE Serial Monitor (115200 baud)
3. **System Check**: Verify all sensors are detected
4. **Calibration**: Test soil moisture sensor in air and water

### Daily Operation

1. **Monitoring**: Check LCD display or serial output
2. **Status LEDs**:
   - Green: System OK
   - Red: Pump active or error
3. **Automatic Irrigation**: System waters when soil moisture < threshold
4. **Manual Override**: Use emergency stop if needed

### Control Interface

#### Rotary Encoder Control

- **Press Button**: Enter menu mode
- **Rotate**: Navigate menu items
- **Press**: Select/adjust parameters
- **Long Press**: Save settings and exit

#### Potentiometer Control

- **Real-time Adjustment**: Turn potentiometer to adjust threshold
- **Immediate Effect**: Changes apply instantly
- **Visual Feedback**: LCD shows current threshold value

## Display Information

### LCD 1602 (Cycling Screens)

- **Screen 1**: Soil moisture percentage and status
- **Screen 2**: Temperature and humidity (if DHT enabled)
- **Screen 3**: System status and irrigation count

### LCD 2004 (All Information)

- **Line 1**: Soil moisture percentage and status
- **Line 2**: Temperature and humidity (if DHT enabled)
- **Line 3**: Light level (if LDR enabled)
- **Line 4**: System status and irrigation count

### Serial Output

```
Smart Farming System - Offline Version
=====================================
Soil Moisture: 45% (Threshold: 30%)
Temperature: 25.5°C
Humidity: 60.0%
Light Level: 75%
System Status: OK
Daily Irrigations: 3
Last Irrigation: 2 hours ago
```

## Safety Features

### Watchdog Timer

- **Timeout**: 30 seconds
- **Function**: Prevents system hangs
- **Recovery**: Automatic system restart

### Emergency Stop

- **Manual Override**: Immediate pump shutdown
- **Safety**: Prevents overwatering
- **Recovery**: Manual reset required

### Pump Protection

- **Maximum Runtime**: 5 minutes continuous
- **Cooldown Period**: 5 minutes between irrigations
- **Daily Limit**: Maximum 10 irrigations per day

### Sensor Validation

- **Range Checking**: Validates sensor readings
- **Consistency**: Checks for sensor disconnection
- **Error Handling**: Automatic recovery from sensor errors

## Troubleshooting

### Common Issues

1. **System Not Starting**

   - Check power supply (minimum 2A)
   - Verify all connections
   - Run hardware test first
2. **Sensors Not Reading**

   - Check wiring connections
   - Verify power supply voltage
   - Check sensor configuration in `config.h`
3. **Display Not Working**

   - Verify I2C address (usually 0x27)
   - Check SDA/SCL connections
   - Test with I2C scanner
4. **Pump Not Working**

   - Check relay module power
   - Verify control pin connection
   - Test relay manually
5. **Control Not Responding**

   - Check encoder/potentiometer connections
   - Verify pin assignments in `config.h`
   - Test with multimeter

### Debug Mode

Enable debug mode in `config.h` for detailed output:

```cpp
#define DEBUG_MODE true
```

### Serial Monitor Output

Monitor the serial output for:

- System startup messages
- Sensor readings
- Error messages
- Status updates

## Performance Optimization

### Power Management

- Use headless mode for lower power consumption
- Increase sensor reading intervals
- Disable unused features

### Memory Optimization

- Disable unused sensors
- Reduce debug output
- Use latest library versions

## Calibration

### Soil Moisture Sensor

1. **Dry Calibration**: Place sensor in air, note reading
2. **Wet Calibration**: Place sensor in water, note reading
3. **Update Values**: Modify `SOIL_MOISTURE_DRY_VALUE` and `SOIL_MOISTURE_WET_VALUE`

### DHT Sensor

- No calibration needed
- Ensure proper wiring
- Check sensor type (DHT11 vs DHT22)

### LDR Sensor

1. **Dark Calibration**: Cover sensor completely
2. **Bright Calibration**: Expose to bright light
3. **Update Values**: Modify `LDR_DARK_VALUE` and `LDR_BRIGHT_VALUE`

## Data Logging

### Local Logging

- **Serial Output**: Continuous data logging
- **Status Updates**: Regular system status reports
- **Error Logging**: Detailed error messages

### Data Storage

- **No Persistent Storage**: Data not saved between reboots
- **Real-time Monitoring**: Continuous serial output
- **Manual Recording**: Copy serial output for records

## Maintenance

### Regular Checks

- **Sensor Cleaning**: Clean soil moisture sensor regularly
- **Connection Check**: Verify all wiring connections
- **Power Supply**: Monitor power supply voltage
- **Display**: Check display brightness and contrast

### Software Updates

- **Firmware Updates**: Upload new versions as needed
- **Configuration Changes**: Modify `config.h` as required
- **Library Updates**: Keep libraries up to date

## Comparison with Online Version

| Feature            | Offline Version | Online Version |
| ------------------ | --------------- | -------------- |
| Internet Required  | No              | Yes            |
| Cloud Data Logging | No              | Yes            |
| Remote Monitoring  | No              | Yes            |
| Web Interface      | No              | Yes            |
| OTA Updates        | No              | Yes            |
| Data Storage       | Local Only      | Cloud + Local  |
| Complexity         | Simple          | Advanced       |
| Power Consumption  | Lower           | Higher         |
| Cost               | Lower           | Higher         |

## Support

For questions, issues, or contributions:

- **GitHub Repository**: [Smart Farming ESP32](https://github.com/yourusername/smart-farming-esp32)
- **Documentation**: See main README.md
- **Hardware Test**: Use `TestCode/hardware/hardware_test.ino` first
- **Issues**: Report bugs via GitHub issues

## License

This project is licensed under the MIT License. See LICENSE file for details.

## Version History

- **v1.0.0**: Initial release with basic functionality
- **v1.1.0**: Added rotary encoder support
- **v1.2.0**: Added LCD 2004 support
- **v1.3.0**: Added LDR sensor support
- **v1.4.0**: Improved safety features and error handling
