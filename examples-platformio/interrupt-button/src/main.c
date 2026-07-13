#define HAL_CORE_IMPLEMENTATION
#define HAL_MAIN_IMPLEMENTATION
#include <xc.h>
#include "config.h"
#include <Arduino.h>

#define BTN_PIN  8
#define LED_PIN 18

static volatile uint8_t btn_flag = 0;

static void btn_isr(void) {
    btn_flag = 1;
}

void setup(void) {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BTN_PIN, INPUT_PULLUP);
    attachInterrupt(BTN_PIN, btn_isr, FALLING);
}

void loop(void) {
    if (btn_flag) {
        btn_flag = 0;
        togglePin(LED_PIN);
    }
}
