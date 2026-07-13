#define HAL_CORE_IMPLEMENTATION
#define HAL_MAIN_IMPLEMENTATION
#include <xc.h>
#include "config.h"
#include <Arduino.h>

#define PWM_PIN 18

void setup(void) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    pwmInit();
    pwmFrequency(PWM_PIN, 5000);
}

void loop(void) {
    static uint8_t brightness = 0;
    static int8_t dir = 1;

    pwmWrite(PWM_PIN, brightness);
    brightness = (uint8_t)((int)brightness + dir);
    if (brightness == 0 || brightness == 255)
        dir = -dir;
    delay(5);
}
