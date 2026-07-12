#ifndef HAL_SPI_H
#define HAL_SPI_H

#include <stdint.h>
#include <stddef.h>

#define SPI_CLOCK_DIV2   2
#define SPI_CLOCK_DIV4   4
#define SPI_CLOCK_DIV8   8
#define SPI_CLOCK_DIV16  16
#define SPI_CLOCK_DIV32  32
#define SPI_CLOCK_DIV64  64
#define SPI_CLOCK_DIV128 128

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

typedef struct {
    void (*begin)(void);
    uint8_t (*transfer)(uint8_t data);
    void (*end)(void);
    void (*setBitOrder)(uint8_t order);
    void (*setDataMode)(uint8_t mode);
    void (*setClockDivider)(uint8_t div);
} hal_spi_t;

extern const hal_spi_t SPI;

void SPI_begin(void);
uint8_t SPI_transfer(uint8_t data);
void SPI_end(void);
void SPI_setBitOrder(uint8_t order);
void SPI_setDataMode(uint8_t mode);
void SPI_setClockDivider(uint8_t div);

#endif
