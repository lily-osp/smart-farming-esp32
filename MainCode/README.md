# Main Smart Farming Code

This folder contains the main smart farming system code with all features and functionality.

## Files

- `smart_farming_offline.ino` - Standalone version without internet connectivity
- `smart_farming_online.ino` - IoT version with WiFi and cloud connectivity
- `config.h` - Centralized configuration file

## Prerequisites

Before using the main code:

1. **Run Hardware Tests**: Use the test code from the `TestCode` folder first
2. **Install Libraries**: Install all required libraries (see `libraries.txt`)
3. **Configure Hardware**: Update `config.h` with your hardware setup
4. **Verify Connections**: Ensure all hardware tests pass

## Quick Start

### 1. Choose Your Version

**Offline Version** (`smart_farming_offline.ino`):
- Local operation only
- No internet required
- Serial output for monitoring
- LED status indicators
- Automatic irrigation control

**Online Version** (`smart_farming_online.ino`):
- WiFi connectivity
- Cloud data logging (ThingSpeak, Adafruit IO)
- Web interface for remote monitoring
- OTA firmware updates
- JSON API for integration

### 2. Configure System

Edit `config.h` to match your hardware:

```cpp
// Display Configuration
#define DISPLAY_TYPE DISPLAY_LCD_1602     // or DISPLAY_LCD_2004, DISPLAY_NONE

// Control Configuration  
#define CONTROL_TYPE CONTROL_ROTARY_ENCODER  // or CONTROL_POTENTIOMETER, CONTROL_NONE

// Sensor Configuration
#define DHT_SENSOR_TYPE DHT22             // or DHT11, DHT_NONE

// WiFi Configuration (Online version only)
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Cloud Services (Online version only)
#define THINGSPEAK_ENABLED true
#define THINGSPEAK_API_KEY "YOUR_API_KEY"
#define ADAFRUIT_IO_ENABLED true
#define ADAFRUIT_IO_USERNAME "YOUR_USERNAME"
#define ADAFRUIT_IO_KEY "YOUR_KEY"
```

### 3. Upload Code

1. **Select Board**: ESP32 Dev Module
2. **Select Port**: Your ESP32 COM port
3. **Upload**: Upload the chosen version
4. **Monitor**: Open Serial Monitor (115200 baud)

## Configuration Examples

### Minimal Setup (Soil Moisture Only)
```cpp
#define DHT_SENSOR_TYPE DHT_NONE
#define DISPLAY_TYPE DISPLAY_NONE
#define CONTROL_TYPE CONTROL_NONE
```

### Basic Setup (DHT11 + LCD 1602)
```cpp
#define DHT_SENSOR_TYPE DHT11
#define DISPLAY_TYPE DISPLAY_LCD_1602
#define CONTROL_TYPE CONTROL_NONE
```

### Professional Setup (DHT22 + LCD 2004 + Rotary Encoder)
```cpp
#define DHT_SENSOR_TYPE DHT22
#define DISPLAY_TYPE DISPLAY_LCD_2004
#define CONTROL_TYPE CONTROL_ROTARY_ENCODER
```

## Features

### Core Features
- **Automated Irrigation**: Intelligent watering based on soil moisture
- **Sensor Monitoring**: Temperature, humidity, soil moisture
- **Safety Mechanisms**: Watchdog timer, emergency stop, pump protection
- **Modular Design**: Flexible hardware configurations
- **Fail-safe Operation**: Automatic recovery and error handling

### Online Features (Online Version Only)
- **WiFi Connectivity**: Automatic connection and reconnection
- **Cloud Integration**: ThingSpeak and Adafruit IO
- **Web Interface**: Remote monitoring and control
- **OTA Updates**: Over-the-air firmware updates
- **JSON API**: RESTful API for integration

### Display Options
- **LCD 1602**: 16x2 character display with cycling screens
- **LCD 2004**: 20x4 character display with all information
- **Headless**: Serial output only, no display

### Control Options
- **Rotary Encoder**: Interactive menu system
- **Potentiometer**: Real-time threshold adjustment
- **Automated**: Fully automated operation

## Usage

### Offline Mode
1. **Power On**: Connect power supply
2. **Monitor**: Check serial output or LCD display
3. **Status LEDs**: Green (OK), Red (Pump), Blue (Not used)
4. **Automatic Operation**: System waters when soil moisture < threshold

### Online Mode
1. **WiFi Setup**: Configure WiFi credentials in `config.h`
2. **Cloud Setup**: Configure ThingSpeak and/or Adafruit IO
3. **Web Interface**: Access via `http://[ESP32_IP]`
4. **Remote Monitoring**: Use cloud dashboards
5. **OTA Updates**: Update firmware remotely

## Safety Features

- **Watchdog Timer**: Prevents system hangs (30-second timeout)
- **Emergency Stop**: Manual emergency shutdown
- **Pump Protection**: Maximum 5-minute runtime protection
- **Sensor Validation**: Range and consistency checking
- **Automatic Recovery**: Self-healing after errors

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

3. **WiFi Not Connecting** (Online version)
   - Check WiFi credentials
   - Verify signal strength
   - Ensure 2.4GHz network

4. **Cloud Data Not Transmitting** (Online version)
   - Check API keys and credentials
   - Verify internet connectivity
   - Check cloud service status

### Debug Mode

Enable debug mode in `config.h` for detailed output:

```cpp
#define DEBUG_MODE true
```

## Performance Optimization

### Power Management
- Use headless mode for lower power consumption
- Increase sensor reading intervals
- Enable WiFi power save mode

### Memory Optimization
- Disable unused features
- Reduce debug output
- Use latest library versions

## Development

### Adding New Features
1. **Modify Configuration**: Update `config.h`
2. **Update Code**: Modify main code files
3. **Test Thoroughly**: Test on actual hardware
4. **Document Changes**: Update documentation

### Code Structure
- **Configuration**: Centralized in `config.h`
- **Functions**: Organized by functionality
- **Libraries**: Separated by purpose
- **Documentation**: Comprehensive code comments

## Support

For questions, issues, or contributions:
- **GitHub Repository**: [https://github.com/lily-osp/smart-farming-esp32](https://github.com/lily-osp/smart-farming-esp32)
- **Documentation**: See main README.md
- **Hardware Test**: Use TestCode folder first

## License

This project is licensed under the MIT License. See LICENSE file for details.
