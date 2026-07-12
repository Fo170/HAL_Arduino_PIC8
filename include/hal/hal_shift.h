#ifndef HAL_SHIFT_H
#define HAL_SHIFT_H

#include <stdint.h>

static inline uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    uint8_t value = 0;
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(clockPin, HIGH);
        if (bitOrder == LSBFIRST)
            value |= (uint8_t)(digitalRead(dataPin) << i);
        else
            value = (uint8_t)((value << 1) | digitalRead(dataPin));
        digitalWrite(clockPin, LOW);
    }
    return value;
}

static inline void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    for (uint8_t i = 0; i < 8; i++) {
        if (bitOrder == LSBFIRST)
            digitalWrite(dataPin, (uint8_t)(val & 1u));
        else
            digitalWrite(dataPin, (uint8_t)((val >> 7) & 1u));
        val = (bitOrder == LSBFIRST) ? (uint8_t)(val >> 1) : (uint8_t)(val << 1);
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}

#endif
