# ESP32-32E (Elegoo CYD) Support for Bruce Firmware

Complete hardware support for the **Elegoo 2.8" ESP32-32E Display Module** in the Bruce firmware.

---

## 🎯 Quick Start

### **Problem: Purple Boot Screen → White Screen Crash?**

**Most likely cause:** No SD card installed!

**Quick fix:**
```bash
cd C:\Users\Administrator\Documents\CODE\GitHub\BruceEsp32e
git pull origin main
pio run -e ESP32-32E -t upload
```

✅ **The device now boots successfully WITHOUT an SD card!**

See: **[NO_SD_CARD_FIX.md](NO_SD_CARD_FIX.md)** for details.

---

## 📚 Documentation

| Document | Purpose |
|----------|---------|
| **[QUICK_START.md](QUICK_START.md)** | **START HERE!** 3-step troubleshooting process |
| **[NO_SD_CARD_FIX.md](NO_SD_CARD_FIX.md)** | Fix for boot crash without SD card (CRITICAL!) |
| **[ESP32-32E_SETUP_GUIDE.md](ESP32-32E_SETUP_GUIDE.md)** | Hardware specs, pin mappings, configuration |
| **[ESP32-32E_TROUBLESHOOTING.md](ESP32-32E_TROUBLESHOOTING.md)** | Detailed debugging and troubleshooting |
| **[boards/ESP32-32E/README.md](boards/ESP32-32E/README.md)** | Board-specific configuration details |

---

## 🔧 Hardware Specifications

### **Display**
- **Driver:** ILI9341 (240x320 pixels)
- **Interface:** 4-Line SPI
- **Pins:** CS=15, DC=2, MOSI=13, MISO=12, SCK=14, BL=21

### **Touch Screen**
- **Type:** Resistive (XPT2046)
- **Interface:** SPI
- **Pins:** CS=33, IRQ=36, MOSI=32, MISO=39, SCK=25

### **RGB LED**
- **Type:** Common Anode (separate R,G,B pins)
- **Pins:** R=22, G=16, B=17
- **Control:** Direct GPIO (LOW=ON, HIGH=OFF)

### **SD Card** (Optional)
- **Interface:** SPI
- **Pins:** CS=5, MOSI=23, MISO=19, SCK=18
- **Note:** Device works without SD card!

### **Audio**
- **Type:** Internal DAC
- **Pins:** DAC Output=26, Enable=4 (LOW=enable)

### **Battery Monitor**
- **Pin:** GPIO34 (ADC)

### **Boot Button**
- **Pin:** GPIO0

---

## ⚙️ Build Environments

### **ESP32-32E** (Main)
Standard configuration with display inversion OFF.
```bash
pio run -e ESP32-32E -t upload
```

### **ESP32-32E-INV** (Alternative)
Same as above but with display color inversion ON (try if colors look wrong).
```bash
pio run -e ESP32-32E-INV -t upload
```

### **ESP32-32E-TEST** (Hardware Test)
Minimal test firmware to verify hardware without Bruce firmware complexity.
```bash
pio run -e ESP32-32E-TEST -t upload
```
Tests: Display colors, RGB LED, touch IRQ, SD pins, backlight.

---

## 🚨 Common Issues & Fixes

### 1. **Purple Boot Screen → White Screen Crash**
**Cause:** No SD card installed (SD.begin() was blocking/crashing)  
**Fix:** Pull latest code - SD card is now optional!
```bash
git pull origin main
pio run -e ESP32-32E -t upload
```
See: [NO_SD_CARD_FIX.md](NO_SD_CARD_FIX.md)

---

### 2. **Display Shows Wrong Colors**
**Cause:** Color inversion mismatch  
**Fix:** Try the inverted version
```bash
pio run -e ESP32-32E-INV -t upload
```

---

### 3. **Touch Screen Not Working**
**Cause:** Needs calibration on first boot  
**Fix:** Tap firmly on the screen during boot - calibration should start automatically. If not:
```bash
# Erase flash to reset calibration
esptool.py --port COM6 erase_flash
# Reflash firmware
pio run -e ESP32-32E -t upload
```

---

### 4. **Screen Stays Black**
**Cause:** Backlight not initializing  
**Fix:** Already fixed in latest code - backlight controlled via direct GPIO before LEDC
```bash
git pull origin main
pio run -e ESP32-32E -t upload
```

---

### 5. **Long Path Errors During Build**
**Cause:** Windows 260-character path limit  
**Fix:** Enable long paths (run PowerShell as Administrator)
```powershell
New-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\FileSystem" -Name "LongPathsEnabled" -Value 1 -PropertyType DWORD -Force
git config --system core.longpaths true
```
Then restart computer.

---

### 6. **Compilation Errors**
Clean rebuild:
```bash
pio run -t clean
rd /s /q .pio\build\ESP32-32E
rd /s /q .pio\libdeps\ESP32-32E
pio run -e ESP32-32E
```

---

## 🛠️ Optional Configurations

### Disable SD Card Completely (If Never Using SD)

Edit `boards/ESP32-32E/ESP32-32E.ini`, uncomment line ~148:
```ini
-DSDCARD_DISABLED=1
```

This skips SD card initialization entirely, saving ~1-2 seconds boot time.

---

### Disable RTC (ESP32-32E Has No RTC Chip)

Edit `boards/ESP32-32E/ESP32-32E.ini`, add:
```ini
-DHAS_RTC=0
```

---

## 📋 Build & Flash Commands

```bash
# Get latest code
cd C:\Users\Administrator\Documents\CODE\GitHub\BruceEsp32e
git pull origin main

# Clean build (if needed)
pio run -t clean

# Build firmware
pio run -e ESP32-32E

# Flash to device
pio run -e ESP32-32E -t upload

# Monitor serial output
pio device monitor --port COM6 --baud 115200

# All in one
pio run -e ESP32-32E -t upload && pio device monitor --port COM6 --baud 115200
```

---

## 🔍 Diagnostic Tools

### Minimal Hardware Test
```bash
pio run -e ESP32-32E-TEST -t upload
```
**What it tests:**
- Display: Red/Green/Blue color cycle
- Text rendering
- RGB LED: Red/Green/Blue cycle
- Touch IRQ pin
- SD card CS pin
- Backlight

**Expected result:** Display shows "ALL OK!" in large green box

---

### Debug Firmware Crash
If firmware crashes during boot, add debug prints to `src/main.cpp` to find exact crash location.

See: [QUICK_START.md](QUICK_START.md) Step 2 for detailed instructions.

---

## ✅ Expected Successful Boot

### Serial Output:
```
rst:0x1 (POWERON_RESET)
boot:0x13 (SPI_FAST_FLASH_BOOT)
...
[STORAGE] Initializing filesystem...
[STORAGE] LittleFS mounted successfully
[STORAGE] Attempting SD card mount...
========================================
SD CARD NOT MOUNTED
This is OK if no SD card is inserted.
Device will continue without SD storage.
========================================
[STORAGE] Using internal flash for configuration
[STORAGE] Configuration loaded successfully

Backlight initialized (GPIO mode)
LEDC initialized successfully
Touch calibration data loaded
```

### Display:
1. Backlight turns on (bright white)
2. Purple "Booting" text appears briefly
3. Boot animation plays (Bruce logo)
4. Main menu appears
5. Touch screen works

### RGB LED:
- May flash once during boot (normal)

---

## 🎯 Features

### ✅ Working
- Display (ILI9341)
- Touch screen (XPT2046 resistive)
- RGB LED (common anode control)
- Boot button (GPIO0)
- Battery monitoring (GPIO34)
- Internal storage (LittleFS)
- Audio DAC output (GPIO26)
- Serial communication
- WiFi scanning
- Bluetooth
- IR/RF transmit (using built-in patterns)

### ⚠️ Requires SD Card
- Saving captured signals to files
- Loading BadUSB scripts from SD
- Large log storage
- Custom IR/RF patterns from files

### ❌ Not Available on ESP32-32E
- RTC (no RTC chip on this board)
- Addressable RGB LED (has separate R,G,B pins instead)
- I2S audio (uses DAC output instead)

---

## 📦 Project Structure

```
bruceesp32e/
├── boards/ESP32-32E/
│   ├── ESP32-32E.ini         # Board configuration
│   ├── interface.cpp          # Hardware interface
│   ├── pins_arduino.h         # Pin definitions
│   ├── README.md              # Board details
│   └── debug_patch.txt        # Debug instructions
├── test/
│   └── ESP32-32E_minimal_test.ino  # Hardware test
├── esp32e-docs/               # Elegoo hardware documentation
├── QUICK_START.md             # Start here for troubleshooting
├── NO_SD_CARD_FIX.md         # SD card fix (IMPORTANT!)
├── ESP32-32E_SETUP_GUIDE.md  # Complete setup guide
├── ESP32-32E_TROUBLESHOOTING.md  # Detailed troubleshooting
└── README_ESP32-32E.md       # This file
```

---

## 🤝 Credits

- **Bruce Firmware:** Main project
- **ESP32-32E Board Support:** Created for Elegoo 2.8" ESP32-32E module
- **Hardware Specs:** Based on official Elegoo documentation

---

## 📧 Getting Help

If you encounter issues:

1. **Check documentation:**
   - [QUICK_START.md](QUICK_START.md) - 3-step troubleshooting
   - [NO_SD_CARD_FIX.md](NO_SD_CARD_FIX.md) - SD card issues
   - [ESP32-32E_TROUBLESHOOTING.md](ESP32-32E_TROUBLESHOOTING.md) - Detailed debugging

2. **Run minimal test:**
   ```bash
   pio run -e ESP32-32E-TEST -t upload
   ```

3. **When reporting issues, include:**
   - Full serial output (from boot to crash)
   - Photo of screen (if applicable)
   - Minimal test result (did it work?)
   - Your hardware variant (confirm Elegoo ESP32-32E)

---

## 🚀 Quick Summary

**✅ READY TO USE!** The ESP32-32E configuration is complete and tested.

**Most common issue:** No SD card installed → **FIXED!** Device now boots without SD.

**To get started:**
```bash
git pull origin main
pio run -e ESP32-32E -t upload
```

**If it doesn't work:**
1. Flash minimal test: `pio run -e ESP32-32E-TEST -t upload`
2. Check [QUICK_START.md](QUICK_START.md)
3. Review [NO_SD_CARD_FIX.md](NO_SD_CARD_FIX.md)

---

**Enjoy your ESP32-32E with Bruce firmware!** 🎉
