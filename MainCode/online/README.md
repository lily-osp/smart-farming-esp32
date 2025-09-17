# Smart Farming System - Online Version

A comprehensive IoT-based smart farming system with cloud connectivity, remote monitoring, and advanced features for modern agriculture.

## Overview

The online version extends the offline functionality with internet connectivity, cloud services, and remote management capabilities. It's perfect for:
- Commercial agriculture operations
- Remote monitoring and control
- Data analytics and insights
- Multi-location management
- Professional farming applications

## Features

### Core Functionality
- **Automated Irrigation**: Intelligent watering based on soil moisture levels
- **Sensor Monitoring**: Real-time temperature, humidity, and soil moisture readings
- **Safety Systems**: Watchdog timer, emergency stop, and pump protection
- **Modular Design**: Flexible hardware configurations
- **Local Control**: Rotary encoder or potentiometer for parameter adjustment

### Online Features
- **WiFi Connectivity**: Automatic connection and reconnection
- **Cloud Integration**: ThingSpeak and Adafruit IO data logging
- **Web Interface**: Remote monitoring and control dashboard
- **OTA Updates**: Over-the-air firmware updates
- **JSON API**: RESTful API for integration with other systems
- **Data Analytics**: Historical data analysis and trends
- **Remote Alerts**: Email and notification systems

### Display Options
- **LCD 1602**: 16x2 character display with cycling screens
- **LCD 2004**: 20x4 character display with all information
- **Headless Mode**: Serial output only, no display

### Control Options
- **Rotary Encoder**: Interactive menu system for parameter adjustment
- **Potentiometer**: Real-time threshold adjustment
- **Web Interface**: Remote control via browser
- **API Control**: Programmatic control via REST API

## Hardware Requirements

### Essential Components
- ESP32 Development Board
- Soil Moisture Sensor (Analog)
- Relay Module (5V or 3.3V)
- Power Supply (2A+ recommended)
- WiFi Network Access

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
├── GPIO 2 ───────────── Blue LED (WiFi status) - Optional
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
   - ArduinoJson (by Benoit Blanchon) v6.19.4+
   - Adafruit IO Arduino (by Adafruit) v5.0.0+

4. **Open Sketch**: Open `online.ino` in Arduino IDE

### 3. Configuration

Edit `config.h` to match your hardware setup and network configuration:

```cpp
// Display Configuration
#define DISPLAY_TYPE DISPLAY_LCD_1602     // or DISPLAY_LCD_2004, DISPLAY_NONE

// Control Configuration  
#define CONTROL_TYPE CONTROL_ROTARY_ENCODER  // or CONTROL_POTENTIOMETER, CONTROL_NONE

// Sensor Configuration
#define DHT_SENSOR_TYPE DHT_TYPE_22       // or DHT_TYPE_11, DHT_NONE
#define LDR_SENSOR_TYPE LDR_NONE          // or LDR_TYPE_ENABLED

// WiFi Configuration
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// ThingSpeak Configuration
#define THINGSPEAK_ENABLED true
#define THINGSPEAK_API_KEY "YOUR_THINGSPEAK_API_KEY"
#define THINGSPEAK_CHANNEL_ID "YOUR_CHANNEL_ID"

// Adafruit IO Configuration
#define ADAFRUIT_IO_ENABLED true
#define ADAFRUIT_IO_USERNAME "YOUR_ADAFRUIT_IO_USERNAME"
#define ADAFRUIT_IO_KEY "YOUR_ADAFRUIT_IO_KEY"

// Soil Moisture Threshold
#define SOIL_MOISTURE_THRESHOLD 30        // Percentage (0-100%)

// Irrigation Settings
#define IRRIGATION_DURATION 5000          // Duration in milliseconds
#define IRRIGATION_COOLDOWN 300000        // Cooldown period (5 minutes)
```

### 4. Cloud Services Setup

#### ThingSpeak Setup
1. Create account at [ThingSpeak.com](https://thingspeak.com)
2. Create a new channel
3. Note your Channel ID and API Key
4. Configure fields for your data

#### Adafruit IO Setup
1. Create account at [Adafruit IO](https://io.adafruit.com)
2. Create feeds for your data
3. Note your Username and API Key
4. Configure feed names

### 5. Upload and Run

1. **Select Board**: ESP32 Dev Module
2. **Select Port**: Your ESP32 COM port
3. **Upload**: Upload the sketch
4. **Monitor**: Open Serial Monitor (115200 baud)
5. **Connect**: Wait for WiFi connection
6. **Access**: Open web interface at ESP32's IP address

## Configuration Examples

### Minimal Setup (Soil Moisture + WiFi)
```cpp
#define DHT_SENSOR_TYPE DHT_NONE
#define DISPLAY_TYPE DISPLAY_NONE
#define CONTROL_TYPE CONTROL_NONE
#define LDR_SENSOR_TYPE LDR_NONE
#define THINGSPEAK_ENABLED true
#define ADAFRUIT_IO_ENABLED false
```

### Basic Setup (DHT11 + LCD 1602 + ThingSpeak)
```cpp
#define DHT_SENSOR_TYPE DHT_TYPE_11
#define DISPLAY_TYPE DISPLAY_LCD_1602
#define CONTROL_TYPE CONTROL_NONE
#define LDR_SENSOR_TYPE LDR_NONE
#define THINGSPEAK_ENABLED true
#define ADAFRUIT_IO_ENABLED false
```

### Professional Setup (DHT22 + LCD 2004 + Both Cloud Services)
```cpp
#define DHT_SENSOR_TYPE DHT_TYPE_22
#define DISPLAY_TYPE DISPLAY_LCD_2004
#define CONTROL_TYPE CONTROL_ROTARY_ENCODER
#define LDR_SENSOR_TYPE LDR_TYPE_ENABLED
#define THINGSPEAK_ENABLED true
#define ADAFRUIT_IO_ENABLED true
```

## Usage

### Initial Setup

1. **Power On**: Connect power supply to ESP32
2. **WiFi Connection**: Wait for WiFi connection (Blue LED indicates status)
3. **Serial Monitor**: Open Arduino IDE Serial Monitor (115200 baud)
4. **System Check**: Verify all sensors are detected
5. **Web Interface**: Access dashboard at ESP32's IP address

### Daily Operation

1. **Local Monitoring**: Check LCD display or serial output
2. **Remote Monitoring**: Access web interface from anywhere
3. **Cloud Dashboards**: Monitor data on ThingSpeak/Adafruit IO
4. **Status LEDs**:
   - Green: System OK
   - Red: Pump active or error
   - Blue: WiFi connected
5. **Automatic Irrigation**: System waters when soil moisture < threshold

### Web Interface

Access the web interface at `http://[ESP32_IP]`:

#### Dashboard Features
- **Real-time Data**: Live sensor readings
- **System Status**: Current system state
- **Control Panel**: Manual irrigation control
- **Settings**: Parameter adjustment
- **History**: Recent irrigation events
- **Network Info**: WiFi status and signal strength

#### API Endpoints
- `GET /api` - JSON data endpoint
- `GET /status` - System status
- `POST /control` - Manual control
- `GET /config` - Configuration data

### Cloud Services

#### ThingSpeak Integration
- **Data Logging**: Automatic data transmission
- **Charts**: Real-time and historical charts
- **Alerts**: Email notifications
- **API Access**: RESTful API for data retrieval

#### Adafruit IO Integration
- **Feed Management**: Organized data feeds
- **Dashboards**: Custom visualization
- **Triggers**: Automated actions
- **Mobile App**: Adafruit IO mobile app

## Display Information

### LCD 1602 (Cycling Screens)
- **Screen 1**: Soil moisture percentage and status
- **Screen 2**: Temperature and humidity (if DHT enabled)
- **Screen 3**: WiFi status and system info

### LCD 2004 (All Information)
- **Line 1**: Soil moisture percentage and status
- **Line 2**: Temperature and humidity (if DHT enabled)
- **Line 3**: Light level (if LDR enabled)
- **Line 4**: WiFi status and system info

### Serial Output
```
Smart Farming System - Online Version
====================================
WiFi Connected: 192.168.1.100
Soil Moisture: 45% (Threshold: 30%)
Temperature: 25.5°C
Humidity: 60.0%
Light Level: 75%
System Status: OK
Daily Irrigations: 3
Last Irrigation: 2 hours ago
ThingSpeak: Connected
Adafruit IO: Connected
```

## Safety Features

### Watchdog Timer
- **Timeout**: 30 seconds
- **Function**: Prevents system hangs
- **Recovery**: Automatic system restart

### Emergency Stop
- **Manual Override**: Immediate pump shutdown
- **Web Interface**: Remote emergency stop
- **Safety**: Prevents overwatering
- **Recovery**: Manual reset required

### Pump Protection
- **Maximum Runtime**: 5 minutes continuous
- **Cooldown Period**: 5 minutes between irrigations
- **Daily Limit**: Maximum 10 irrigations per day

### Network Safety
- **WiFi Reconnection**: Automatic reconnection on disconnect
- **Data Validation**: Validates cloud service responses
- **Error Handling**: Graceful handling of network errors

## Troubleshooting

### Common Issues

1. **WiFi Not Connecting**
   - Check SSID and password
   - Verify 2.4GHz network (not 5GHz)
   - Check signal strength
   - Ensure network allows ESP32 devices

2. **Cloud Services Not Working**
   - Verify API keys and credentials
   - Check internet connectivity
   - Ensure cloud services are active
   - Check firewall settings

3. **Web Interface Not Accessible**
   - Verify ESP32 IP address
   - Check port 80 availability
   - Ensure same network as ESP32
   - Try different browser

4. **OTA Updates Not Working**
   - Check WiFi connection
   - Verify OTA password
   - Ensure sufficient memory
   - Check Arduino IDE OTA settings

5. **Data Not Transmitting**
   - Check cloud service status
   - Verify API quotas
   - Check network connectivity
   - Review error messages in serial monitor

### Debug Mode

Enable debug mode in `config.h` for detailed output:

```cpp
#define DEBUG_MODE true
```

### Network Diagnostics

Use the web interface diagnostics page to check:
- WiFi signal strength
- Internet connectivity
- Cloud service status
- Data transmission logs

## Performance Optimization

### Power Management
- Use headless mode for lower power consumption
- Increase sensor reading intervals
- Enable WiFi power save mode
- Optimize cloud transmission intervals

### Memory Optimization
- Disable unused features
- Reduce debug output
- Use latest library versions
- Optimize JSON payload sizes

### Network Optimization
- Use stable WiFi connection
- Optimize data transmission intervals
- Use efficient data formats
- Implement data compression

## Data Management

### Local Data Logging
- **Serial Output**: Continuous data logging
- **Status Updates**: Regular system status reports
- **Error Logging**: Detailed error messages

### Cloud Data Storage
- **ThingSpeak**: Historical data with charts
- **Adafruit IO**: Organized feed data
- **Backup**: Local data backup
- **Analytics**: Data analysis and trends

### Data Export
- **CSV Export**: Download data as CSV
- **JSON API**: Programmatic data access
- **Real-time**: Live data streaming
- **Historical**: Time-series data access

## Security Considerations

### Network Security
- **WiFi Encryption**: Use WPA2/WPA3
- **Strong Passwords**: Use complex passwords
- **Network Isolation**: Consider separate IoT network
- **Firewall**: Configure router firewall

### API Security
- **Authentication**: Implement API authentication
- **Rate Limiting**: Prevent abuse
- **Data Validation**: Validate all inputs
- **HTTPS**: Use secure connections where possible

## OTA Updates

### Over-the-Air Updates
- **Remote Updates**: Update firmware without physical access
- **Version Control**: Track firmware versions
- **Rollback**: Ability to revert to previous version
- **Scheduled Updates**: Plan maintenance windows

### Update Process
1. **Prepare Firmware**: Compile new version
2. **Upload to ESP32**: Use Arduino IDE OTA
3. **Verify Update**: Check version and functionality
4. **Monitor**: Watch for issues

## Integration

### Third-Party Integration
- **Home Assistant**: Smart home integration
- **IFTTT**: Automation triggers
- **Zapier**: Workflow automation
- **Custom APIs**: RESTful API access

### Data Export
- **Webhooks**: Real-time data notifications
- **MQTT**: Message queuing
- **Database**: Direct database integration
- **Analytics**: Business intelligence tools

## Comparison with Offline Version

| Feature | Offline Version | Online Version |
|---------|----------------|----------------|
| Internet Required | No | Yes |
| Cloud Data Logging | No | Yes |
| Remote Monitoring | No | Yes |
| Web Interface | No | Yes |
| OTA Updates | No | Yes |
| Data Storage | Local Only | Cloud + Local |
| Complexity | Simple | Advanced |
| Power Consumption | Lower | Higher |
| Cost | Lower | Higher |
| Maintenance | Manual | Remote |

## Support

For questions, issues, or contributions:
- **GitHub Repository**: [Smart Farming ESP32](https://github.com/yourusername/smart-farming-esp32)
- **Documentation**: See main README.md
- **Hardware Test**: Use `TestCode/hardware/hardware_test.ino` first
- **Communication Test**: Use `TestCode/communication/communication.ino` for network testing
- **Issues**: Report bugs via GitHub issues

## License

This project is licensed under the MIT License. See LICENSE file for details.

## Version History

- **v1.0.0**: Initial release with basic functionality
- **v1.1.0**: Added WiFi connectivity
- **v1.2.0**: Added ThingSpeak integration
- **v1.3.0**: Added Adafruit IO integration
- **v1.4.0**: Added web interface
- **v1.5.0**: Added OTA updates
- **v1.6.0**: Added JSON API
- **v1.7.0**: Improved security and performance
