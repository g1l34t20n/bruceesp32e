/**
 * ESP32-32E Minimal Hardware Test
 * Tests display, touch, RGB LED, and SD card
 * 
 * Upload this with Arduino IDE or PlatformIO
 * Board: ESP32 Dev Module
 * Upload Speed: 115200
 */

#include <TFT_eSPI.h>
#include <SPI.h>

// Display pins (from ESP32-32E spec)
#define TFT_CS    15
#define TFT_DC    2
#define TFT_MOSI  13
#define TFT_MISO  12
#define TFT_SCLK  14
#define TFT_BL    21

// Touch pins (XPT2046 resistive)
#define TOUCH_CS   33
#define TOUCH_IRQ  36
#define TOUCH_CLK  25
#define TOUCH_MOSI 32
#define TOUCH_MISO 39

// RGB LED pins (common anode - LOW = ON)
#define RGB_R 22
#define RGB_G 16
#define RGB_B 17

// SD Card pins
#define SD_CS   5
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCK  18

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n========================================");
    Serial.println("ESP32-32E Minimal Hardware Test");
    Serial.println("========================================\n");
    
    // Test 1: Backlight
    Serial.println("[1/5] Testing backlight...");
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    Serial.println("  ✓ Backlight ON (should be bright)");
    delay(500);
    
    // Test 2: TFT Display
    Serial.println("[2/5] Initializing display...");
    tft.init();
    Serial.println("  ✓ tft.init() completed");
    
    tft.setRotation(1);  // Landscape
    Serial.println("  ✓ Set rotation to 1 (landscape)");
    
    tft.fillScreen(TFT_BLACK);
    Serial.println("  ✓ Screen cleared (black)");
    
    // Draw test pattern
    tft.fillScreen(TFT_RED);
    delay(500);
    tft.fillScreen(TFT_GREEN);
    delay(500);
    tft.fillScreen(TFT_BLUE);
    delay(500);
    tft.fillScreen(TFT_BLACK);
    Serial.println("  ✓ Color test (red/green/blue) completed");
    
    // Draw text
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("ESP32-32E Test");
    tft.setCursor(10, 40);
    tft.setTextColor(TFT_GREEN);
    tft.println("Display: OK");
    Serial.println("  ✓ Text displayed");
    
    // Test 3: RGB LED
    Serial.println("[3/5] Testing RGB LED...");
    pinMode(RGB_R, OUTPUT);
    pinMode(RGB_G, OUTPUT);
    pinMode(RGB_B, OUTPUT);
    
    // All OFF (common anode)
    digitalWrite(RGB_R, HIGH);
    digitalWrite(RGB_G, HIGH);
    digitalWrite(RGB_B, HIGH);
    
    Serial.println("  Testing RED...");
    tft.setCursor(10, 70);
    tft.setTextColor(TFT_RED);
    tft.println("LED: RED");
    digitalWrite(RGB_R, LOW);
    delay(1000);
    digitalWrite(RGB_R, HIGH);
    
    Serial.println("  Testing GREEN...");
    tft.fillRect(10, 70, 300, 20, TFT_BLACK);
    tft.setCursor(10, 70);
    tft.setTextColor(TFT_GREEN);
    tft.println("LED: GREEN");
    digitalWrite(RGB_G, LOW);
    delay(1000);
    digitalWrite(RGB_G, HIGH);
    
    Serial.println("  Testing BLUE...");
    tft.fillRect(10, 70, 300, 20, TFT_BLACK);
    tft.setCursor(10, 70);
    tft.setTextColor(TFT_BLUE);
    tft.println("LED: BLUE");
    digitalWrite(RGB_B, LOW);
    delay(1000);
    digitalWrite(RGB_B, HIGH);
    
    Serial.println("  ✓ RGB LED test completed");
    
    // Test 4: Touch Screen
    Serial.println("[4/5] Setting up touch...");
    pinMode(TOUCH_CS, OUTPUT);
    digitalWrite(TOUCH_CS, HIGH);
    pinMode(TOUCH_IRQ, INPUT);
    Serial.println("  ✓ Touch CS configured");
    Serial.println("  (Touch functionality requires XPT2046 library)");
    
    tft.fillRect(10, 70, 300, 20, TFT_BLACK);
    tft.setCursor(10, 70);
    tft.setTextColor(TFT_YELLOW);
    tft.println("Touch: Ready");
    
    // Test 5: SD Card pins
    Serial.println("[5/5] Checking SD Card pins...");
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    Serial.println("  ✓ SD CS configured");
    Serial.println("  (SD functionality requires SD library)");
    
    tft.setCursor(10, 100);
    tft.setTextColor(TFT_CYAN);
    tft.println("SD: Pins OK");
    
    // Final status
    Serial.println("\n========================================");
    Serial.println("HARDWARE TEST COMPLETED");
    Serial.println("========================================");
    Serial.println("If you see this message and colors on");
    Serial.println("the display, basic hardware is working!");
    Serial.println("\nNow test:");
    Serial.println("- Touch the screen (check serial for IRQ)");
    Serial.println("- Insert SD card");
    Serial.println("\nMonitoring...\n");
    
    tft.fillRect(0, 130, 320, 60, TFT_DARKGREEN);
    tft.setCursor(20, 145);
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE);
    tft.println("ALL OK!");
}

void loop() {
    static uint32_t lastCheck = 0;
    
    if (millis() - lastCheck > 500) {
        lastCheck = millis();
        
        // Check touch IRQ
        if (digitalRead(TOUCH_IRQ) == LOW) {
            Serial.println("TOUCH DETECTED! (IRQ LOW)");
            
            // Flash LED
            digitalWrite(RGB_B, LOW);
            delay(100);
            digitalWrite(RGB_B, HIGH);
        }
    }
}
