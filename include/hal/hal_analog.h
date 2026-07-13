#ifndef HAL_ANALOG_H
#define HAL_ANALOG_H

#include <stdint.h>

#define DEFAULT 0
#define EXTERNAL 1
#define INTERNAL 2

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

static inline void analogReference(uint8_t type) {
#if defined(ADCON1)
    switch (type) {
        case DEFAULT:  ADCON1 = 0x0E; break;
        case EXTERNAL: ADCON1 = 0x00; break;
        case INTERNAL: ADCON1 = 0x0F; break;
        default:       ADCON1 = 0x0E; break;
    }
#endif
}

static inline uint16_t analogRead(uint8_t pin) {
#if defined(ADCON0)
    uint8_t channel = 0;
#if defined(HAL_MCU_18F4550) || defined(HAL_MCU_18F4553)
    if (pin >= 24 && pin <= 26) channel = (uint8_t)(13 + (pin - 24));
    else if (pin >= 8 && pin <= 12) channel = (uint8_t)(12 - (pin - 8));
    else channel = pin;
#elif defined(HAL_MCU_16F887) || defined(HAL_MCU_16F886)
    if (pin >= 16) channel = (uint8_t)(10 + (pin - 16));
    else if (pin >= 8) channel = (uint8_t)(5 + (pin - 8));
    else channel = pin;
#else
    channel = pin;
#endif
    ADCON0 &= 0xC5;
    ADCON0 |= (uint8_t)((channel & 0x0F) << 2);
    ADCON0 |= 0x01;
    __delay_us(10);
    ADCON0 |= 0x04;
    while (ADCON0 & 0x04);
    uint16_t result = (uint16_t)(((uint16_t)ADRESH << 8) | ADRESL);
    ADCON0 &= (uint8_t)~0x01;
    return result;
#else
    (void)pin;
    return 0;
#endif
}

static inline void analogWrite(uint8_t pin, uint8_t val) {
    pwmWrite(pin, val);
}

#endif
