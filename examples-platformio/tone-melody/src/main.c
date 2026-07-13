#define HAL_CORE_IMPLEMENTATION
#define HAL_MAIN_IMPLEMENTATION
#include <xc.h>
#include "config.h"
#include <Arduino.h>

#define TONE_PIN 18

void setup(void) {
    pinMode(TONE_PIN, OUTPUT);
}

void loop(void) {
    tone(TONE_PIN, 262);
    delay(400);
    tone(TONE_PIN, 330);
    delay(400);
    tone(TONE_PIN, 392);
    delay(400);
    tone(TONE_PIN, 523);
    delay(800);
    noTone(TONE_PIN);
    delay(1000);
}
