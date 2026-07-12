#ifndef HAL_ANALOG_H
#define HAL_ANALOG_H

#include <stdint.h>

#define DEFAULT 0
#define EXTERNAL 1
#define INTERNAL 2

void analogReference(uint8_t type);
uint16_t analogRead(uint8_t pin);
void analogWrite(uint8_t pin, uint8_t val);

#endif
