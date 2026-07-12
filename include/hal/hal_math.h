#ifndef HAL_MATH_H
#define HAL_MATH_H

static inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (long)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

#endif
