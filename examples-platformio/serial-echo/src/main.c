#define HAL_CORE_IMPLEMENTATION
#define HAL_MAIN_IMPLEMENTATION
#include <xc.h>
#include "config.h"
#include <Arduino.h>

void setup(void) {
    Serial.begin(9600);
    Serial.println("Serial echo ready!");
}

void loop(void) {
    if (Serial.available()) {
        int c = Serial.read();
        if (c >= 0) {
            Serial.write((uint8_t)c);
            if (c == '\r')
                Serial.write('\n');
        }
    }
}
