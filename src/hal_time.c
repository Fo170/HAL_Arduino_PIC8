#include "Arduino.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

volatile unsigned long hal_timer1_millis = 0;

static void hal_time_init(void) {
    hal_timer1_millis = 0;
    T1CON = 0x01;
    TMR1H = 0xEC;
    TMR1L = 0x78;
    TMR1IF = 0;
    TMR1IE = 1;
}

unsigned long millis(void) {
    unsigned long m;
    GIE = 0;
    m = hal_timer1_millis;
    GIE = 1;
    return m;
}

unsigned long micros(void) {
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
