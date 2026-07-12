#ifndef ARDUINO_TYPES_H
#define ARDUINO_TYPES_H

#include <stdint.h>

typedef uint8_t  byte;
typedef uint16_t word;
typedef uint8_t  boolean;

#define HIGH      1
#define LOW       0
#define OUTPUT    0
#define INPUT     1
#define INPUT_PULLUP 2

#define true  1
#define false 0

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

#define PI 3.14159265358979323846f

#define LSBFIRST 0
#define MSBFIRST 1

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define CHANGE   1
#define FALLING  2
#define RISING   3

#ifndef NULL
#define NULL 0
#endif

#define lowByte(w)  ((uint8_t)((w) & 0xFF))
#define highByte(w) ((uint8_t)((w) >> 8))

#define bitRead(v,n)    (((v) >> (n)) & 1)
#define bitWrite(v,n,b) ((b) ? ((v) |= (1u << (n))) : ((v) &= ~(1u << (n))))
#define bitSet(v,n)     ((v) |= (1u << (n)))
#define bitClear(v,n)   ((v) &= ~(1u << (n)))
#define bit(n)          (1u << (n))

#define digitalPinToInterrupt(p) ((uint8_t)(p))

#define abs(x)   ((x) > 0 ? (x) : -(x))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define constrain(amt,l,h) ((amt) < (l) ? (l) : ((amt) > (h) ? (h) : (amt)))
#define sq(x)    ((x) * (x))

#define interrupts()   ei()
#define noInterrupts() di()

#define SERIAL_RX_BUFFER_SIZE 16

#endif
