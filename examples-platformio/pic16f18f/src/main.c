#include <xc.h>

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 20000000

void main(void) {
    TRISB = 0;
    while (1) {
        PORTB = 0xFF;
        __delay_ms(500);
        PORTB = 0x00;
        __delay_ms(500);
    }
}
