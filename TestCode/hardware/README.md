# Hardware Test Code

This folder contains test code to verify all hardware connections and components before running the main smart farming system.

## Files

- `hardware_test.ino` - Comprehensive hardware test suite

## Purpose

The hardware test code performs the following tests:

1. **Serial Communication Test** - Verifies serial communication is working
2. **LED Status Test** - Tests all status LEDs (Green, Red, Blue)
3. **Soil Moisture Sensor Test** - Verifies analog sensor readings
4. **DHT Sensor Test** - Tests temperature/humidity sensor (if enabled)
5. **LCD Display Test** - Tests I2C LCD display (if enabled)
6. **Relay Module Test** - Tests pump control relay
7. **Control System Test** - Tests rotary encoder/potentiometer (if enabled)
8. **Emergency Stop Test** - Tests emergency stop button (if enabled)

## Configuration

Before running the test, modify the configuration section in `hardware_test.ino`:

```cpp
// Test Configuration
#define TEST_DELAY 2000           // Delay between tests (ms)
#define SERIAL_BAUD_RATE 115200   // Serial communication speed

// Hardware Configuration
#define SOIL_MOISTURE_PIN 36      // ADC1_CH0
#define RELAY_PIN 19              // Relay control pin
#define LED_GREEN_PIN 18          // System status LED
#define LED_RED_PIN 23            // Pump active LED
#define LED_BLUE_PIN 2            // WiFi status LED
#define EMERGENCY_STOP_PIN 0      // Emergency stop button (optional)

// DHT Sensor Configuration (set to 0 to disable)
#define DHT_ENABLED 1             // 1 = enabled, 0 = disabled
#define DHT_PIN 5                 // DHT data pin
#define DHT_TYPE DHT22            // DHT11 or DHT22

// LCD Configuration (set to 0 to disable)
#define LCD_ENABLED 1             // 1 = enabled, 0 = disabled
#define LCD_ADDRESS 0x27          // I2C address (0x27 or 0x3F)
#define LCD_COLS 16               // 16 for 1602, 20 for 2004
#define LCD_ROWS 2                // 2 for 1602, 4 for 2004

// Control System Configuration (set to 0 to disable)
#define CONTROL_ENABLED 1         // 1 = enabled, 0 = disabled
#define ENCODER_CLK_PIN 16        // Rotary encoder clock pin
#define ENCODER_DT_PIN 17         // Rotary encoder data pin
#define ENCODER_SW_PIN 4          // Rotary encoder switch pin
#define POTENTIOMETER_PIN 34      // Potentiometer center pin
```

## Usage

1. **Configure Hardware**: Update the configuration section to match your hardware setup
2. **Upload Code**: Upload `hardware_test.ino` to your ESP32
3. **Open Serial Monitor**: Set baud rate to 115200
4. **Run Tests**: The test sequence will run automatically
5. **Check Results**: Review the test results in the serial monitor
6. **Fix Issues**: Address any failed tests before running the main system

## Test Results

The test will display results in the following format:

```
========================================
HARDWARE TEST RESULTS
========================================
Serial Communication: ✓ PASS
LED Status Indicators: ✓ PASS
Soil Moisture Sensor: ✓ PASS
DHT Sensor: ✓ PASS
LCD Display: ✓ PASS
Relay Module: ✓ PASS
Control System: ✓ PASS
Emergency Stop: ✓ PASS
========================================
🎉 ALL TESTS PASSED! Hardware is ready.
========================================
```

## Continuous Monitoring

After the initial tests, the system enters continuous monitoring mode, displaying:

- Soil moisture readings
- Temperature and humidity (if DHT enabled)
- Potentiometer values (if control enabled)
- Emergency stop status (if enabled)
- System memory usage
- Uptime

## Troubleshooting

### Common Issues

1. **DHT Sensor Errors**
   - Check wiring (VCC, GND, Data)
   - Verify pull-up resistor (10kΩ)
   - Check DHT_TYPE setting

2. **LCD Not Displaying**
   - Check I2C connections (SDA/SCL)
   - Verify I2C address (0x27 or 0x3F)
   - Check power supply (5V)

3. **Soil Moisture Sensor Issues**
   - Check analog pin connection
   - Verify power supply (3.3V)
   - Test in air (dry) and water (wet)

4. **Relay Not Working**
   - Check relay control pin
   - Verify relay power supply (5V)
   - Test with multimeter

5. **Control System Issues**
   - Check encoder/potentiometer connections
   - Verify pull-up resistors
   - Test with multimeter

## Next Steps

After all tests pass:

1. **Upload Main Code**: Use the code from the `MainCode` folder
2. **Configure System**: Update `config.h` with your settings
3. **Run System**: Start the smart farming system

## Safety Notes

- **Power Supply**: Ensure adequate power supply (minimum 2A)
- **Wiring**: Double-check all connections before powering on
- **Relay**: Be careful with high-voltage connections
- **Water**: Keep electronics away from water sources
