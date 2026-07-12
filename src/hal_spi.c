#include "Arduino.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

void SPI_begin(void) {
#if defined(SSPCON1)
    uint8_t p;
    p = pinToPort(22);
    volatile uint8_t *tris;
    switch (p) {
        case 0: tris = (volatile uint8_t *)&TRISA; break;
        case 1: tris = (volatile uint8_t *)&TRISB; break;
        case 2: tris = (volatile uint8_t *)&TRISC; break;
        case 3: tris = (volatile uint8_t *)&TRISD; break;
        case 4: tris = (volatile uint8_t *)&TRISE; break;
        default: return;
    }
    *tris &= (uint8_t)~pinToMask(22);
    *tris &= (uint8_t)~pinToMask(21);
    *tris |= pinToMask(20);
    *tris &= (uint8_t)~pinToMask(19);

    SSPSTAT = 0x40;
    SSPCON1 = 0x20;
    SSPCON1 |= 0x10;
#endif
}

uint8_t SPI_transfer(uint8_t data) {
#if defined(SSPCON1)
    SSPBUF = data;
    while (!SSPSTATbits.BF);
    return SSPBUF;
#else
    (void)data;
    return 0;
#endif
}

void SPI_end(void) {
#if defined(SSPCON1)
    SSPCON1 &= 0xEF;
#endif
}

void SPI_setBitOrder(uint8_t order) {
#if defined(SSPSTAT)
    if (order == LSBFIRST)
        SSPSTAT |= 0x80;
    else
        SSPSTAT &= 0x7F;
#endif
}

void SPI_setDataMode(uint8_t mode) {
#if defined(SSPSTAT) && defined(SSPCON1)
    switch (mode) {
        case SPI_MODE0: SSPSTAT &= 0xBF; SSPCON1 &= 0xDF; break;
        case SPI_MODE1: SSPSTAT |= 0x40; SSPCON1 &= 0xDF; break;
        case SPI_MODE2: SSPSTAT &= 0xBF; SSPCON1 |= 0x20; break;
        case SPI_MODE3: SSPSTAT |= 0x40; SSPCON1 |= 0x20; break;
    }
#endif
}

void SPI_setClockDivider(uint8_t div) {
#if defined(SSPCON1)
    uint8_t val;
    switch (div) {
        case SPI_CLOCK_DIV4:  val = 0; break;
        case SPI_CLOCK_DIV16: val = 1; break;
        case SPI_CLOCK_DIV64: val = 2; break;
        case SPI_CLOCK_DIV2:  val = 4; break;
        case SPI_CLOCK_DIV8:  val = 5; break;
        case SPI_CLOCK_DIV32: val = 6; break;
        default:              val = 0; break;
    }
    SSPCON1 = (SSPCON1 & 0xF1) | (val << 1);
#endif
}

const hal_spi_t SPI = {
    .begin           = SPI_begin,
    .transfer        = SPI_transfer,
    .end             = SPI_end,
    .setBitOrder     = SPI_setBitOrder,
    .setDataMode     = SPI_setDataMode,
    .setClockDivider = SPI_setClockDivider,
};
