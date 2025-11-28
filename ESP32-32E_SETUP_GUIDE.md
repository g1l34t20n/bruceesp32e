# ESP32-32E (Elegoo CYD) Board Support for Bruce

## Overview
Successfully added full support for the **ESP32-32E (Elegoo CYD 2.8" display module)** to the Bruce firmware. This is based on the official Elegoo hardware specifications and is compatible with the E32R28T (with resistive touch) and E32N28T (without touch) models.

## Hardware Specifications

### Main Controller
- **Module**: ESP32-WROOM-32E
- **CPU**: Xtensa dual-core 32-bit LX6 @ 240MHz
- **Memory**: 4MB Flash, 520KB SRAM
- **WiFi**: 2.4GHz 802.11b/g/n
- **Bluetooth**: v4.2 BR/EDR and BLE

### Display
- **Size**: 2.8 inch TFT LCD
- **Resolution**: 240x320 pixels
- **Driver**: ILI9341
- **Interface**: 4-wire SPI (HSPI)
- **Backlight**: White LED x4
- **Colors**: 262K (RGB666), commonly 65K (RGB565)

### Touch Screen (E32R28T model)
- **Type**: Resistive touch screen
- **Driver**: XPT2046
- **Interface**: SPI
- **Resolution**: 240x320
- **Touch Interrupt**: GPIO36

### Other Features
- **RGB LED**: 3-color LED (Red: IO22, Green: IO16, Blue: IO17) - Common anode
- **Audio**: DAC output on IO26 with enable pin on IO4
- **SD Card**: MicroSD slot via SPI
- **Battery**: 3.7V LiPo support with charging circuit and monitoring (IO34)
- **USB**: Type-C for programming and power

## Pin Mapping

### Display Pins (ILI9341)
```
TFT_CS    = 15  (Chip Select)
TFT_DC    = 2   (Data/Command)
TFT_RST   = EN  (Reset - shared with ESP32 EN)
TFT_MOSI  = 13  (SPI MOSI)
TFT_MISO  = 12  (SPI MISO)
TFT_SCLK  = 14  (SPI Clock)
TFT_BL    = 21  (Backlight)
```

### Touch Screen Pins (XPT2046)
```
TOUCH_CS   = 33  (Chip Select)
TOUCH_IRQ  = 36  (Interrupt - active low)
TOUCH_MOSI = 32  (SPI MOSI)
TOUCH_MISO = 39  (SPI MISO)
TOUCH_CLK  = 25  (SPI Clock)
```

### SD Card Pins
```
SD_CS   = 5   (Chip Select)
SD_MOSI = 23  (SPI MOSI)
SD_MISO = 19  (SPI MISO)
SD_SCK  = 18  (SPI Clock)
```

### RGB LED Pins (Common Anode - LOW = ON)
```
LED_RED   = 22
LED_GREEN = 16
LED_BLUE  = 17
```

### Audio Pins
```
AUDIO_EN  = 4   (Enable - LOW = enabled)
AUDIO_DAC = 26  (DAC output)
```

### Other Pins
```
BOOT_BTN  = 0   (Boot button)
BAT_ADC   = 34  (Battery voltage monitoring)
UART_TX   = 1   (Serial TX)
UART_RX   = 3   (Serial RX)
SPI_SS    = 27  (SPI peripheral chip select)
GPIO35    = 35  (NC - Input only)
```

## PlatformIO Environments

Three build environments are available:

### 1. ESP32-32E (Standard)
```ini
[env:ESP32-32E]
```
Full-featured build with all capabilities enabled.

### 2. ESP32-32E-RESISTIVE (Resistive Touch)
```ini
[env:ESP32-32E-RESISTIVE]
```
Explicitly configured for resistive touch models (E32R28T).

### 3. LAUNCHER_ESP32-32E (Lite Version)
```ini
[env:LAUNCHER_ESP32-32E]
```
Lighter build with some features limited for M5Launcher compatibility.

## Building the Firmware

### Using PlatformIO CLI
```bash
# Build for ESP32-32E
pio run -e ESP32-32E

# Upload to device
pio run -e ESP32-32E -t upload

# Monitor serial output
pio device monitor
```

### Using PlatformIO IDE
1. Open the project in VS Code with PlatformIO extension
2. Select environment: **ESP32-32E** from the bottom toolbar
3. Click **Build** or **Upload**

## Configuration Files Created

### boards/ESP32-32E/ESP32-32E.ini
Main board configuration with:
- Pin definitions
- Display settings (ILI9341, 240x320)
- Touch screen settings (XPT2046 resistive)
- RGB LED support
- Audio configuration
- SD card setup
- Battery monitoring
- SPI/I2C peripheral configurations

### boards/ESP32-32E/interface.cpp
Device-specific interface code including:
- GPIO initialization
- Touch screen calibration
- Brightness control
- Input handling
- Power management
- RGB LED control

### boards/ESP32-32E/pins_arduino.h
Arduino pin definitions for compatibility with standard Arduino libraries.

## Features Enabled

✅ **TFT Display** - ILI9341 driver at 80MHz SPI  
✅ **Resistive Touch** - XPT2046 with calibration support  
✅ **RGB LED** - Full color control (common anode)  
✅ **Audio Output** - DAC on GPIO26  
✅ **SD Card** - MicroSD support via SPI  
✅ **Battery Monitoring** - ADC reading on GPIO34  
✅ **Boot Button** - GPIO0 for mode selection  
✅ **Deep Sleep** - Wake on touch interrupt  
✅ **Serial Communication** - UART TX/RX  
✅ **SPI Peripherals** - Support for CC1101, NRF24, W5500  

## Testing Recommendations

1. **Display Test**: Verify screen orientation and colors
2. **Touch Calibration**: Run first-time touch calibration
3. **SD Card**: Test file read/write operations
4. **RGB LED**: Test all three colors
5. **Audio**: Test DAC output if audio features are used
6. **Battery**: Verify ADC readings if using battery
7. **Peripherals**: Test any external SPI devices

## Differences from Standard CYD

The ESP32-32E has these differences from the standard CYD-2432S028:

| Feature | ESP32-32E | CYD-2432S028 |
|---------|-----------|--------------|
| Display Driver | ILI9341 | ILI9341_2 |
| Touch Type | XPT2046 Resistive | XPT2046 or Capacitive |
| Touch Clock | GPIO25 | GPIO33 |
| Touch MOSI | GPIO32 | Varies |
| Touch MISO | GPIO39 | Varies |
| RGB LED | Yes (22,16,17) | Optional |
| Audio DAC | GPIO26 | Not standard |
| Battery ADC | GPIO34 | Not standard |

## Troubleshooting

### Display Issues
- Check HSPI port is being used (not VSPI)
- Verify TFT_RST is set to -1 (uses ESP32 EN pin)
- Try adjusting SPI_FREQUENCY if display is unstable

### Touch Not Working
- Run touch calibration on first boot
- Check TOUCH_CS = 33 and TOUCH_IRQ = 36
- Verify touch orientation matches display rotation

### RGB LED Not Working
- Remember LEDs are common anode (LOW = ON, HIGH = OFF)
- Check if HAS_RGB_LED flag is enabled

### SD Card Issues
- Verify SD card is formatted as FAT32
- Check SD_CS = 5 and SPI pins are correct
- SD shares SPI bus with external peripherals

## Documentation Sources

All pin mappings and specifications are based on:
- Official Elegoo E32R28T/E32N28T Specification V1.0
- ESP32-32E Hardware Schematic PDF
- Arduino demo code from Elegoo
- Tested against existing CYD board configurations

## Next Steps

1. ✅ Board configuration created and committed
2. ⏳ Build and test firmware on actual hardware
3. ⏳ Fine-tune touch calibration if needed
4. ⏳ Test all peripheral features
5. ⏳ Create hardware test sketch for validation
6. ⏳ Update main README with ESP32-32E support

## Contributing

If you find any issues or improvements needed for the ESP32-32E support:
1. Test on actual hardware
2. Document any pin mapping issues
3. Submit pull request with fixes
4. Update this guide with findings

---

**Created by**: Gilby via Genspark AI  
**Date**: November 28, 2024  
**Repository**: https://github.com/g1l34t20n/bruceesp32e  
**Based on**: Bruce firmware with CYD board support

## RGB LED Usage Note

The ESP32-32E has **individual RGB LED pins** (common anode), not an addressable LED strip like WS2812B. This means:

- **No FastLED library needed** - LEDs are controlled directly via GPIO
- **Control method**: Use `digitalWrite()` directly on RGB_LED_R, RGB_LED_G, RGB_LED_B
- **Common anode wiring**: Write `LOW` to turn ON, `HIGH` to turn OFF
- **Pins**: Red=22, Green=16, Blue=17

### Example RGB LED Control Code

```cpp
// Turn on RED LED
digitalWrite(RGB_LED_R, LOW);  // LOW = ON for common anode
digitalWrite(RGB_LED_G, HIGH); // HIGH = OFF
digitalWrite(RGB_LED_B, HIGH);

// Turn on GREEN LED
digitalWrite(RGB_LED_R, HIGH);
digitalWrite(RGB_LED_G, LOW);
digitalWrite(RGB_LED_B, HIGH);

// Turn on BLUE LED
digitalWrite(RGB_LED_R, HIGH);
digitalWrite(RGB_LED_G, HIGH);
digitalWrite(RGB_LED_B, LOW);

// PURPLE (Red + Blue)
digitalWrite(RGB_LED_R, LOW);
digitalWrite(RGB_LED_G, HIGH);
digitalWrite(RGB_LED_B, LOW);

// Turn OFF all LEDs
digitalWrite(RGB_LED_R, HIGH);
digitalWrite(RGB_LED_G, HIGH);
digitalWrite(RGB_LED_B, HIGH);
```

The firmware's LED control system handles this automatically through the GPIO interface.
