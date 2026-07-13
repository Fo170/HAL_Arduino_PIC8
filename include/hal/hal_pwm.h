#ifndef HAL_PWM_H
#define HAL_PWM_H

#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

extern uint8_t hal_current_pr2;

static inline uint8_t pwm_pin_to_ccp(uint8_t pin) {
#if defined(HAL_MCU_16F877) || defined(HAL_MCU_16F877A) || defined(HAL_MCU_16F876) || \
    defined(HAL_MCU_16F887) || defined(HAL_MCU_16F886) || \
    defined(HAL_MCU_18F46K20) || defined(HAL_MCU_18F4685)
    if (pin == 17) return 2;
    if (pin == 18) return 1;
#elif defined(HAL_MCU_16F88)
    if (pin == 8)  return 1;
#elif defined(HAL_MCU_16F628A)
    if (pin == 8)  return 1;
    if (pin == 9)  return 2;
#elif defined(HAL_MCU_18F4550) || defined(HAL_MCU_18F4553)
    if (pin == 18) return 1;
#endif
    return 0;
}

static inline void pwm_ccp_init(uint8_t ccp) {
    switch (ccp) {
        case 1: CCP1CON = 0x0C; break;
#if defined(CCP2CON)
        case 2: CCP2CON = 0x0C; break;
#endif
    }
}

static inline void pwm_ccp_set_duty(uint8_t ccp, uint8_t duty) {
    uint16_t period = (uint16_t)4 * (uint16_t)(hal_current_pr2 + 1);
    if (period == 0) period = 1024;
    uint16_t duty_10 = ((uint16_t)duty * period) / 255;
    switch (ccp) {
        case 1:
            CCPR1L = (uint8_t)(duty_10 >> 2);
            CCP1CON = (uint8_t)((CCP1CON & 0xCF) | ((uint8_t)(duty_10 & 0x03) << 4));
            break;
#if defined(CCP2CON) && defined(CCPR2L)
        case 2:
            CCPR2L = (uint8_t)(duty_10 >> 2);
            CCP2CON = (uint8_t)((CCP2CON & 0xCF) | ((uint8_t)(duty_10 & 0x03) << 4));
            break;
#endif
    }
}

static inline void pwm_timer2_freq(unsigned long freq_hz) {
    unsigned long period_cycles;
    uint8_t t2ckps;
    uint16_t pr2;
    if (freq_hz == 0) freq_hz = 5000;
    period_cycles = _XTAL_FREQ / (4ul * freq_hz);
    if (period_cycles <= 256) {
        t2ckps = 0;
        pr2 = (uint16_t)(period_cycles - 1);
    } else if (period_cycles <= 1024) {
        t2ckps = 1;
        pr2 = (uint16_t)((period_cycles / 4) - 1);
    } else {
        t2ckps = 3;
        pr2 = (uint16_t)((period_cycles / 16) - 1);
        if (pr2 > 255) pr2 = 255;
    }
    hal_current_pr2 = (uint8_t)pr2;
    PR2 = (uint8_t)pr2;
    T2CON = (T2CON & 0xFC) | t2ckps;
    T2CON |= 0x04;
}

static inline void pwmInit(void) {
    pwm_timer2_freq(5000);
}

static inline void pwmWrite(uint8_t pin, uint8_t duty) {
    uint8_t ccp = pwm_pin_to_ccp(pin);
    if (ccp == 0) return;
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
    *tris &= (uint8_t)~m;
    pwm_ccp_init(ccp);
    pwm_ccp_set_duty(ccp, duty);
}

static inline void pwmFrequency(uint8_t pin, unsigned long freq) {
    (void)pin;
    pwm_timer2_freq(freq);
}

#endif
