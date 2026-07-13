#include <xc.h>

#pragma config FOSC = HSPLL_HS
#pragma config WDT = OFF
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 2
#pragma config VREGEN = ON
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config MCLRE = ON
#pragma config XINST = OFF

#define _XTAL_FREQ 48000000

void main(void) {
    TRISD = 0;
    while (1) {
        PORTD = 0xFF;
        unsigned long i;
        for(i = 0; i < 100000; i++);
        PORTD = 0x00;
        for(i = 0; i < 100000; i++);
    }
}
