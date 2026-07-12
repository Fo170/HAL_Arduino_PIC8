#ifndef HAL_SERIAL_H
#define HAL_SERIAL_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    void (*begin)(unsigned long baud);
    void (*end)(void);
    int  (*read)(void);
    int  (*peek)(void);
    uint8_t (*available)(void);
    void (*flush)(void);
    size_t (*write)(const uint8_t *buf, size_t len);
} hal_serial_t;

extern const hal_serial_t Serial;

void Serial_begin(unsigned long baud);
void Serial_end(void);
int  Serial_read(void);
int  Serial_peek(void);
uint8_t Serial_available(void);
void Serial_flush(void);
void Serial_write(uint8_t c);
void Serial_print(const char *str);
void Serial_println(const char *str);
void Serial_printNumber(unsigned long n, uint8_t base);

#endif
