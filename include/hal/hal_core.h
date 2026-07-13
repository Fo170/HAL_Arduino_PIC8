#ifndef HAL_CORE_H
#define HAL_CORE_H

#include <stdint.h>

typedef void (*voidFuncPtr)(void);

/* Interrupt callback pointers — defined under HAL_CORE_IMPLEMENTATION */
extern void (*hal_int0_callback)(void);
extern void (*hal_int1_callback)(void);
extern void (*hal_int2_callback)(void);
extern void (*hal_portb_change_callback)(void);

/* Shared tone state */
extern uint8_t hal_tone_pin;
extern volatile uint8_t hal_tone_state;
extern volatile uint16_t hal_tone_half_period;
extern volatile uint16_t hal_tone_count;
extern volatile uint16_t hal_tone_duration_count;

/* RX ring buffer */
extern volatile uint8_t hal_rx_buf[];
extern volatile uint8_t hal_rx_head;
extern volatile uint8_t hal_rx_tail;

/* Timer1 / random state */
extern volatile unsigned long hal_timer1_millis;
extern unsigned long hal_rand_state;

/* PWM state */
extern uint8_t hal_current_pr2;

static inline void attachInterrupt(uint8_t pin, voidFuncPtr isr, uint8_t mode) {
    GIE = 0;
    if (pin == 8) {
        INT0IF = 0;
        switch (mode) {
            case LOW:    INTEDG0 = 0; break;
            case CHANGE: INTEDG0 = 0; break;
            case RISING: INTEDG0 = 1; break;
            case FALLING:INTEDG0 = 0; break;
        }
        hal_int0_callback = isr;
        INT0IE = 1;
    }
#if defined(INT1IE)
    else if (pin == 9) {
        INT1IF = 0;
        switch (mode) {
            case RISING:  INTEDG1 = 1; break;
            case FALLING: INTEDG1 = 0; break;
        }
        hal_int1_callback = isr;
        INT1IE = 1;
    }
#endif
#if defined(INT2IE)
    else if (pin == 10) {
        INT2IF = 0;
        switch (mode) {
            case RISING:  INTEDG2 = 1; break;
            case FALLING: INTEDG2 = 0; break;
        }
        hal_int2_callback = isr;
        INT2IE = 1;
    }
#endif
    GIE = 1;
}

static inline void detachInterrupt(uint8_t pin) {
    if (pin == 8) { INT0IE = 0; hal_int0_callback = 0; }
#if defined(INT1IE)
    else if (pin == 9) { INT1IE = 0; hal_int1_callback = 0; }
#endif
#if defined(INT2IE)
    else if (pin == 10) { INT2IE = 0; hal_int2_callback = 0; }
#endif
}

/* ------------------------------------------------------------------ *
 *  Singleton definitions
 *  #define HAL_CORE_IMPLEMENTATION in exactly ONE translation unit
 *  to pull in the ISR, global variables, and tone state.
 * ------------------------------------------------------------------ */
#ifdef HAL_CORE_IMPLEMENTATION

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

#define RX_BUF_SIZE SERIAL_RX_BUFFER_SIZE

volatile unsigned long hal_timer1_millis = 0;

volatile uint8_t hal_rx_buf[RX_BUF_SIZE];
volatile uint8_t hal_rx_head = 0;
volatile uint8_t hal_rx_tail = 0;

void (*hal_int0_callback)(void) = 0;
void (*hal_int1_callback)(void) = 0;
void (*hal_int2_callback)(void) = 0;
void (*hal_portb_change_callback)(void) = 0;

uint8_t hal_tone_pin = 0;
volatile uint8_t hal_tone_state = 0;
volatile uint16_t hal_tone_half_period = 0;
volatile uint16_t hal_tone_count = 0;
volatile uint16_t hal_tone_duration_count = 0;

unsigned long hal_rand_state = 0;

uint8_t hal_current_pr2 = 0;

/* I2C state */
uint8_t hal_i2c_addr = 0;
uint8_t hal_i2c_rx_idx = 0;
uint8_t hal_i2c_rx_len = 0;

/* ------------------------------------------------------------------ */
/*  ISR
 * ------------------------------------------------------------------ */
#if defined(HAL_FAMILY_PIC18)

void __interrupt() hal_isr_high(void) {
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;
        TMR1H = 0xEC;
        TMR1L = 0x78;
        hal_timer1_millis++;
    }
    if (PIR1bits.RCIF) {
        uint8_t c = RCREG;
        uint8_t next = (uint8_t)((hal_rx_head + 1) % RX_BUF_SIZE);
        if (next != hal_rx_tail) {
            hal_rx_buf[hal_rx_head] = c;
            hal_rx_head = next;
        }
    }
    if (INTCONbits.INT0IF) {
        INTCONbits.INT0IF = 0;
        if (hal_int0_callback) hal_int0_callback();
    }
#if defined(INTCON3bits)
    if (INTCON3bits.INT1IF) {
        INTCON3bits.INT1IF = 0;
        if (hal_int1_callback) hal_int1_callback();
    }
    if (INTCON3bits.INT2IF) {
        INTCON3bits.INT2IF = 0;
        if (hal_int2_callback) hal_int2_callback();
    }
#endif
    if (INTCONbits.RBIF) {
        INTCONbits.RBIF = 0;
        if (hal_portb_change_callback) hal_portb_change_callback();
    }
    if (PIR1bits.TMR2IF) {
        PIR1bits.TMR2IF = 0;
        if (hal_tone_half_period && hal_tone_duration_count) {
            hal_tone_count++;
            if (hal_tone_count >= hal_tone_half_period) {
                hal_tone_count = 0;
                hal_tone_state ^= 1;
                uint8_t p = pinToPort(hal_tone_pin);
                uint8_t m = pinToMask(hal_tone_pin);
                volatile uint8_t *out;
                switch (p) {
                    #ifdef USE_LATCH
                    case 0: out = (volatile uint8_t *)&LATA; break;
                    case 1: out = (volatile uint8_t *)&LATB; break;
                    case 2: out = (volatile uint8_t *)&LATC; break;
                    case 3: out = (volatile uint8_t *)&LATD; break;
                    case 4: out = (volatile uint8_t *)&LATE; break;
                    #else
                    case 0: out = (volatile uint8_t *)&PORTA; break;
                    case 1: out = (volatile uint8_t *)&PORTB; break;
                    case 2: out = (volatile uint8_t *)&PORTC; break;
                    case 3: out = (volatile uint8_t *)&PORTD; break;
                    case 4: out = (volatile uint8_t *)&PORTE; break;
                    #endif
                    default: return;
                }
                if (hal_tone_state) *out |= m; else *out &= (uint8_t)~m;
                if (hal_tone_duration_count < 0xFFFF) hal_tone_duration_count--;
            }
        }
        if (hal_tone_duration_count == 0 && hal_tone_half_period) {
            uint8_t p = pinToPort(hal_tone_pin);
            uint8_t m = pinToMask(hal_tone_pin);
            volatile uint8_t *out;
            switch (p) {
                #ifdef USE_LATCH
                case 0: out = (volatile uint8_t *)&LATA; break;
                case 1: out = (volatile uint8_t *)&LATB; break;
                case 2: out = (volatile uint8_t *)&LATC; break;
                case 3: out = (volatile uint8_t *)&LATD; break;
                case 4: out = (volatile uint8_t *)&LATE; break;
                #else
                case 0: out = (volatile uint8_t *)&PORTA; break;
                case 1: out = (volatile uint8_t *)&PORTB; break;
                case 2: out = (volatile uint8_t *)&PORTC; break;
                case 3: out = (volatile uint8_t *)&PORTD; break;
                case 4: out = (volatile uint8_t *)&PORTE; break;
                #endif
                default: return;
            }
            *out &= (uint8_t)~m;
            hal_tone_half_period = 0;
        }
    }
}

#else /* HAL_FAMILY_PIC16 */

void __interrupt() hal_isr(void) {
    if (TMR1IF) {
        TMR1IF = 0;
        TMR1H = 0xEC;
        TMR1L = 0x78;
        hal_timer1_millis++;
    }
    if (RCIF) {
        uint8_t c = RCREG;
        uint8_t next = (uint8_t)((hal_rx_head + 1) % RX_BUF_SIZE);
        if (next != hal_rx_tail) {
            hal_rx_buf[hal_rx_head] = c;
            hal_rx_head = next;
        }
    }
    if (INT0IF) {
        INT0IF = 0;
        if (hal_int0_callback) hal_int0_callback();
    }
    if (RBIF) {
        RBIF = 0;
        if (hal_portb_change_callback) hal_portb_change_callback();
    }
    if (TMR2IF) {
        TMR2IF = 0;
        if (hal_tone_half_period && hal_tone_duration_count) {
            hal_tone_count++;
            if (hal_tone_count >= hal_tone_half_period) {
                hal_tone_count = 0;
                hal_tone_state ^= 1;
                uint8_t p = pinToPort(hal_tone_pin);
                uint8_t m = pinToMask(hal_tone_pin);
                volatile uint8_t *out;
                switch (p) {
                    #ifdef USE_LATCH
                    case 0: out = (volatile uint8_t *)&LATA; break;
                    case 1: out = (volatile uint8_t *)&LATB; break;
                    case 2: out = (volatile uint8_t *)&LATC; break;
                    case 3: out = (volatile uint8_t *)&LATD; break;
                    case 4: out = (volatile uint8_t *)&LATE; break;
                    #else
                    case 0: out = (volatile uint8_t *)&PORTA; break;
                    case 1: out = (volatile uint8_t *)&PORTB; break;
                    case 2: out = (volatile uint8_t *)&PORTC; break;
                    case 3: out = (volatile uint8_t *)&PORTD; break;
                    case 4: out = (volatile uint8_t *)&PORTE; break;
                    #endif
                    default: return;
                }
                if (hal_tone_state) *out |= m; else *out &= (uint8_t)~m;
                if (hal_tone_duration_count < 0xFFFF) hal_tone_duration_count--;
            }
        }
        if (hal_tone_duration_count == 0 && hal_tone_half_period) {
            uint8_t p = pinToPort(hal_tone_pin);
            uint8_t m = pinToMask(hal_tone_pin);
            volatile uint8_t *out;
            switch (p) {
                #ifdef USE_LATCH
                case 0: out = (volatile uint8_t *)&LATA; break;
                case 1: out = (volatile uint8_t *)&LATB; break;
                case 2: out = (volatile uint8_t *)&LATC; break;
                case 3: out = (volatile uint8_t *)&LATD; break;
                case 4: out = (volatile uint8_t *)&LATE; break;
                #else
                case 0: out = (volatile uint8_t *)&PORTA; break;
                case 1: out = (volatile uint8_t *)&PORTB; break;
                case 2: out = (volatile uint8_t *)&PORTC; break;
                case 3: out = (volatile uint8_t *)&PORTD; break;
                case 4: out = (volatile uint8_t *)&PORTE; break;
                #endif
                default: return;
            }
            *out &= (uint8_t)~m;
            hal_tone_half_period = 0;
        }
    }
}

#endif /* HAL_FAMILY_PIC18 */

#endif /* HAL_CORE_IMPLEMENTATION */

#endif /* HAL_CORE_H */
