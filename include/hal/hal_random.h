#ifndef HAL_RANDOM_H
#define HAL_RANDOM_H

#include <stdint.h>

void randomSeed(unsigned long seed);
long random(long max);
long randomRange(long min, long max);

#endif
