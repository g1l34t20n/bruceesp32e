# ESP32-32E Troubleshooting Guide

## Current Issue: Purple Boot Screen → White Screen Crash

### Symptoms
- Device shows purple "Booting" text briefly
- Screen turns white/blank
- RGB LED flashes once
- Serial output stops after hardware config JSON
- Never reaches main firmware UI

### Root Cause
The firmware **partially boots** but **crashes during initialization** after displaying "Booting" text. The crash occurs somewhere between `begin_storage()` and `_post_setup_gpio()`.

---

## Step-by-Step Debugging

### Step 1: Verify Hardware with Minimal Test ⚡

First, confirm your ESP32-32E hardware works independently of Bruce firmware:

```bash
cd C:\Users\Administrator\Documents\CODE\GitHub\BruceEsp32e
git pull origin main
pio run -e ESP32-32E-TEST -t upload
pio device monitor --port COM6 --baud 115200
```

**Expected Results:**
- ✅ Display shows: RED → GREEN → BLUE color test
- ✅ White text: "ESP32-32E Test", "Display: OK"
- ✅ RGB LED cycles: RED → GREEN → BLUE
- ✅ Green box with "ALL OK!" message
- ✅ Serial output: All 5 hardware tests pass

**If minimal test FAILS:**
- Hardware issue (display, connections, or module defect)
- Check solder joints, ribbon cables
- Verify power supply (5V/1A minimum)

**If minimal test SUCCEEDS:**
- ✅ Hardware is good
- 🔴 Problem is in Bruce firmware initialization
- Continue to Step 2

---

### Step 2: Identify Exact Crash Point

Add debug prints to track where the firmware crashes:

```bash
cd C:\Users\Administrator\Documents\CODE\GitHub\BruceEsp32e
```

**Option A: Manual Debug (Recommended)**

Edit `src/main.cpp` and add these lines:

1. **After line ~377** (`Serial.begin(115200);`):
```cpp
Serial.println("\n\n========================================");
Serial.println("[DEBUG] setup() STARTED");
Serial.println("========================================");
Serial.flush(); delay(100);
```

2. **After line ~395** (`setup_gpio();`):
```cpp
Serial.println("[DEBUG] setup_gpio() COMPLETED");
Serial.flush(); delay(100);
```

3. **After line ~398** (`tft.init();`):
```cpp
Serial.println("[DEBUG] tft.init() COMPLETED");
Serial.flush(); delay(100);
```

4. **After line ~404** (`tft.drawCentreString("Booting"...);`):
```cpp
Serial.println("[DEBUG] 'Booting' text displayed");
Serial.flush(); delay(100);
```

5. **After line ~407** (`begin_storage();`):
```cpp
Serial.println("[DEBUG] begin_storage() COMPLETED");
Serial.flush(); delay(100);
```

6. **After line ~408** (`begin_tft();`):
```cpp
Serial.println("[DEBUG] begin_tft() COMPLETED");
Serial.flush(); delay(100);
```

7. **After line ~409** (`init_clock();`):
```cpp
Serial.println("[DEBUG] init_clock() COMPLETED");
Serial.flush(); delay(100);
```

8. **After line ~410** (`init_led();`):
```cpp
Serial.println("[DEBUG] init_led() COMPLETED");
Serial.flush(); delay(100);
```

9. **After line ~413** (`_post_setup_gpio();`):
```cpp
Serial.println("[DEBUG] _post_setup_gpio() COMPLETED");
Serial.flush(); delay(100);
```

**Then rebuild and flash:**
```bash
pio run -e ESP32-32E -t upload
pio device monitor --port COM6 --baud 115200
```

**Analyze Serial Output:**
The last debug message printed before the crash identifies the failing function.

Example:
```
[DEBUG] begin_storage() COMPLETED
[DEBUG] begin_tft() COMPLETED
[Crash occurs here - no further messages]
```
→ **Crash is in `init_clock()`**

---

### Step 3: Common Crash Causes & Fixes

#### 🔴 Crash in `begin_storage()` (SD Card)
**Symptoms:** Last message is `[DEBUG] 'Booting' text displayed`

**Cause:** SD card initialization hanging or crashing

**Fix:**
```ini
# In boards/ESP32-32E/ESP32-32E.ini, add:
-DSD_CARD_DISABLED=1
```

Or physically remove SD card while testing.

---

#### 🔴 Crash in `begin_tft()` (Display Reconfiguration)
**Symptoms:** Last message is `[DEBUG] begin_storage() COMPLETED`

**Cause:** `setBrightness()` called before LEDC ready, or rotation issue

**Fix:** Already applied in latest code. If still crashes, try:
```ini
# In boards/ESP32-32E/ESP32-32E.ini:
-DROTATION=0  # Try different rotations: 0, 1, 2, 3
```

---

#### 🔴 Crash in `init_clock()` (RTC)
**Symptoms:** Last message is `[DEBUG] begin_tft() COMPLETED`

**Cause:** No RTC module on ESP32-32E

**Fix:**
```ini
# In boards/ESP32-32E/ESP32-32E.ini:
-DHAS_RTC=0
```

---

#### 🔴 Crash in `init_led()` (RGB LED)
**Symptoms:** Last message is `[DEBUG] init_clock() COMPLETED`

**Cause:** LED initialization conflict

**Fix:** Already disabled in config. If crashes, check `src/core/led_control.cpp`.

---

#### 🔴 Crash in `_post_setup_gpio()` (Touch Calibration)
**Symptoms:** Last message is `[DEBUG] init_led() COMPLETED`

**Cause:** Touch SPI initialization or LittleFS file access

**Fix:**
```bash
# Erase flash to reset calibration:
esptool.py --port COM6 erase_flash
# Then reflash firmware
pio run -e ESP32-32E -t upload
```

---

### Step 4: Alternative Display Driver

If crashes persist, try the **color-inverted** variant:

```bash
pio run -e ESP32-32E-INV -t upload
```

This uses `TFT_INVERSION_ON` instead of `TFT_INVERSION_OFF`.

---

## Hardware Verification Checklist

Before debugging firmware, verify:

- [ ] **Power Supply:** 5V/1A minimum (USB 2.0 may be insufficient)
- [ ] **Serial Connection:** COM port accessible, no other programs using it
- [ ] **USB Cable:** Use data cable (not charge-only cable)
- [ ] **Driver:** CP210x or CH340 driver installed
- [ ] **Module:** Elegoo ESP32-32E (not different CYD variant)
- [ ] **Firmware:** Latest from GitHub (`git pull origin main`)
- [ ] **Build:** Clean rebuild (`pio run -t clean`)

---

## Serial Monitor Tools

If `pio device monitor` keeps failing, use alternatives:

### PowerShell Script:
```powershell
# Save as monitor.ps1
$port = New-Object System.IO.Ports.SerialPort COM6,115200,None,8,One
$port.Open()
while($true) {
    if($port.BytesToRead -gt 0) {
        Write-Host $port.ReadExisting() -NoNewline
    }
    Start-Sleep -Milliseconds 10
}
```

Run: `powershell -File monitor.ps1`

### PuTTY:
1. Download: https://www.putty.org/
2. Connection type: Serial
3. Serial line: COM6
4. Speed: 115200
5. Click "Open"

### Arduino IDE:
1. Tools → Port → COM6
2. Tools → Serial Monitor
3. Set baud rate: 115200

---

## Build Issues

### Long Path Errors
Already fixed. If recurs:
```powershell
# Run as Administrator:
New-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\FileSystem" -Name "LongPathsEnabled" -Value 1 -PropertyType DWORD -Force
git config --system core.longpaths true
# Restart computer
```

### Library Install Errors
Ignore SmartRC-CC1101 errors for `lilygo-t-embed-cc1101` environment (not used by ESP32-32E).

### Compilation Errors
```bash
# Clean rebuild:
cd C:\Users\Administrator\Documents\CODE\GitHub\BruceEsp32e
pio run -t clean
rd /s /q .pio\build\ESP32-32E
rd /s /q .pio\libdeps\ESP32-32E
pio run -e ESP32-32E
```

---

## Quick Reference: Commands

```bash
# Get latest code
git pull origin main

# Build firmware
pio run -e ESP32-32E

# Flash firmware
pio run -e ESP32-32E -t upload

# Monitor serial
pio device monitor --port COM6 --baud 115200

# Flash minimal test
pio run -e ESP32-32E-TEST -t upload

# Erase flash completely
esptool.py --port COM6 erase_flash

# Manual flash (if upload fails)
esptool.py --chip esp32 --port COM6 --baud 921600 write_flash -z 0x0 .pio\build\ESP32-32E\firmware.bin
```

---

## Next Steps

1. ✅ **Run minimal test** (`ESP32-32E-TEST`) to confirm hardware
2. ✅ **Add debug prints** to `src/main.cpp` (see Step 2)
3. ✅ **Flash instrumented firmware** and capture serial output
4. ✅ **Report last debug message** before crash
5. ✅ **Apply specific fix** from Step 3 based on crash location

---

## Get Help

When reporting issues, include:

1. **Serial output** (full log from boot to crash)
2. **Last debug message** printed
3. **Photo of screen** (purple boot / white screen)
4. **Minimal test results** (did ESP32-32E-TEST work?)
5. **Hardware variant** (confirm Elegoo ESP32-32E, not different CYD)

---

## Summary

Your ESP32-32E hardware is likely **working correctly** (minimal test will confirm this). The issue is **Bruce firmware crashes during initialization**. By adding debug prints, we'll pinpoint the exact function causing the crash, then apply a targeted fix.

**Most likely causes:**
1. SD card initialization hanging → Disable SD or remove card
2. LEDC/brightness timing issue → Already fixed in latest code
3. Touch calibration crash → Erase flash to reset
4. RTC initialization (no RTC on this board) → Disable RTC

Follow Step 1 (minimal test) first, then Step 2 (debug prints) to identify the culprit! 🚀
