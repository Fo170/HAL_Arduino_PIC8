#ifndef HAL_I2C_H
#define HAL_I2C_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    void (*begin)(void);
    void (*beginTransmission)(uint8_t addr);
    uint8_t (*endTransmission)(uint8_t stop);
    size_t (*write)(const uint8_t *data, size_t len);
    uint8_t (*requestFrom)(uint8_t addr, uint8_t len);
    int  (*read)(void);
    uint8_t (*available)(void);
} hal_wire_t;

extern const hal_wire_t Wire;

void Wire_begin(void);
void Wire_beginTransmission(uint8_t addr);
uint8_t Wire_endTransmission(uint8_t stop);
size_t Wire_write(const uint8_t *data, size_t len);
uint8_t Wire_requestFrom(uint8_t addr, uint8_t len);
int  Wire_read(void);
uint8_t Wire_available(void);

#endif
