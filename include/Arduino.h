#ifndef ARDUINO_H
#define ARDUINO_H

#include <xc.h>
#include <stdint.h>
#include <stddef.h>

/* Suppress benign warnings for static inline functions not called
 * by a specific example and indirect call patterns in ISR. */
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif

#include "Arduino_types.h"
#include "hal/hal_mcu.h"
#include "hal/hal_pins.h"
#include "hal/hal_gpio.h"
#include "hal/hal_time.h"
#include "hal/hal_serial.h"
#include "hal/hal_pwm.h"
#include "hal/hal_analog.h"
#include "hal/hal_core.h"
#include "hal/hal_tone.h"
#include "hal/hal_shift.h"
#include "hal/hal_math.h"
#include "hal/hal_random.h"
#include "hal/hal_spi.h"
#include "hal/hal_i2c.h"

void setup(void);
void loop(void);

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include "hal/hal_main.h"

#endif
