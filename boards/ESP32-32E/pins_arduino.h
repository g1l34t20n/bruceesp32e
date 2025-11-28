#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

// UART pins
static const uint8_t TX = 1;
static const uint8_t RX = 3;

// I2C pins (can be used for other purposes)
static const uint8_t SDA = 27;
static const uint8_t SCL = 4;

// SPI pins for SD Card and SPI peripheral
static const uint8_t SS = 5;      // SD Card CS
static const uint8_t MOSI = 23;
static const uint8_t MISO = 19;
static const uint8_t SCK = 18;

// ADC pins
static const uint8_t A0 = 36;   // Touch interrupt
static const uint8_t A3 = 39;   // Touch MISO
static const uint8_t A4 = 32;   // Touch MOSI
static const uint8_t A5 = 33;   // Touch CS
static const uint8_t A6 = 34;   // Battery ADC
static const uint8_t A7 = 35;   // NC (Input only)
static const uint8_t A10 = 4;
static const uint8_t A11 = 0;   // BOOT button
static const uint8_t A12 = 2;   // TFT DC
static const uint8_t A13 = 15;  // TFT CS
static const uint8_t A14 = 13;  // TFT MOSI
static const uint8_t A15 = 12;  // TFT MISO
static const uint8_t A16 = 14;  // TFT SCK
static const uint8_t A17 = 27;  // SPI SS / I2C SDA
static const uint8_t A18 = 25;  // Touch SCK
static const uint8_t A19 = 26;  // Audio DAC

// Touch pins
static const uint8_t T0 = 4;
static const uint8_t T1 = 0;
static const uint8_t T2 = 2;
static const uint8_t T3 = 15;
static const uint8_t T4 = 13;
static const uint8_t T5 = 12;
static const uint8_t T6 = 14;
static const uint8_t T7 = 27;
static const uint8_t T8 = 33;
static const uint8_t T9 = 32;

// DAC pins
static const uint8_t DAC1 = 25;  // Not used
static const uint8_t DAC2 = 26;  // Audio output

// Deep sleep - wake on touch interrupt
#define DEEPSLEEP_WAKEUP_PIN 36 // Touch interrupt (XPT2046 IRQ)
#define DEEPSLEEP_PIN_ACT LOW

#endif /* Pins_Arduino_h */
