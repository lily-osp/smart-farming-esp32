# Smart Farming Communication Test

This test suite validates all communication features of the smart farming system.

## Features Tested

### 1. WiFi Connectivity
- Tests WiFi connection establishment
- Monitors signal strength (RSSI)
- Handles reconnection on disconnection
- Reports connection status and IP address

### 2. ThingSpeak Integration
- Tests data transmission to ThingSpeak channels
- Validates API key and channel configuration
- Sends sensor data (temperature, humidity, soil moisture, light level, pump status, irrigation count)
- Reports transmission success/failure

### 3. Adafruit IO Integration
- Tests data transmission to Adafruit IO feeds
- Validates username and API key
- Sends sensor data in JSON format
- Reports transmission success/failure

### 4. Web Server Functionality
- Tests local web server on port 80
- Provides web interface for monitoring
- Offers API endpoints for data access
- Tests server responsiveness

### 5. OTA (Over-The-Air) Updates
- Tests OTA update capability
- Validates hostname and password configuration
- Monitors OTA service status
- Reports OTA readiness

## Configuration

Before running the test, update the configuration section in `communication.ino`:

```cpp
// WiFi Configuration
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// ThingSpeak Configuration
#define THINGSPEAK_API_KEY "YOUR_THINGSPEAK_API_KEY"
#define THINGSPEAK_CHANNEL_ID "YOUR_CHANNEL_ID"

// Adafruit IO Configuration
#define ADAFRUIT_IO_USERNAME "YOUR_ADAFRUIT_IO_USERNAME"
#define ADAFRUIT_IO_KEY "YOUR_ADAFRUIT_IO_KEY"
```

## Usage

1. **Upload the sketch** to your ESP32 board
2. **Open Serial Monitor** at 115200 baud
3. **Monitor the test results** in real-time
4. **Access web interface** at the ESP32's IP address
5. **Check API endpoints**:
   - `http://[ESP32_IP]/` - Main dashboard
   - `http://[ESP32_IP]/api` - JSON data
   - `http://[ESP32_IP]/status` - System status
   - `http://[ESP32_IP]/test` - Test endpoint

## Test Output

The test provides detailed output including:

- **Connection Status**: WiFi, ThingSpeak, Adafruit IO, Web Server, OTA
- **Signal Strength**: WiFi RSSI in dBm
- **IP Address**: ESP32's assigned IP address
- **Test Results**: Pass/fail status for each communication method
- **Success Rate**: Overall communication success percentage
- **Error Messages**: Detailed error information for failed tests

## Test Intervals

- **Test Frequency**: Every 5 seconds
- **WiFi Timeout**: 10 seconds
- **HTTP Timeout**: 5 seconds
- **Watchdog Timeout**: 30 seconds

## Troubleshooting

### Common Issues

1. **WiFi Connection Failed**
   - Check SSID and password
   - Verify network availability
   - Check signal strength

2. **ThingSpeak Transmission Failed**
   - Verify API key and channel ID
   - Check internet connectivity
   - Ensure channel is public or properly configured

3. **Adafruit IO Transmission Failed**
   - Verify username and API key
   - Check feed names and permissions
   - Ensure account is active

4. **Web Server Not Responding**
   - Check if port 80 is available
   - Verify WiFi connection
   - Check firewall settings

5. **OTA Updates Not Working**
   - Verify hostname and password
   - Check WiFi connection
   - Ensure OTA is properly initialized

## Expected Results

A successful test should show:
- ✓ WiFi Connected
- ✓ ThingSpeak data sent successfully
- ✓ Adafruit IO data sent successfully
- ✓ Web Server responding
- ✓ OTA Updates ready
- Success Rate: 5/5 (100%)

## Hardware Requirements

- ESP32 Development Board
- WiFi network access
- Internet connectivity
- USB cable for programming and monitoring

## Dependencies

Required libraries:
- WiFi (ESP32 core)
- HTTPClient (ESP32 core)
- ArduinoJson
- WebServer (ESP32 core)
- ArduinoOTA (ESP32 core)

## Notes

- The test simulates sensor data for demonstration purposes
- Real sensor data can be integrated by modifying the `updateTestData()` function
- Test results are logged to Serial Monitor and web interface
- The test runs continuously and provides real-time monitoring
- All communication methods are tested independently
- Failed tests are automatically retried on subsequent test cycles
