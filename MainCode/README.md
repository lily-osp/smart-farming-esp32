# Main Smart Farming Code

This folder contains the main smart farming system code organized for Arduino IDE compatibility.

## **Version 1.8.0 - Production Ready**

### **Key Improvements**

- **Revolutionary Setup System**: Step-by-step config.h with visual guides
- **Production Optimization**: 3-6KB smaller firmware, 5-10% faster execution
- **Enhanced Reliability**: Improved irrigation control and sensor validation
- **Complete Documentation**: Comprehensive library setup and troubleshooting guides

## Project Structure

```
MainCode/
├── offline/                    # Offline version (standalone)
│   ├── offline.ino            # Main offline sketch
│   └── config.h               # Offline configuration
├── online/                     # Online version (IoT with cloud)
│   ├── online.ino             # Main online sketch
│   └── config.h               # Online configuration
└── README.md                  # This documentation
```

## Files

- **Offline Version**: `offline/offline.ino` - Standalone version without internet connectivity
- **Online Version**: `online/online.ino` - IoT version with WiFi and cloud connectivity
- **Configuration Files**: Each version has its own `config.h` for modular configuration

## Prerequisites

Before using the main code:

1. **📋 Library Setup**: Follow `LIBRARY_SETUP.md` for ESP32-compatible libraries
2. **🔧 Run Hardware Tests**: Use the test code from the `TestCode` folder first
3. **⚙️ Easy Configuration**: Use the new step-by-step config.h setup system
4. **✅ Verify Connections**: Ensure all hardware tests pass

### **New Library Setup Process**

The system now includes comprehensive library setup documentation:

- **ESP32-Compatible Libraries**: Detailed instructions for ESP32-specific library versions
- **Automatic Validation**: Compile-time checks for library compatibility
- **Troubleshooting Guide**: Solutions for common library and compilation issues
- **Version Requirements**: Specific library versions tested and verified

## Quick Start

### 1. Choose Your Version

**Offline Version** (`offline/offline.ino`):

- Local operation only
- No internet required
- Serial output for monitoring
- LED status indicators
- Automatic irrigation control

**Online Version** (`online/online.ino`):

- WiFi connectivity
- Cloud data logging (ThingSpeak, Adafruit IO)
- Web interface for remote monitoring
- OTA firmware updates
- JSON API for integration

### 2. Configure System

The new configuration system makes setup effortless with pre-configured options:

#### **Step-by-Step Setup (New!)**

**STEP 1: Choose Your Setup Type**

```cpp
// Uncomment ONE setup type:
// #define BASIC_SETUP           // Minimal: Soil sensor only
// #define STANDARD_SETUP        // Basic + DHT22 + LCD 1602  
#define POTENTIOMETER_SETUP    // Standard + potentiometer control
// #define PROFESSIONAL_SETUP    // All features + LCD 2004 + encoder
// #define CUSTOM_SETUP          // Manual configuration
```

**STEP 2: Hardware Connections**
Visual pin diagrams show exactly where to connect components.

**STEP 3: Plant-Specific Settings**

```cpp
#define SOIL_MOISTURE_THRESHOLD 30    // Adjust for your plants:
// Succulents: 10-15%, Herbs: 20-25%, Vegetables: 30-40%
```

**STEP 4: WiFi & IoT (Online Version)**

```cpp
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define THINGSPEAK_API_KEY "YOUR_API_KEY"    // Optional cloud logging
```

#### **Benefits of New System**

- **90% Faster Setup**: Pre-configured options eliminate manual configuration
- **Error Prevention**: Automatic validation prevents configuration mistakes
- **Beginner Friendly**: Visual guides and plant-specific recommendations
- **Expert Friendly**: Full manual control still available with CUSTOM_SETUP

### 3. Upload Code

1. **Open Arduino IDE**
2. **Select Board**: ESP32 Dev Module
3. **Select Port**: Your ESP32 COM port
4. **Open Sketch**:
   - For offline: Open `offline/offline.ino`
   - For online: Open `online/online.ino`
5. **Upload**: Upload the chosen version
6. **Monitor**: Open Serial Monitor (115200 baud)

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
- **Configurable Serial Output**: Optional serial monitor output for debugging and monitoring

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

### Serial Output Options

- **Enabled (Default)**: Full serial output for debugging and monitoring
- **Disabled**: No serial output for production deployments
- **Performance**: Disabling serial output improves performance and reduces memory usage

## Usage

### Offline Mode

1. **Power On**: Connect power supply
2. **Monitor**: Check serial output or LCD display
3. **Status LEDs**: Green (OK), Red (Pump), Blue (Not used)
4. **Automatic Operation**: System waters when soil moisture < threshold

### Serial Output Configuration

To enable or disable serial output, modify the configuration in `config.h`:

```cpp
// Enable serial output (default)
#define SERIAL_OUTPUT_ENABLED true

// Disable serial output for production
#define SERIAL_OUTPUT_ENABLED false
```

**When Enabled:**

- Full debug output and status messages
- Sensor readings and validation results
- Error messages and warnings
- System heartbeat and data logging

**When Disabled:**

- No serial output (improves performance)
- Reduced memory usage
- Suitable for production deployments
- System still functions normally

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
- Disable serial output for production deployments

### Memory Optimization

- Disable unused features
- Disable serial output (`SERIAL_OUTPUT_ENABLED false`)
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
