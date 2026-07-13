#define HAL_CORE_IMPLEMENTATION
#define HAL_MAIN_IMPLEMENTATION
#include <xc.h>
#include "config.h"
#include <Arduino.h>

void setup(void) {
    Serial.begin(9600);
    analogReference(DEFAULT);
    Serial_println("Analog read ready!");
}

void loop(void) {
    uint16_t val = analogRead(A0);
    Serial_print("A0 = ");
    Serial_printNumber(val, DEC);
    Serial_println("");
    delay(200);
}
