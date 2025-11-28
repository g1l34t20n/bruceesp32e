void _post_setup_gpio() {
    Serial.println("[DEBUG] _post_setup_gpio() started");
    
#if defined(USE_TFT_eSPI_TOUCH)
    Serial.println("[DEBUG] Initializing touch...");
    pinMode(TOUCH_CS, OUTPUT);
    uint16_t calData[5];
    File caldata = LittleFS.open("/calData", "r");

    if (!caldata) {
        Serial.println("[DEBUG] No calibration found, running calibration...");
        tft.setRotation(ROTATION);
        tft.calibrateTouch(calData, TFT_WHITE, TFT_BLACK, 10);

        caldata = LittleFS.open("/calData", "w");
        if (caldata) {
            caldata.printf(
                "%d\n%d\n%d\n%d\n%d\n", calData[0], calData[1], calData[2], calData[3], calData[4]
            );
            caldata.close();
        }
    } else {
        Serial.print("[DEBUG] Loading calibration: ");
        for (int i = 0; i < 5; i++) {
            String line = caldata.readStringUntil('\n');
            calData[i] = line.toInt();
            Serial.printf("%d, ", calData[i]);
        }
        Serial.println();
        caldata.close();
    }
    tft.setTouch(calData);
    Serial.println("[DEBUG] Touch initialized");
#endif

    Serial.println("[DEBUG] Initializing LEDC for backlight PWM...");
    ledcSetup(TFT_BRIGHT_CHANNEL, TFT_BRIGHT_FREQ, TFT_BRIGHT_Bits);
    Serial.println("[DEBUG] ledcSetup done");
    
    ledcAttachPin(TFT_BL, TFT_BRIGHT_CHANNEL);
    Serial.println("[DEBUG] ledcAttachPin done");
    
    ledcWrite(TFT_BRIGHT_CHANNEL, 255);
    Serial.println("[DEBUG] ledcWrite done");
    
    ledcBrightnessInitialized = true;
    Serial.println("[DEBUG] LEDC initialized successfully");
    Serial.println("[DEBUG] _post_setup_gpio() completed");
}
