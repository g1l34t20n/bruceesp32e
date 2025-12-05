# ESP32 Marauder Firmware for ESP32-32E

## About ESP32 Marauder

**ESP32 Marauder** (also called "Willy" firmware by some) is WiFi/Bluetooth penetration testing firmware originally created by justcallmekoko.

**Official Repository:** https://github.com/justcallmekoko/ESP32Marauder

**What it does:**
- WiFi packet sniffing and injection
- Deauth attacks
- Beacon spam
- Bluetooth scanning (BLE spam)
- PCAP file capture
- BadUSB attacks (some versions)

---

## ⚠️ Important Notes

### **ESP32-32E Compatibility:**

The ESP32-32E (Elegoo CYD) is **NOT officially supported** by ESP32 Marauder out of the box because:
- Different display (ILI9341 vs other supported displays)
- Different touch controller (XPT2046 resistive)
- Custom pin mappings

**You would need to:**
1. Fork the Marauder repository
2. Add ESP32-32E board configuration (like we did for Bruce)
3. Configure display/touch drivers
4. Compile custom build

---

## 🎯 Recommendation: Use Bruce Firmware Instead

**Bruce firmware already has most Marauder features:**
- ✅ WiFi scanning and attacks (beacon spam, deauth)
- ✅ BLE scanning and attacks
- ✅ RF attacks (CC1101/NRF24)
- ✅ BadUSB (Bluetooth HID)
- ✅ IR blaster (TV-B-Gone)
- ✅ RFID/NFC tools
- ✅ **Already configured for your ESP32-32E!**

Bruce is essentially "Marauder + more features" and **already works on your board**.

---

## 🔧 If You Really Want Marauder

### **Option 1: Use a Supported Board**

Marauder officially supports:
- **ESP32-S2**
- **ESP32-S3** (recommended)
- **LILYGO T-Display-S3**
- **Flipper Zero WiFi Dev Board**
- **ESP32-WROOM DevKit**

Buy one of these if you want official Marauder.

---

### **Option 2: Port Marauder to ESP32-32E (Advanced)**

**Steps:**

1. **Clone Marauder repo:**
```bash
git clone https://github.com/justcallmekoko/ESP32Marauder.git
cd ESP32Marauder
```

2. **Add ESP32-32E board config:**

Create `esp32_marauder/configs/ESP32-32E.h`:
```cpp
#ifndef ESP32_32E_H
#define ESP32_32E_H

#include "MenuFunctions.h"
#include "Display.h"

// Display settings
#define TFT_CS 15
#define TFT_DC 2
#define TFT_RST -1
#define TFT_MOSI 13
#define TFT_MISO 12
#define TFT_SCLK 14
#define TFT_BL 21

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Touch settings
#define TOUCH_CS 33
#define TOUCH_IRQ 36

// SD Card
#define SD_CS 5
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCK 18

// LED (disabled for CC1101)
// #define LED_PIN 22  // Disabled, used by CC1101

// Button
#define BUTTON_PIN 0

#endif
```

3. **Modify `platformio.ini`:**

Add environment:
```ini
[env:ESP32-32E]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
build_flags = 
    ${env.build_flags}
    -DESP32_32E
    -DILI9341_DRIVER
    -DUSER_SETUP_LOADED
    -DTFT_WIDTH=240
    -DTFT_HEIGHT=320
lib_deps = 
    ${env.lib_deps}
    bodmer/TFT_eSPI@^2.5.0
```

4. **Update display initialization in `Display.cpp`**

5. **Update touch handling for XPT2046**

6. **Test and debug**

**This is a LOT of work** and will take several hours of development time.

---

## 💡 Better Alternative: Flash Pre-Built Marauder for Supported Board

If you have a supported ESP32-S3 board, you can use the **Marauder Flasher:**

**Web Flasher:** https://esp32marauder.com/

Just connect your ESP32 via USB and click "Flash" - done in 2 minutes!

---

## 🚀 Quickest Solution: Use What You Have

**Your ESP32-32E with Bruce firmware can already do:**

### **WiFi Attacks:**
- Navigate to: **WiFi → Scan**
- Deauth attack: **WiFi → Beacon Spam**
- SSID spam: **WiFi → Evil Portal**

### **Bluetooth Attacks:**
- Navigate to: **Bluetooth → BLE Spam**
- Keyboard injection: **Bluetooth → BadUSB**

### **RF Attacks (with CC1101):**
- Navigate to: **RF → Scan/Copy**
- Replay attacks: **RF → Custom SubGhz**
- Frequency jammer: **RF → Jammer**

### **RFID/NFC:**
- Navigate to: **RFID → Read**
- Clone cards: **RFID → Clone**

**Bruce already does 90% of what Marauder does!** And it's **already working** on your ESP32-32E.

---

## 📚 Resources

### **ESP32 Marauder:**
- GitHub: https://github.com/justcallmekoko/ESP32Marauder
- Wiki: https://github.com/justcallmekoko/ESP32Marauder/wiki
- Discord: https://discord.gg/marauder

### **Bruce Firmware (what you have):**
- GitHub: https://github.com/pr3y/Bruce
- Wiki: https://github.com/pr3y/Bruce/wiki
- Website: https://bruce.computer

---

## ✅ My Recommendation

**Stick with Bruce firmware** on your ESP32-32E because:

1. ✅ **Already configured** for your hardware
2. ✅ **More features** than Marauder (RF, IR, NFC, etc.)
3. ✅ **Active development** for this board
4. ✅ **Working right now** - no porting needed
5. ✅ **Community support** for ESP32-32E

If you **really** want Marauder, buy a **LILYGO T-Display-S3** (~$20) and use the web flasher. That's 100x easier than porting Marauder to ESP32-32E.

---

## 🔧 For Your CC1101 Issue

**First, let's fix your CC1101 hardware problem with the diagnostic tool!**

```bash
cd C:\Users\Administrator\Documents\CODE\GitHub\BruceEsp32e
git pull origin main
pio run -e ESP32-32E-CC1101-TEST -t upload --upload-port COM2
pio device monitor --port COM2 --baud 115200
```

This will test **every single pin** and tell you exactly what's wrong:
- ✓ Shorts to ground/VCC
- ✓ Voltage levels
- ✓ SPI communication
- ✓ Module detection
- ✓ Continuity issues

Once we see the diagnostic output, we'll know exactly what's wrong! 🔍
