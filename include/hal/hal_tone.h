#ifndef HAL_TONE_H
#define HAL_TONE_H

#include <stdint.h>

void tone(uint8_t pin, unsigned int frequency);
void toneDuration(uint8_t pin, unsigned int frequency, unsigned long duration);
void noTone(uint8_t pin);

#endif
