#ifndef HAL_TONE_H
#define HAL_TONE_H

#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

extern uint8_t hal_tone_pin;
extern volatile uint8_t hal_tone_state;
extern volatile uint16_t hal_tone_half_period;
extern volatile uint16_t hal_tone_count;
extern volatile uint16_t hal_tone_duration_count;

static inline void noTone(uint8_t pin);
static inline void toneDuration(uint8_t pin, unsigned int frequency, unsigned long duration);

static inline void hal_tone_timer2_freq(unsigned int freq) {
    unsigned long period_cycles = _XTAL_FREQ / (4ul * freq);
    uint8_t t2ckps, pr2;
    if (period_cycles <= 256) {
        t2ckps = 0;
        pr2 = (uint8_t)(period_cycles - 1);
    } else if (period_cycles <= 1024) {
        t2ckps = 1;
        pr2 = (uint8_t)((period_cycles / 4) - 1);
    } else {
        t2ckps = 3;
        pr2 = (uint8_t)((period_cycles / 16) - 1);
        if (pr2 > 255) pr2 = 255;
    }
    PR2 = pr2;
    T2CON = (T2CON & 0xFC) | t2ckps;
    T2CON |= 0x04;
}

static inline void tone(uint8_t pin, unsigned int frequency) {
    toneDuration(pin, frequency, 0);
}

static inline void toneDuration(uint8_t pin, unsigned int frequency, unsigned long duration) {
    if (frequency == 0) { noTone(pin); return; }
    noTone(hal_tone_pin);
    hal_tone_pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    hal_tone_state = 0;
    hal_tone_count = 0;
    hal_tone_half_period = (uint16_t)((_XTAL_FREQ / (4ul * 2 * frequency)) / 1);
    if (duration > 0)
        hal_tone_duration_count = (uint16_t)((duration * frequency) / 1000);
    else
        hal_tone_duration_count = 0xFFFF;
    hal_tone_timer2_freq(frequency);
    TMR2IF = 0;
    TMR2IE = 1;
}

static inline void noTone(uint8_t pin) {
    if (pin == hal_tone_pin) {
        TMR2IE = 0;
        digitalWrite(hal_tone_pin, LOW);
        hal_tone_half_period = 0;
        hal_tone_pin = 0;
    }
}

#endif
