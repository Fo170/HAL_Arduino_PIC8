#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "hal_pins.h"

static inline void pinMode(uint8_t pin, uint8_t mode) {
    uint8_t p = pinToPort(pin);
    uint8_t m = pinToMask(pin);
    volatile uint8_t *tris;
    switch (p) {
        case 0: tris = (volatile uint8_t *)&TRISA; break;
        case 1: tris = (volatile uint8_t *)&TRISB; break;
        case 2: tris = (volatile uint8_t *)&TRISC; break;
        case 3: tris = (volatile uint8_t *)&TRISD; break;
        case 4: tris = (volatile uint8_t *)&TRISE; break;
        default: return;
    }
    if (mode == OUTPUT)
        *tris &= (uint8_t)~m;
    else
        *tris |= m;
    if (mode == INPUT_PULLUP) {
        #ifdef OPTION_REG
            OPTION_REG &= (uint8_t)~0x80;
        #endif
    }
}

static inline void digitalWrite(uint8_t pin, uint8_t val) {
    uint8_t p = pinToPort(pin);
    uint8_t m = pinToMask(pin);
    volatile uint8_t *out;
    switch (p) {
        #ifdef USE_LATCH
        case 0: out = (volatile uint8_t *)&LATA; break;
        case 1: out = (volatile uint8_t *)&LATB; break;
        case 2: out = (volatile uint8_t *)&LATC; break;
        case 3: out = (volatile uint8_t *)&LATD; break;
        case 4: out = (volatile uint8_t *)&LATE; break;
        #else
        case 0: out = (volatile uint8_t *)&PORTA; break;
        case 1: out = (volatile uint8_t *)&PORTB; break;
        case 2: out = (volatile uint8_t *)&PORTC; break;
        case 3: out = (volatile uint8_t *)&PORTD; break;
        case 4: out = (volatile uint8_t *)&PORTE; break;
        #endif
        default: return;
    }
    if (val) *out |= m;
    else     *out &= (uint8_t)~m;
}

static inline uint8_t digitalRead(uint8_t pin) {
    uint8_t p = pinToPort(pin);
    uint8_t b = pinToBit(pin);
    volatile uint8_t *in;
    switch (p) {
        case 0: in = (volatile uint8_t *)&PORTA; break;
        case 1: in = (volatile uint8_t *)&PORTB; break;
        case 2: in = (volatile uint8_t *)&PORTC; break;
        case 3: in = (volatile uint8_t *)&PORTD; break;
        case 4: in = (volatile uint8_t *)&PORTE; break;
        default: return 0;
    }
    return (uint8_t)((*in >> b) & 1u);
}

static inline void togglePin(uint8_t pin) {
    uint8_t p = pinToPort(pin);
    uint8_t m = pinToMask(pin);
    volatile uint8_t *out;
    switch (p) {
        #ifdef USE_LATCH
        case 0: out = (volatile uint8_t *)&LATA; break;
        case 1: out = (volatile uint8_t *)&LATB; break;
        case 2: out = (volatile uint8_t *)&LATC; break;
        case 3: out = (volatile uint8_t *)&LATD; break;
        case 4: out = (volatile uint8_t *)&LATE; break;
        #else
        case 0: out = (volatile uint8_t *)&PORTA; break;
        case 1: out = (volatile uint8_t *)&PORTB; break;
        case 2: out = (volatile uint8_t *)&PORTC; break;
        case 3: out = (volatile uint8_t *)&PORTD; break;
        case 4: out = (volatile uint8_t *)&PORTE; break;
        #endif
        default: return;
    }
    *out ^= m;
}

#endif
