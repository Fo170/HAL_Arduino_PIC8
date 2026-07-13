#ifndef HAL_SERIAL_H
#define HAL_SERIAL_H

#include <stdint.h>
#include <stddef.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

#ifndef SERIAL_RX_BUFFER_SIZE
#define SERIAL_RX_BUFFER_SIZE 16
#endif

typedef struct {
    void (*begin)(unsigned long baud);
    void (*end)(void);
    int  (*read)(void);
    int  (*peek)(void);
    uint8_t (*available)(void);
    void (*flush)(void);
    size_t (*write)(const uint8_t *buf, size_t len);
} hal_serial_t;

extern volatile uint8_t hal_rx_buf[];
extern volatile uint8_t hal_rx_head;
extern volatile uint8_t hal_rx_tail;

static inline void Serial_begin(unsigned long baud) {
    hal_rx_head = 0;
    hal_rx_tail = 0;
    uint16_t spbrg;
    if (baud == 0) return;
    spbrg = (uint16_t)(_XTAL_FREQ / (64ul * baud) - 1);
    if (spbrg > 255) {
        BRGH = 1;
        spbrg = (uint16_t)(_XTAL_FREQ / (16ul * baud) - 1);
    } else {
        BRGH = 0;
    }
    SPBRG = (uint8_t)(spbrg & 0xFF);
#if defined(SPBRGH)
    SPBRGH = (uint8_t)((spbrg >> 8) & 0xFF);
#endif
    TXEN = 1;
    CREN = 1;
    SPEN = 1;
    RCIE = 1;
}

static inline void Serial_end(void) {
    RCIE = 0;
    SPEN = 0;
    TXEN = 0;
    CREN = 0;
}

static inline int Serial_read(void) {
    if (hal_rx_head == hal_rx_tail) return -1;
    uint8_t c = hal_rx_buf[hal_rx_tail];
    hal_rx_tail = (uint8_t)((hal_rx_tail + 1) % SERIAL_RX_BUFFER_SIZE);
    return (int)c;
}

static inline int Serial_peek(void) {
    if (hal_rx_head == hal_rx_tail) return -1;
    return (int)hal_rx_buf[hal_rx_tail];
}

static inline uint8_t Serial_available(void) {
    return (uint8_t)((hal_rx_head - hal_rx_tail) % SERIAL_RX_BUFFER_SIZE);
}

static inline void Serial_flush(void) {
    while (!TRMT);
}

static inline void Serial_write(uint8_t c) {
    while (!TXIF);
    TXREG = c;
}

static inline size_t Serial_write_buf(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; i++)
        Serial_write(buf[i]);
    return len;
}

static inline void Serial_print(const char *str) {
    while (*str)
        Serial_write((uint8_t)*str++);
}

static inline void Serial_println(const char *str) {
    Serial_print(str);
    Serial_write('\r');
    Serial_write('\n');
}

static inline void Serial_printNumber(unsigned long n, uint8_t base) {
    char buf[33];
    char *p = buf + sizeof(buf) - 1;
    if (n == 0) {
        Serial_write('0');
        return;
    }
    *p = '\0';
    if (base == DEC) {
        while (n > 0) {
            *--p = (char)('0' + (n % 10));
            n /= 10;
        }
    } else if (base == HEX) {
        while (n > 0) {
            uint8_t d = (uint8_t)(n & 0x0F);
            *--p = (char)(d < 10 ? '0' + d : 'A' + d - 10);
            n >>= 4;
        }
    } else {
        return;
    }
    Serial_print(p);
}

static const hal_serial_t Serial = {
    .begin     = Serial_begin,
    .end       = Serial_end,
    .read      = Serial_read,
    .peek      = Serial_peek,
    .available = Serial_available,
    .flush     = Serial_flush,
    .write     = Serial_write_buf,
};

#endif
