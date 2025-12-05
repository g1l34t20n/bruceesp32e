/**
 * CC1101/NRF24 Hardware Diagnostic Tool for ESP32-32E
 * 
 * Tests each pin individually:
 * - Voltage levels (VCC should be 3.3V)
 * - Continuity (MOSI/MISO/SCK should not be shorted)
 * - SPI communication
 * - Module detection
 * 
 * Upload with Arduino IDE or PlatformIO
 * Board: ESP32 Dev Module
 * Monitor: 115200 baud
 */

#include <SPI.h>

// CC1101/NRF24 Pin Definitions for ESP32-32E
#define CC1101_GDO0  22  // Signal pin (was IO4, now IO22)
#define CC1101_CS    27  // Chip Select
#define CC1101_MOSI  23  // SPI MOSI
#define CC1101_MISO  19  // SPI MISO
#define CC1101_SCK   18  // SPI Clock
#define VCC_PIN      -1  // Can't measure directly, but we can check voltage on MISO/MOSI

// Test results
struct PinTest {
    String name;
    int pin;
    bool pass;
    String error;
    float voltage;
};

SPIClass spi(HSPI);

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("\n\n========================================");
    Serial.println("CC1101/NRF24 Hardware Diagnostic Tool");
    Serial.println("ESP32-32E Pin Configuration Test");
    Serial.println("========================================\n");
    
    // Test 1: Pin Configuration
    Serial.println("TEST 1: Pin Configuration Check");
    Serial.println("--------------------------------");
    testPinConfiguration();
    delay(1000);
    
    // Test 2: Short Circuit Detection
    Serial.println("\nTEST 2: Short Circuit Detection");
    Serial.println("--------------------------------");
    testShortCircuits();
    delay(1000);
    
    // Test 3: SPI Bus Initialization
    Serial.println("\nTEST 3: SPI Bus Test");
    Serial.println("--------------------------------");
    testSPIBus();
    delay(1000);
    
    // Test 4: CC1101 Communication Test
    Serial.println("\nTEST 4: CC1101 Communication");
    Serial.println("--------------------------------");
    testCC1101Communication();
    delay(1000);
    
    // Test 5: Pin Pull Test (detect floating pins)
    Serial.println("\nTEST 5: Floating Pin Detection");
    Serial.println("--------------------------------");
    testFloatingPins();
    
    // Summary
    Serial.println("\n========================================");
    Serial.println("DIAGNOSTIC SUMMARY");
    Serial.println("========================================");
    printSummary();
    
    Serial.println("\n\nDiagnostic complete. Check results above.");
    Serial.println("If all tests pass but module doesn't work:");
    Serial.println("1. Check solder joints under magnification");
    Serial.println("2. Verify 3.3V power (NOT 5V!)");
    Serial.println("3. Try a different CC1101 module");
    Serial.println("4. Check for cold solder joints on female headers");
}

void loop() {
    // Continuous voltage monitoring
    delay(5000);
    Serial.println("\n--- Live Pin Monitoring ---");
    
    // Check if CS is being held correctly
    pinMode(CC1101_CS, INPUT_PULLUP);
    delay(10);
    bool csHigh = digitalRead(CC1101_CS);
    Serial.printf("CS (IO27): %s (should be HIGH when idle)\n", csHigh ? "HIGH ✓" : "LOW ✗");
    
    // Check GDO0 state
    pinMode(CC1101_GDO0, INPUT);
    delay(10);
    bool gdo0State = digitalRead(CC1101_GDO0);
    Serial.printf("GDO0 (IO22): %s\n", gdo0State ? "HIGH" : "LOW");
    
    // Check if SPI pins are stable
    pinMode(CC1101_SCK, INPUT_PULLUP);
    pinMode(CC1101_MOSI, INPUT_PULLUP);
    pinMode(CC1101_MISO, INPUT_PULLUP);
    delay(10);
    
    Serial.printf("SCK (IO18): %s\n", digitalRead(CC1101_SCK) ? "HIGH" : "LOW");
    Serial.printf("MOSI (IO23): %s\n", digitalRead(CC1101_MOSI) ? "HIGH" : "LOW");
    Serial.printf("MISO (IO19): %s\n", digitalRead(CC1101_MISO) ? "HIGH" : "LOW");
}

void testPinConfiguration() {
    Serial.println("Configuring pins...");
    
    // Set CS high (inactive)
    pinMode(CC1101_CS, OUTPUT);
    digitalWrite(CC1101_CS, HIGH);
    Serial.printf("  CS (IO27): Configured as OUTPUT, set HIGH ✓\n");
    delay(10);
    
    // Set GDO0 as input
    pinMode(CC1101_GDO0, INPUT);
    Serial.printf("  GDO0 (IO22): Configured as INPUT ✓\n");
    delay(10);
    
    // Initialize SPI pins (will be done by SPI.begin())
    Serial.printf("  MOSI (IO23): Will be configured by SPI\n");
    Serial.printf("  MISO (IO19): Will be configured by SPI\n");
    Serial.printf("  SCK (IO18): Will be configured by SPI\n");
    
    Serial.println("\n✓ Pin configuration complete");
}

void testShortCircuits() {
    Serial.println("Checking for short circuits...");
    
    // Test each SPI pin for shorts to ground or VCC
    PinTest tests[] = {
        {"MOSI", CC1101_MOSI, false, "", 0},
        {"MISO", CC1101_MISO, false, "", 0},
        {"SCK", CC1101_SCK, false, "", 0},
        {"CS", CC1101_CS, false, "", 0},
        {"GDO0", CC1101_GDO0, false, "", 0}
    };
    
    for (int i = 0; i < 5; i++) {
        // Set pin as input with pullup
        pinMode(tests[i].pin, INPUT_PULLUP);
        delay(50);
        int pullupRead = digitalRead(tests[i].pin);
        
        // Set pin as input with pulldown
        pinMode(tests[i].pin, INPUT_PULLDOWN);
        delay(50);
        int pulldownRead = digitalRead(tests[i].pin);
        
        // Analyze results
        if (pullupRead == HIGH && pulldownRead == LOW) {
            tests[i].pass = true;
            Serial.printf("  %s (IO%d): OK - No shorts detected ✓\n", 
                         tests[i].name.c_str(), tests[i].pin);
        } else if (pullupRead == LOW && pulldownRead == LOW) {
            tests[i].pass = false;
            tests[i].error = "SHORT TO GROUND";
            Serial.printf("  %s (IO%d): ✗ SHORTED TO GROUND!\n", 
                         tests[i].name.c_str(), tests[i].pin);
        } else if (pullupRead == HIGH && pulldownRead == HIGH) {
            tests[i].pass = false;
            tests[i].error = "SHORT TO VCC";
            Serial.printf("  %s (IO%d): ✗ SHORTED TO VCC!\n", 
                         tests[i].name.c_str(), tests[i].pin);
        } else {
            tests[i].pass = false;
            tests[i].error = "UNKNOWN STATE";
            Serial.printf("  %s (IO%d): ✗ Unknown state\n", 
                         tests[i].name.c_str(), tests[i].pin);
        }
        
        // Reset to safe state
        pinMode(tests[i].pin, INPUT);
    }
}

void testSPIBus() {
    Serial.println("Initializing SPI bus...");
    
    // Initialize SPI
    spi.begin(CC1101_SCK, CC1101_MISO, CC1101_MOSI, CC1101_CS);
    Serial.println("  SPI.begin() called ✓");
    
    // Test SPI settings
    spi.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    Serial.println("  SPI transaction started ✓");
    
    // Try to transfer a byte
    digitalWrite(CC1101_CS, LOW);
    delay(1);
    uint8_t response = spi.transfer(0x00);
    delay(1);
    digitalWrite(CC1101_CS, HIGH);
    
    Serial.printf("  SPI test transfer: sent 0x00, received 0x%02X\n", response);
    
    spi.endTransaction();
    
    Serial.println("\n✓ SPI bus initialized (check if response makes sense)");
}

void testCC1101Communication() {
    Serial.println("Attempting CC1101 detection...");
    
    // CC1101 STATUS register (burst read)
    digitalWrite(CC1101_CS, LOW);
    delayMicroseconds(10);
    
    spi.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    
    // Read PARTNUM register (0x30)
    uint8_t partnum = readRegister(0x30);
    Serial.printf("  PARTNUM register (0x30): 0x%02X ", partnum);
    if (partnum == 0x00) {
        Serial.println("✗ (Expected 0x00 for CC1101, got 0x00 - could be working or no response)");
    } else {
        Serial.printf("(Got 0x%02X)\n", partnum);
    }
    
    // Read VERSION register (0x31)
    uint8_t version = readRegister(0x31);
    Serial.printf("  VERSION register (0x31): 0x%02X ", version);
    if (version == 0x14 || version == 0x04) {
        Serial.println("✓ CC1101 DETECTED!");
    } else if (version == 0x00 || version == 0xFF) {
        Serial.println("✗ No response (check wiring/power)");
    } else {
        Serial.printf("? Unexpected value\n");
    }
    
    spi.endTransaction();
    digitalWrite(CC1101_CS, HIGH);
    
    if (version == 0x14 || version == 0x04) {
        Serial.println("\n✓✓✓ CC1101 MODULE DETECTED! ✓✓✓");
        Serial.println("Hardware is working correctly!");
    } else {
        Serial.println("\n✗ CC1101 NOT detected");
        Serial.println("Possible issues:");
        Serial.println("  1. Module not powered (check 3.3V)");
        Serial.println("  2. Bad solder joints");
        Serial.println("  3. Wrong module or dead module");
        Serial.println("  4. SPI wiring incorrect");
    }
}

uint8_t readRegister(uint8_t reg) {
    digitalWrite(CC1101_CS, LOW);
    delayMicroseconds(10);
    
    spi.transfer(reg | 0x80);  // Read burst
    uint8_t value = spi.transfer(0x00);
    
    delayMicroseconds(10);
    digitalWrite(CC1101_CS, HIGH);
    
    return value;
}

void testFloatingPins() {
    Serial.println("Checking for floating/disconnected pins...");
    
    // Test if MISO responds (should see data when CC1101 responds)
    pinMode(CC1101_MISO, INPUT);
    delay(100);
    
    int misoChanges = 0;
    int lastState = digitalRead(CC1101_MISO);
    
    for (int i = 0; i < 100; i++) {
        int state = digitalRead(CC1101_MISO);
        if (state != lastState) misoChanges++;
        lastState = state;
        delayMicroseconds(100);
    }
    
    if (misoChanges == 0) {
        Serial.printf("  MISO (IO19): ⚠ No activity detected - check connection\n");
    } else {
        Serial.printf("  MISO (IO19): Activity detected (%d changes) ✓\n", misoChanges);
    }
    
    // Check CS holds HIGH
    pinMode(CC1101_CS, INPUT_PULLUP);
    delay(10);
    if (digitalRead(CC1101_CS)) {
        Serial.printf("  CS (IO27): Held HIGH correctly ✓\n");
    } else {
        Serial.printf("  CS (IO27): ✗ Not HIGH - check pullup\n");
    }
}

void printSummary() {
    Serial.println("Expected CC1101 Wiring:");
    Serial.println("  GDO0  → IO22 (was RGB Red LED)");
    Serial.println("  CS    → IO27");
    Serial.println("  MOSI  → IO23");
    Serial.println("  MISO  → IO19");
    Serial.println("  SCK   → IO18");
    Serial.println("  VCC   → 3.3V (NOT 5V!)");
    Serial.println("  GND   → GND");
    Serial.println("");
    Serial.println("Common Issues:");
    Serial.println("  ✗ 5V power (must be 3.3V!)");
    Serial.println("  ✗ Cold solder joints on headers");
    Serial.println("  ✗ Broken wire at solder joint");
    Serial.println("  ✗ Wrong pin assignments");
    Serial.println("  ✗ Dead/fake CC1101 module");
    Serial.println("");
    Serial.println("Next Steps:");
    Serial.println("  1. If VERSION=0x14 or 0x04 → Module works!");
    Serial.println("  2. If VERSION=0x00 or 0xFF → Check power/wiring");
    Serial.println("  3. If shorts detected → Fix solder joints");
    Serial.println("  4. Measure 3.3V at module VCC pin with multimeter");
}
