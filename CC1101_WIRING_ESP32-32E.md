# CC1101 / NRF24 Module Wiring for ESP32-32E

## 📡 Pin Configuration

Your ESP32-32E is now configured to match **ESP-WROOM-32 Rev 3 CYD** wiring for CC1101/NRF24 modules.

---

## 🔌 CC1101 Module Wiring

### **Pin Connections:**

| CC1101 Pin | ESP32-32E GPIO | Function | Notes |
|-----------|----------------|----------|-------|
| **GDO0** | **IO22** | Data/Signal | ✅ Bidirectional (was RGB Red LED) |
| **GDO2** | IO35 (optional) | Secondary signal | ⚠️ Input-only (ADC) |
| **CS** (CSN) | IO27 | Chip Select | Shared with NRF24/W5500 |
| **MOSI** (SI) | IO23 | SPI Data Out | Shared SPI bus |
| **MISO** (SO) | IO19 | SPI Data In | Shared SPI bus |
| **SCK** (SCLK) | IO18 | SPI Clock | Shared SPI bus |
| **VCC** | 3.3V | Power | ⚠️ NOT 5V! |
| **GND** | GND | Ground | |

---

## 📻 NRF24L01 Module Wiring

The NRF24 uses the **same wiring** as CC1101 (they share the SPI bus):

| NRF24 Pin | ESP32-32E GPIO | Function | Notes |
|-----------|----------------|----------|-------|
| **CE** | **IO22** | Chip Enable | Same as CC1101 GDO0 |
| **CSN** | IO27 | Chip Select | Shared with CC1101 |
| **MOSI** | IO23 | SPI Data Out | Shared SPI bus |
| **MISO** | IO19 | SPI Data In | Shared SPI bus |
| **SCK** | IO18 | SPI Clock | Shared SPI bus |
| **IRQ** | Not connected | Interrupt | Optional |
| **VCC** | 3.3V | Power | ⚠️ Add 10µF capacitor! |
| **GND** | GND | Ground | |

---

## ⚠️ Important Notes

### **1. IO22 Change**
- **Previously:** RGB LED Red (common anode)
- **Now:** CC1101 GDO0 / NRF24 CE
- **Trade-off:** RGB LED disabled to free IO22

### **2. Why Not IO35?**
You asked about using IO35 instead of IO22. **IO35 is INPUT-ONLY** on ESP32:
- ❌ No internal pullup
- ❌ Cannot drive output signals
- ❌ ADC input only (read-only)
- ✅ Can be used for GDO2 (secondary, input signal)

**GDO0/CE needs bidirectional capability**, so IO22 is the correct choice.

### **3. Shared SPI Bus**
CC1101, NRF24, W5500, and SD Card all share the same SPI bus (IO18/19/23). They use different CS pins:
- **CC1101:** CS = IO27
- **NRF24:** CS = IO27 (same as CC1101)
- **W5500:** CS = IO27 (same!)
- **SD Card:** CS = IO5

⚠️ **You can only use ONE module at a time** (CC1101 OR NRF24 OR W5500) since they share CS=IO27.

---

## 🔧 How to Change Back (If Needed)

If you need the RGB LED and want to wire CC1101 differently:

### **Option A: Use IO4 for GDO0**
Edit `boards/ESP32-32E/ESP32-32E.ini`:

```ini
;Uncomment RGB LED
-DHAS_RGB_LED=1
-DRGB_LED_R=22
-DRGB_LED_G=16
-DRGB_LED_B=17

;Change CC1101 to IO4
-DCC1101_GDO0_PIN=4
-DNRF24_CE_PIN=4
```

**Wire CC1101 GDO0 to IO4 instead.**

---

## 🛠️ Building Firmware

After pulling the latest code:

```bash
git pull origin main
pio run -e ESP32-32E -t upload
```

The firmware is now configured for IO22 CC1101 wiring.

---

## 📋 Configuration Summary

Current ESP32-32E pin assignments:

```ini
; CC1101 / NRF24 Configuration
-DCC1101_GDO0_PIN=22        ; Was 4 (now matches CYD Rev 3)
-DCC1101_SS_PIN=27          ; Chip select
-DCC1101_MOSI_PIN=23        ; SPI MOSI
-DCC1101_SCK_PIN=18         ; SPI SCK
-DCC1101_MISO_PIN=19        ; SPI MISO
-DCC1101_GDO2_PIN=35        ; Optional (input-only)

-DNRF24_CE_PIN=22           ; Same as CC1101 GDO0
-DNRF24_SS_PIN=27           ; Same as CC1101 CS

; RGB LED - DISABLED
;-DHAS_RGB_LED=1            ; Commented out
;-DRGB_LED_R=22             ; Now used by CC1101
```

---

## 🧪 Testing CC1101

After wiring and flashing:

1. **Power on** the ESP32-32E
2. **Navigate to** RF menu in Bruce firmware
3. **Select** CC1101 scanner or transmitter
4. **Check serial output** for "CC1101 initialized"

If it doesn't work:
- Check wiring (especially GND and 3.3V)
- Verify CC1101 is getting power
- Check serial output for error messages

---

## 🎯 Quick Reference

**To wire CC1101 module matching CYD Rev 3:**

```
CC1101    →    ESP32-32E
------         ----------
GDO0      →    IO22 (was RGB Red)
CS        →    IO27
MOSI      →    IO23
MISO      →    IO19
SCK       →    IO18
GND       →    GND
VCC       →    3.3V (NOT 5V!)
```

**Trade-off:** RGB LED disabled (IO22 freed for CC1101).

---

## ✅ Summary

✅ **CC1101 GDO0 remapped to IO22** (matches CYD Rev 3)  
✅ **NRF24 CE remapped to IO22** (same wiring)  
✅ **RGB LED disabled** (IO22 freed)  
✅ **IO35 explained** (input-only, not suitable for GDO0)  
✅ **Shared CS on IO27** (CC1101/NRF24/W5500 - use one at a time)  

Your ESP32-32E is now ready for CC1101/NRF24 modules with standard CYD wiring! 🚀
