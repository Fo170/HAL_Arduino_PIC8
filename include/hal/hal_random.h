#ifndef HAL_RANDOM_H
#define HAL_RANDOM_H

#include <stdint.h>

extern unsigned long hal_rand_state;

static inline void randomSeed(unsigned long seed) {
    hal_rand_state = seed ? seed : 1;
}

static inline long random(long max) {
    if (max == 0) return 0;
    hal_rand_state = hal_rand_state * 1103515245ul + 12345ul;
    return (long)(hal_rand_state % (unsigned long)max);
}

static inline long randomRange(long min, long max) {
    if (min >= max) return min;
    return min + random(max - min);
}

#endif
