// DEBUG WRAPPER - Add comprehensive debug prints to main.cpp

// This file should be #include'd into main.cpp to add debug statements

// Add after Serial.begin(115200) in setup():
void debug_setup_start() {
    Serial.println("\n\n========================================");
    Serial.println("[DEBUG] setup() STARTED");
    Serial.println("========================================");
    delay(100);
}

// Add after setup_gpio():
void debug_after_setup_gpio() {
    Serial.println("[DEBUG] setup_gpio() COMPLETED");
    Serial.flush();
    delay(100);
}

// Add after tft.init():
void debug_after_tft_init() {
    Serial.println("[DEBUG] tft.init() COMPLETED");
    Serial.flush();
    delay(100);
}

// Add after "Booting" display:
void debug_after_boot_text() {
    Serial.println("[DEBUG] 'Booting' text displayed");
    Serial.flush();
    delay(100);
}

// Add after begin_storage():
void debug_after_storage() {
    Serial.println("[DEBUG] begin_storage() COMPLETED");
    Serial.flush();
    delay(100);
}

// Add after begin_tft():
void debug_after_begin_tft() {
    Serial.println("[DEBUG] begin_tft() COMPLETED");
    Serial.flush();
    delay(100);
}

// Add after init_clock():
void debug_after_init_clock() {
    Serial.println("[DEBUG] init_clock() COMPLETED");
    Serial.flush();
    delay(100);
}

// Add after init_led():
void debug_after_init_led() {
    Serial.println("[DEBUG] init_led() COMPLETED");
    Serial.flush();
    delay(100);
}

// Add after _post_setup_gpio():
void debug_after_post_setup() {
    Serial.println("[DEBUG] _post_setup_gpio() COMPLETED");
    Serial.flush();
    delay(100);
}
