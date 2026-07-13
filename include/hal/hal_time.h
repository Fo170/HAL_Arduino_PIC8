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

extern volatile unsigned long hal_timer1_millis;

static inline unsigned long millis(void) {
    unsigned long m;
    GIE = 0;
    m = hal_timer1_millis;
    GIE = 1;
    return m;
}

static inline unsigned long micros(void) {
    unsigned long m;
    uint16_t t;
    GIE = 0;
    m = hal_timer1_millis;
    t = (uint16_t)TMR1L | ((uint16_t)TMR1H << 8);
    GIE = 1;
    uint16_t elapsed;
    if (t >= 0xEC78u)
        elapsed = t - 0xEC78u;
    else
        elapsed = t + (uint16_t)(65536u - 0xEC78u);
    return m * 1000ul + (unsigned long)(elapsed / 5u);
}

#endif
