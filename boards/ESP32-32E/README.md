# ESP32-32E Board Support

## Quick Reference

### Board Models
- **E32R28T**: With resistive touchscreen
- **E32N28T**: Without touchscreen

### Pin Quick Reference Table

| Function | GPIO | Notes |
|----------|------|-------|
| **Display** | | |
| TFT CS | 15 | Chip Select |
| TFT DC | 2 | Data/Command |
| TFT RST | EN | Uses ESP32 EN |
| TFT MOSI | 13 | HSPI |
| TFT MISO | 12 | HSPI |
| TFT SCK | 14 | HSPI |
| TFT BL | 21 | Backlight PWM |
| **Touch** | | |
| Touch CS | 33 | XPT2046 |
| Touch IRQ | 36 | Interrupt |
| Touch MOSI | 32 | |
| Touch MISO | 39 | |
| Touch SCK | 25 | |
| **SD Card** | | |
| SD CS | 5 | |
| SD MOSI | 23 | Shared SPI |
| SD MISO | 19 | Shared SPI |
| SD SCK | 18 | Shared SPI |
| **RGB LED** | | Common Anode |
| Red LED | 22 | LOW = ON |
| Green LED | 16 | LOW = ON |
| Blue LED | 17 | LOW = ON |
| **Audio** | | |
| Audio EN | 4 | LOW = Enable |
| Audio Out | 26 | DAC |
| **System** | | |
| Boot BTN | 0 | Mode select |
| Battery ADC | 34 | Input only |
| Serial TX | 1 | UART |
| Serial RX | 3 | UART |
| SPI SS | 27 | Peripheral |
| GPIO35 | 35 | NC, Input only |

### Build Commands

```bash
# Standard build
pio run -e ESP32-32E

# Upload firmware
pio run -e ESP32-32E -t upload

# Clean build
pio run -e ESP32-32E -t clean
```

### Display Configuration
- **Driver**: ILI9341
- **Resolution**: 240 x 320
- **Interface**: HSPI @ 80MHz
- **Orientation**: Portrait (ROTATION=1)
- **Colors**: RGB565 (65K colors)

### Touch Configuration
- **Controller**: XPT2046
- **Type**: 4-wire resistive
- **Interface**: SPI @ 2.5MHz
- **Calibration**: Auto on first boot
- **Interrupt**: GPIO36 (active LOW)

### Important Notes

1. **Touch Calibration**: First boot will run calibration - follow on-screen instructions
2. **RGB LED**: Common anode - use `digitalWrite(pin, LOW)` to turn ON
3. **Audio Enable**: Set GPIO4 LOW to enable audio amplifier
4. **SD Card**: Shares SPI bus with peripheral port (GPIO27 CS)
5. **Battery**: Read ADC on GPIO34 for battery voltage (0-4.2V range)

### Known Compatible Modules
- Elegoo ESP32-32E Display Module
- 2.8" ILI9341 ESP32 CYD displays with resistive touch
- ESP32-WROOM-32E based development boards with compatible pinout

### Testing Checklist
- [ ] Display shows correctly
- [ ] Touch responds accurately
- [ ] RGB LED all colors work
- [ ] SD card mounts
- [ ] Audio output (if used)
- [ ] Battery reading (if used)
- [ ] Deep sleep wake on touch
- [ ] All buttons functional

### Support
For issues specific to ESP32-32E hardware, refer to:
- [ESP32-32E Setup Guide](../../ESP32-32E_SETUP_GUIDE.md)
- [Elegoo Documentation](../../esp32e-docs/)
- [Bruce Firmware Issues](https://github.com/pr3y/Bruce/issues)
