# Library Setup Guide for ESP32 Smart Farming System

## Required Libraries

### 1. LiquidCrystal_I2C Library (ESP32 Compatible)

**Issue**: The default LiquidCrystal_I2C library may show a warning about AVR architecture compatibility.

**Solution**: Install an ESP32-compatible version:

#### Option A: Install ESP32-Compatible LiquidCrystal_I2C
1. Open Arduino IDE
2. Go to **Tools > Manage Libraries**
3. Search for "LiquidCrystal_I2C"
4. Install **"LiquidCrystal_I2C" by Frank de Brabander** (ESP32 compatible)
5. Or install **"ESP32_LiquidCrystal_I2C" by Blackhack**

#### Option B: Manual Installation
1. Download: https://github.com/johnrickman/LiquidCrystal_I2C
2. Extract to `Arduino/libraries/LiquidCrystal_I2C/`
3. Restart Arduino IDE

### 2. Other Required Libraries

Install these libraries through **Tools > Manage Libraries**:

```
- DHT sensor library (by Adafruit)
- Adafruit Unified Sensor
- ArduinoJson (by Benoit Blanchon)
- ESP32 WiFi library (built-in)
- ESP32 HTTPClient (built-in)
- ESP32 WebServer (built-in)
- ESP32 ArduinoOTA (built-in)
```

### 3. Adafruit IO Library (Online Version Only)
```
- Adafruit IO Arduino (by Adafruit)
- Adafruit MQTT Library
```

## ESP32 Board Setup

1. **Install ESP32 Board Package**:
   - File > Preferences
   - Additional Board Manager URLs: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Tools > Board > Board Manager
   - Search "ESP32" and install "esp32 by Espressif Systems"

2. **Board Selection**:
   - Tools > Board > ESP32 Arduino > ESP32 Dev Module

3. **Port Selection**:
   - Tools > Port > [Select your ESP32 COM port]

## Compilation Settings

**Upload Speed**: 921600
**CPU Frequency**: 240MHz (WiFi/BT)
**Flash Frequency**: 80MHz
**Flash Mode**: QIO
**Flash Size**: 4MB (32Mb)
**Partition Scheme**: Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)

## Troubleshooting

### Library Warnings
- **Warning about AVR architecture**: This is normal for some libraries. The code will still work on ESP32.
- **Multiple libraries found**: Arduino IDE will use the most appropriate one automatically.

### Compilation Errors
- **"Library not found"**: Install missing libraries through Library Manager
- **"Board not found"**: Install ESP32 board package
- **"Port not found"**: Check USB connection and drivers

### Memory Issues
- If you get memory warnings, disable unused features in `config.h`:
  ```cpp
  #define DHT_SENSOR_TYPE DHT_NONE     // Disable if not using DHT
  #define DISPLAY_TYPE DISPLAY_NONE    // Disable if not using LCD
  #define DEBUG_MODE false             // Disable for production
  ```

## Hardware Connections

### I2C LCD (1602 or 2004)
- **VCC** → 5V or 3.3V
- **GND** → Ground
- **SDA** → GPIO21
- **SCL** → GPIO22

### Potentiometer
- **VCC** → 3.3V
- **GND** → Ground
- **Wiper** → GPIO34

### Soil Moisture Sensor
- **VCC** → 3.3V
- **GND** → Ground
- **Signal** → GPIO36

### DHT22 Sensor
- **VCC** → 3.3V
- **GND** → Ground
- **Data** → GPIO5

### Relay Module
- **VCC** → 5V
- **GND** → Ground
- **Signal** → GPIO19

### Status LEDs
- **Green LED** → GPIO18 (System OK)
- **Red LED** → GPIO23 (Pump Active)
- **Blue LED** → GPIO2 (WiFi Connected - Online version)

## First Upload

1. **Select correct board and port**
2. **Install all required libraries**
3. **Configure `config.h` for your hardware**
4. **Compile and upload**
5. **Open Serial Monitor (115200 baud)**
6. **Check for initialization messages**

The system should now compile and run successfully on your ESP32!
