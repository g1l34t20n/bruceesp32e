# ESP32-32E Quick Start Guide

## 🎯 Your Situation

**Problem:** Device shows purple "Booting" text, then white screen  
**Cause:** Firmware crashes during initialization  
**Solution:** 3-step diagnostic process below

---

## 🚀 Quick Fix Attempt (Try This First!)

```bash
cd C:\Users\Administrator\Documents\CODE\GitHub\BruceEsp32e
git pull origin main
pio run -e ESP32-32E-TEST -t upload
```

**This flashes a minimal hardware test that:**
- ✅ Bypasses Bruce firmware complexity
- ✅ Tests display, LEDs, touch, SD pins
- ✅ Takes 30 seconds to build & flash
- ✅ Proves hardware works

**Expected result:** Display shows color test (red/green/blue) → "ALL OK!" message

---

## 📋 3-Step Diagnostic Process

### Step 1: Verify Hardware (5 minutes)

```bash
# Flash minimal test
pio run -e ESP32-32E-TEST -t upload

# Watch serial output
pio device monitor --port COM6 --baud 115200
```

**What you should see:**
```
[1/5] Testing backlight...
  ✓ Backlight ON (should be bright)
[2/5] Initializing display...
  ✓ tft.init() completed
  ✓ Color test (red/green/blue) completed
[3/5] Testing RGB LED...
  Testing RED...
  Testing GREEN...
  Testing BLUE...
[4/5] Setting up touch...
  ✓ Touch CS configured
[5/5] Checking SD Card pins...
  ✓ SD CS configured

========================================
HARDWARE TEST COMPLETED
========================================
```

**Display should show:**
- Red screen (0.5s) → Green (0.5s) → Blue (0.5s)
- White text: "ESP32-32E Test", "Display: OK"
- Large green box: "ALL OK!"

**Outcome:**
- ✅ **Test passes** → Hardware good, proceed to Step 2
- ❌ **Test fails** → Hardware issue, check connections/power

---

### Step 2: Find Crash Location (10 minutes)

Edit `src/main.cpp` and add debug prints after each initialization step.

**Copy these lines into `src/main.cpp`:**

Find line ~377 (`Serial.begin(115200);`) and add after it:
```cpp
Serial.println("\n\n========================================");
Serial.println("[DEBUG] setup() STARTED");
Serial.println("========================================");
Serial.flush(); delay(100);
```

Find line ~395 (`setup_gpio();`) and add after it:
```cpp
Serial.println("[DEBUG] setup_gpio() COMPLETED");
Serial.flush(); delay(100);
```

Find line ~398 (`tft.init();`) and add after it:
```cpp
Serial.println("[DEBUG] tft.init() COMPLETED");
Serial.flush(); delay(100);
```

Find line ~404 (`tft.drawCentreString("Booting"...);`) and add after it:
```cpp
Serial.println("[DEBUG] 'Booting' text displayed");
Serial.flush(); delay(100);
```

Find line ~407 (`begin_storage();`) and add after it:
```cpp
Serial.println("[DEBUG] begin_storage() COMPLETED");
Serial.flush(); delay(100);
```

Find line ~408 (`begin_tft();`) and add after it:
```cpp
Serial.println("[DEBUG] begin_tft() COMPLETED");
Serial.flush(); delay(100);
```

Find line ~409 (`init_clock();`) and add after it:
```cpp
Serial.println("[DEBUG] init_clock() COMPLETED");
Serial.flush(); delay(100);
```

Find line ~410 (`init_led();`) and add after it:
```cpp
Serial.println("[DEBUG] init_led() COMPLETED");
Serial.flush(); delay(100);
```

Find line ~413 (`_post_setup_gpio();`) and add after it:
```cpp
Serial.println("[DEBUG] _post_setup_gpio() COMPLETED");
Serial.flush(); delay(100);
```

**Then rebuild and flash:**
```bash
pio run -e ESP32-32E -t upload
pio device monitor --port COM6 --baud 115200
```

**Look for the LAST debug message before crash:**

Example output:
```
[DEBUG] setup() STARTED
[DEBUG] setup_gpio() COMPLETED
[DEBUG] tft.init() COMPLETED
[DEBUG] 'Booting' text displayed
[DEBUG] begin_storage() COMPLETED
[DEBUG] begin_tft() COMPLETED
[Crash here - no more messages]
```

→ **Crash is in `init_clock()`** ← This tells us what to fix!

---

### Step 3: Apply the Fix (Based on Step 2 Results)

#### 🔧 If crash is in `begin_storage()`
**Fix: Disable SD card temporarily**

Edit `boards/ESP32-32E/ESP32-32E.ini`, add this line:
```ini
-DSD_CARD_DISABLED=1
```

Or physically remove SD card.

---

#### 🔧 If crash is in `begin_tft()`
**Fix: Already applied in latest code**

Make sure you have latest:
```bash
git pull origin main
pio run -t clean
pio run -e ESP32-32E
```

---

#### 🔧 If crash is in `init_clock()`
**Fix: Disable RTC (ESP32-32E has no RTC chip)**

Edit `boards/ESP32-32E/ESP32-32E.ini`, find and comment out:
```ini
;-DHAS_RTC=1  # Add semicolon to disable
```

Or add:
```ini
-DHAS_RTC=0
```

---

#### 🔧 If crash is in `init_led()`
**Fix: Already disabled in config**

Check `boards/ESP32-32E/ESP32-32E.ini` has:
```ini
;-DHAS_RGB_LED=1  # Should be commented out
```

---

#### 🔧 If crash is in `_post_setup_gpio()`
**Fix: Reset touch calibration**

```bash
# Erase all flash data
esptool.py --port COM6 erase_flash

# Reflash firmware
pio run -e ESP32-32E -t upload
```

---

## 🎬 Expected Final Result

After applying the fix, you should see:

**Serial output:**
```
[DEBUG] setup() STARTED
[DEBUG] setup_gpio() COMPLETED
[DEBUG] tft.init() COMPLETED
[DEBUG] 'Booting' text displayed
[DEBUG] begin_storage() COMPLETED
[DEBUG] begin_tft() COMPLETED
[DEBUG] init_clock() COMPLETED
[DEBUG] init_led() COMPLETED
[DEBUG] _post_setup_gpio() COMPLETED
Backlight initialized (GPIO mode)
LEDC initialized successfully
Touch calibration data loaded
```

**Display:**
- ✅ Backlight turns on
- ✅ Boot animation plays
- ✅ Main menu appears
- ✅ Touch works

---

## 🛠️ Tools & Commands Reference

### Build & Flash
```bash
# Get latest code
git pull origin main

# Clean build
pio run -t clean

# Build firmware
pio run -e ESP32-32E

# Flash to device
pio run -e ESP32-32E -t upload

# Monitor serial
pio device monitor --port COM6 --baud 115200

# All in one
pio run -e ESP32-32E -t upload && pio device monitor --port COM6 --baud 115200
```

### Minimal Test
```bash
# Flash hardware test
pio run -e ESP32-32E-TEST -t upload
```

### Alternative Display Driver
```bash
# Try color-inverted version
pio run -e ESP32-32E-INV -t upload
```

### Emergency Reset
```bash
# Erase everything
esptool.py --port COM6 erase_flash

# Reflash
pio run -e ESP32-32E -t upload
```

---

## 📚 Documentation

- **[ESP32-32E_SETUP_GUIDE.md](ESP32-32E_SETUP_GUIDE.md)** - Hardware specs, pin mappings, configuration
- **[ESP32-32E_TROUBLESHOOTING.md](ESP32-32E_TROUBLESHOOTING.md)** - Detailed debugging steps
- **[boards/ESP32-32E/README.md](boards/ESP32-32E/README.md)** - Board-specific configuration
- **[test/ESP32-32E_minimal_test.ino](test/ESP32-32E_minimal_test.ino)** - Minimal hardware test source

---

## 🆘 Still Not Working?

### Check These Common Issues:

1. **Wrong COM port** → Use Device Manager to verify
2. **Other program using serial** → Close Arduino IDE, Putty, etc.
3. **USB cable** → Try different cable (must be data cable)
4. **Driver missing** → Install CP210x or CH340 driver
5. **Power insufficient** → Use USB 3.0 port or powered hub
6. **Wrong board variant** → Confirm it's Elegoo ESP32-32E (not different CYD)

### Alternative Serial Monitors:

If `pio device monitor` fails, use:

**PuTTY** (Download: https://www.putty.org/)
- Connection type: Serial
- Serial line: COM6
- Speed: 115200

**Arduino IDE Serial Monitor**
- Tools → Port → COM6
- Tools → Serial Monitor
- Baud: 115200

**PowerShell** (Windows):
```powershell
$port = New-Object System.IO.Ports.SerialPort COM6,115200,None,8,One
$port.Open()
while($true) {
    if($port.BytesToRead -gt 0) {
        Write-Host $port.ReadExisting() -NoNewline
    }
    Start-Sleep -Milliseconds 10
}
```

---

## 📧 Report Issues

Include:
1. Full serial output (from boot to crash)
2. Last debug message printed
3. Photo of screen (purple boot / white screen)
4. Minimal test result (did ESP32-32E-TEST work?)

---

## 🎯 TL;DR - Absolute Quickest Path

```bash
# 1. Flash minimal test (30 seconds)
cd C:\Users\Administrator\Documents\CODE\GitHub\BruceEsp32e
git pull origin main
pio run -e ESP32-32E-TEST -t upload

# 2. If test passes, add debug prints to src/main.cpp (see Step 2 above)

# 3. Flash instrumented firmware
pio run -e ESP32-32E -t upload

# 4. Check serial output for last debug message

# 5. Apply specific fix from Step 3 based on where it crashed

# 6. Rebuild and enjoy! 🎉
```

**Most likely fix needed:** Disable RTC or SD card in `boards/ESP32-32E/ESP32-32E.ini`

Good luck! The finish line is close! 🚀
