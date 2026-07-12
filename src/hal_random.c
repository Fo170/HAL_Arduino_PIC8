#include "Arduino.h"

static unsigned long rand_state = 1;

void randomSeed(unsigned long seed) {
    rand_state = seed ? seed : 1;
}

long random(long max) {
    if (max == 0) return 0;
    rand_state = rand_state * 1103515245ul + 12345ul;
    return (long)(rand_state % (unsigned long)max);
}

long randomRange(long min, long max) {
    if (min >= max) return min;
    return min + random(max - min);
}
