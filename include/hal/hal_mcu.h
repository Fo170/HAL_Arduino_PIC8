#ifndef HAL_MCU_H
#define HAL_MCU_H

#if defined(_PIC18)
    #define HAL_FAMILY_PIC18
    #define USE_LATCH
#elif defined(_PIC14) || defined(_PIC14E)
    #define HAL_FAMILY_PIC16
#else
    #error "HAL: unrecognised MCU family (not _PIC14/_PIC14E/_PIC18)"
#endif

#if defined(__16F876)
    #define HAL_MCU "PIC16F876"
    #define HAL_MCU_16F876
#elif defined(__16F877)
    #define HAL_MCU "PIC16F877"
    #define HAL_MCU_16F877
#elif defined(__16F877A)
    #define HAL_MCU "PIC16F877A"
    #define HAL_MCU_16F877A
#elif defined(__16F88)
    #define HAL_MCU "PIC16F88"
    #define HAL_MCU_16F88
#elif defined(__16F887)
    #define HAL_MCU "PIC16F887"
    #define HAL_MCU_16F887
#elif defined(__16F886)
    #define HAL_MCU "PIC16F886"
    #define HAL_MCU_16F886
#elif defined(__16F628A) || defined(__16F628)
    #define HAL_MCU "PIC16F628A"
    #define HAL_MCU_16F628A
#elif defined(__18F4550)
    #define HAL_MCU "PIC18F4550"
    #define HAL_MCU_18F4550
#elif defined(__18F4553)
    #define HAL_MCU "PIC18F4553"
    #define HAL_MCU_18F4553
#elif defined(__18F46K20)
    #define HAL_MCU "PIC18F46K20"
    #define HAL_MCU_18F46K20
#elif defined(__18F4685)
    #define HAL_MCU "PIC18F4685"
    #define HAL_MCU_18F4685
#else
    #define HAL_MCU "unknown"
    #warning "HAL: MCU not explicitly recognised — using generic family support"
#endif

#if defined(HAL_FAMILY_PIC18)
    #define HAL_NUM_PORTS 5
#else
    #if defined(HAL_MCU_16F88) || defined(HAL_MCU_16F628A)
        #define HAL_NUM_PORTS 3
    #else
        #define HAL_NUM_PORTS 5
    #endif
#endif

#endif
