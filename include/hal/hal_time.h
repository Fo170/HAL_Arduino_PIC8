#ifndef HAL_TIME_H
#define HAL_TIME_H

#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

static inline void delay(unsigned long ms) {
    for (unsigned long i = 0; i < ms; i++)
        __delay_ms(1);
}

static inline void delayMicroseconds(unsigned long us) {
    for (unsigned long i = 0; i < us; i++)
        __delay_us(1);
}

unsigned long millis(void);
unsigned long micros(void);

#endif
