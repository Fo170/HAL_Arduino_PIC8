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

/* Arduino-compatible analog pin aliases */
#if defined(PIN_A0)
#define A0 PIN_A0
#endif
#if defined(PIN_A1)
#define A1 PIN_A1
#endif
#if defined(PIN_A2)
#define A2 PIN_A2
#endif
#if defined(PIN_A3)
#define A3 PIN_A3
#endif
#if defined(PIN_A4)
#define A4 PIN_A4
#endif
#if defined(PIN_A5)
#define A5 PIN_A5
#endif
#if defined(PIN_A6)
#define A6 PIN_A6
#endif
#if defined(PIN_A7)
#define A7 PIN_A7
#endif
#if defined(PIN_A8)
#define A8 PIN_A8
#endif
#if defined(PIN_A9)
#define A9 PIN_A9
#endif
#if defined(PIN_A10)
#define A10 PIN_A10
#endif
#if defined(PIN_A11)
#define A11 PIN_A11
#endif
#if defined(PIN_A12)
#define A12 PIN_A12
#endif
#if defined(PIN_A16)
#define A16 PIN_A16
#endif
#if defined(PIN_A17)
#define A17 PIN_A17
#endif
#if defined(PIN_A18)
#define A18 PIN_A18
#endif
#if defined(PIN_A19)
#define A19 PIN_A19
#endif
#if defined(PIN_A24)
#define A24 PIN_A24
#endif
#if defined(PIN_A25)
#define A25 PIN_A25
#endif
#if defined(PIN_A26)
#define A26 PIN_A26
#endif

#endif
