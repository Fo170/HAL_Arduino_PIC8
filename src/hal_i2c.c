#include "Arduino.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

static uint8_t i2c_addr = 0;
static uint8_t i2c_rx_idx = 0;
static uint8_t i2c_rx_len = 0;

void Wire_begin(void) {
#if defined(SSPCON1) && defined(SSPCON2)
    uint8_t p;

    p = pinToPort(21);
    volatile uint8_t *tris;
    switch (p) {
        case 0: tris = (volatile uint8_t *)&TRISA; break;
        case 1: tris = (volatile uint8_t *)&TRISB; break;
        case 2: tris = (volatile uint8_t *)&TRISC; break;
        case 3: tris = (volatile uint8_t *)&TRISD; break;
        case 4: tris = (volatile uint8_t *)&TRISE; break;
        default: return;
    }
    *tris |= pinToMask(21);
    *tris |= pinToMask(20);

    SSPADD = (uint8_t)(_XTAL_FREQ / (4ul * 100000) - 1);
    SSPSTAT = 0x80;
    SSPCON1 = 0x28;
    SSPCON2 = 0x00;
#endif
}

void Wire_beginTransmission(uint8_t addr) {
    i2c_addr = addr;
}

uint8_t Wire_endTransmission(uint8_t stop) {
#if defined(SSPCON2)
    SSPCON2bits.SEN = 1;
    while (SSPCON2bits.SEN);

    SSPBUF = (uint8_t)(i2c_addr << 1);
    while (SSPSTATbits.BF || SSPSTATbits.R_NOT_W);
    if (SSPCON2bits.ACKSTAT) return 2;

    while (SSPCON2bits.SEN);

    if (stop) {
        SSPCON2bits.PEN = 1;
        while (SSPCON2bits.PEN);
    }
#endif
    (void)stop;
    return 0;
}

size_t Wire_write(const uint8_t *data, size_t len) {
#if defined(SSPCON2)
    uint8_t ack = 1;
    for (size_t i = 0; i < len && ack; i++) {
        SSPBUF = data[i];
        while (SSPSTATbits.BF || SSPSTATbits.R_NOT_W);
        ack = (uint8_t)(!SSPCON2bits.ACKSTAT);
    }
    if (!ack) return len;
#endif
    (void)data;
    return 0;
}

uint8_t Wire_requestFrom(uint8_t addr, uint8_t len) {
#if defined(SSPCON2)
    i2c_rx_idx = 0;
    i2c_rx_len = len;

    SSPCON2bits.SEN = 1;
    while (SSPCON2bits.SEN);

    SSPBUF = (uint8_t)((addr << 1) | 1);
    while (SSPSTATbits.BF || SSPSTATbits.R_NOT_W);
    if (SSPCON2bits.ACKSTAT) return 0;

    for (uint8_t i = 0; i < len; i++) {
        if (i == len - 1)
            SSPCON2bits.ACKDT = 1;
        RCEN = 1;
        while (SSPSTATbits.BF == 0);
        if (i == len - 1) {
            SSPCON2bits.ACKEN = 1;
            while (SSPCON2bits.ACKEN);
        }
    }

    SSPCON2bits.PEN = 1;
    while (SSPCON2bits.PEN);
#endif
    (void)addr;
    return len;
}

int Wire_read(void) {
    return -1;
}

uint8_t Wire_available(void) {
    return 0;
}

const hal_wire_t Wire = {
    .begin            = Wire_begin,
    .beginTransmission = Wire_beginTransmission,
    .endTransmission  = Wire_endTransmission,
    .write            = Wire_write,
    .requestFrom      = Wire_requestFrom,
    .read             = Wire_read,
    .available        = Wire_available,
};
