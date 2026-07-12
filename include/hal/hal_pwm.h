#ifndef HAL_PWM_H
#define HAL_PWM_H

#include <stdint.h>

void pwmInit(void);
void pwmWrite(uint8_t pin, uint8_t duty);
void pwmFrequency(uint8_t pin, unsigned long freq);

#endif
