#ifndef HAL_INTERRUPT_H
#define HAL_INTERRUPT_H

#include <stdint.h>

typedef void (*voidFuncPtr)(void);

void attachInterrupt(uint8_t pin, voidFuncPtr isr, uint8_t mode);
void detachInterrupt(uint8_t pin);

#endif
