# Smart Farming Test Code

This folder contains comprehensive testing and validation tools for the smart farming system, organized for Arduino IDE compatibility.

## Project Structure

```
TestCode/
├── hardware/                    # Hardware testing and validation
│   ├── hardware_test.ino       # Comprehensive hardware test suite
│   └── README.md               # Hardware test documentation
├── communication/              # Communication testing
│   ├── communication.ino      # Communication test suite
│   └── README.md              # Communication test documentation
└── README.md                  # This documentation
```

## Test Applications

### 1. Hardware Test (`hardware/hardware_test.ino`)

**Purpose**: Validates all hardware components before deploying the main system.

**Features**:
- Sensor validation (soil moisture, DHT, LDR)
- Actuator testing (relay, pump)
- Display testing (LCD)
- LED status testing
- Control input testing (encoder, potentiometer)
- Pin configuration validation
- Calibration assistance

**Usage**:
1. Open `hardware/hardware_test.ino` in Arduino IDE
2. Upload to ESP32
3. Follow serial monitor instructions
4. Verify all tests pass before using main code

### 2. Communication Test (`communication/communication.ino`)

**Purpose**: Tests all communication features for the online version.

**Features**:
- WiFi connectivity testing
- ThingSpeak data transmission
- Adafruit IO integration
- Web server functionality
- OTA update capability
- Real-time monitoring dashboard

**Usage**:
1. Configure WiFi and cloud credentials
2. Open `communication/communication.ino` in Arduino IDE
3. Upload to ESP32
4. Monitor test results via serial or web interface

## Testing Workflow

### Recommended Testing Sequence

1. **Hardware Test First**
   ```
   TestCode/hardware/hardware_test.ino
   ```
   - Validate all physical connections
   - Test sensor readings
   - Verify actuator operation
   - Calibrate sensors

2. **Communication Test** (Online version only)
   ```
   TestCode/communication/communication.ino
   ```
   - Test WiFi connectivity
   - Validate cloud services
   - Test web interface
   - Verify OTA functionality

3. **Main Application**
   ```
   MainCode/offline/offline.ino    # For offline operation
   MainCode/online/online.ino      # For online operation
   ```
   - Deploy main smart farming system
   - Monitor operation
   - Fine-tune configuration

## Arduino IDE Integration

### Opening Test Sketches

1. **Hardware Test**:
   - Open `TestCode/hardware/hardware_test.ino`
   - No additional configuration required
   - Self-contained test suite

2. **Communication Test**:
   - Open `TestCode/communication/communication.ino`
   - Configure WiFi and cloud credentials
   - Requires internet connectivity

### Test Configuration

Each test application has its own configuration:

- **Hardware Test**: Uses default pin assignments, no configuration needed
- **Communication Test**: Requires WiFi and cloud service credentials

## Test Results Interpretation

### Hardware Test Results

- **✓ PASS**: Component working correctly
- **✗ FAIL**: Component needs attention
- **⚠ WARNING**: Component working but needs calibration
- **ℹ INFO**: Additional information or suggestions

### Communication Test Results

- **Success Rate**: Percentage of communication methods working
- **Individual Status**: Pass/fail for each communication method
- **Error Details**: Specific error messages for troubleshooting
- **Performance Metrics**: Response times and signal strength

## Troubleshooting

### Common Hardware Test Issues

1. **Sensor Not Detected**
   - Check wiring connections
   - Verify power supply
   - Check pin assignments

2. **Display Not Working**
   - Verify I2C address
   - Check SDA/SCL connections
   - Test with I2C scanner

3. **Relay Not Switching**
   - Check relay module power
   - Verify control pin connection
   - Test relay manually

### Common Communication Test Issues

1. **WiFi Connection Failed**
   - Check SSID and password
   - Verify 2.4GHz network
   - Check signal strength

2. **Cloud Service Failed**
   - Verify API keys and credentials
   - Check internet connectivity
   - Ensure service is active

3. **Web Server Not Responding**
   - Check port 80 availability
   - Verify WiFi connection
   - Check firewall settings

## Best Practices

### Before Testing

1. **Power Supply**: Use stable 2A+ power supply
2. **Wiring**: Double-check all connections
3. **Environment**: Test in actual deployment environment
4. **Documentation**: Keep test results for reference

### During Testing

1. **Serial Monitor**: Keep open at 115200 baud
2. **Patience**: Allow tests to complete fully
3. **Notes**: Document any issues or observations
4. **Calibration**: Follow calibration procedures carefully

### After Testing

1. **Results**: Save test results and screenshots
2. **Issues**: Address any failures before proceeding
3. **Configuration**: Update main code configuration based on test results
4. **Documentation**: Update project documentation with findings

## Integration with Main Code

### Configuration Transfer

After successful testing, transfer validated settings to main code:

1. **Pin Assignments**: Use tested pin configurations
2. **Calibration Values**: Apply sensor calibration data
3. **Threshold Values**: Use validated irrigation thresholds
4. **Network Settings**: Transfer WiFi and cloud credentials

### Validation Checklist

Before deploying main code, ensure:

- [ ] All hardware tests pass
- [ ] Communication tests pass (online version)
- [ ] Sensor readings are accurate
- [ ] Actuators respond correctly
- [ ] Display shows correct information
- [ ] Network connectivity is stable
- [ ] Cloud services are working

## Support

For test-related questions or issues:

- **Hardware Issues**: Check `hardware/README.md`
- **Communication Issues**: Check `communication/README.md`
- **General Questions**: See main project README.md
- **GitHub Issues**: Report bugs via GitHub repository

## License

This test code is part of the smart farming project and is licensed under the MIT License.
