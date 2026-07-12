#ifndef HAL_PINS_H
#define HAL_PINS_H

#define pinToPort(p) ((p) >> 3)
#define pinToBit(p)  ((p) & 0x07)
#define pinToMask(p) ((uint8_t)(1u << pinToBit(p)))

#if defined(HAL_MCU_16F877) || defined(HAL_MCU_16F877A) || defined(HAL_MCU_16F876) || \
    defined(HAL_MCU_16F887) || defined(HAL_MCU_16F88) || defined(HAL_MCU_16F628A) || \
    defined(HAL_MCU_16F886)
    #define LED_BUILTIN  8
#elif defined(HAL_MCU_18F4550) || defined(HAL_MCU_18F4553)
    #define LED_BUILTIN  24
#elif defined(HAL_MCU_18F46K20) || defined(HAL_MCU_18F4685)
    #define LED_BUILTIN  8
#else
    #define LED_BUILTIN  8
#endif

#if defined(HAL_MCU_16F877) || defined(HAL_MCU_16F877A) || defined(HAL_MCU_16F876) || \
    defined(HAL_MCU_16F887) || defined(HAL_MCU_18F4550) || defined(HAL_MCU_18F4553) || \
    defined(HAL_MCU_18F46K20) || defined(HAL_MCU_18F4685)
    #define PIN_SERIAL_TX  22
    #define PIN_SERIAL_RX  23
#elif defined(HAL_MCU_16F88)
    #define PIN_SERIAL_TX  22
    #define PIN_SERIAL_RX  23
#elif defined(HAL_MCU_16F628A)
    #define PIN_SERIAL_TX  11
    #define PIN_SERIAL_RX  10
#else
    #define PIN_SERIAL_TX  22
    #define PIN_SERIAL_RX  23
#endif

#if defined(HAL_MCU_18F4550) || defined(HAL_MCU_18F4553)
    #define NUM_ANALOG_PINS  13
    #define PIN_A0   0
    #define PIN_A1   1
    #define PIN_A2   2
    #define PIN_A3   3
    #define PIN_A5   5
    #define PIN_A8   8
    #define PIN_A9   9
    #define PIN_A10 10
    #define PIN_A11 11
    #define PIN_A12 12
    #define PIN_A24 24
    #define PIN_A25 25
    #define PIN_A26 26
#elif defined(HAL_MCU_16F877) || defined(HAL_MCU_16F877A) || defined(HAL_MCU_16F876)
    #define NUM_ANALOG_PINS  8
    #define PIN_A0   0
    #define PIN_A1   1
    #define PIN_A2   2
    #define PIN_A3   3
    #define PIN_A5   5
    #define PIN_A8   8
    #define PIN_A9   9
    #define PIN_A10 10
#elif defined(HAL_MCU_16F887) || defined(HAL_MCU_16F886)
    #define NUM_ANALOG_PINS  14
    #define PIN_A0   0
    #define PIN_A1   1
    #define PIN_A2   2
    #define PIN_A3   3
    #define PIN_A5   5
    #define PIN_A8   8
    #define PIN_A9   9
    #define PIN_A10 10
    #define PIN_A11 11
    #define PIN_A12 12
    #define PIN_A16 16
    #define PIN_A17 17
    #define PIN_A18 18
    #define PIN_A19 19
#elif defined(HAL_MCU_16F88)
    #define NUM_ANALOG_PINS  7
    #define PIN_A0   0
    #define PIN_A1   1
    #define PIN_A2   2
    #define PIN_A3   3
    #define PIN_A5   5
    #define PIN_A8   8
    #define PIN_A9   9
#else
    #define NUM_ANALOG_PINS  0
#endif

#endif
