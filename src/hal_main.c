#include "Arduino.h"

extern volatile unsigned long hal_timer1_millis;

extern void setup(void);
extern void loop(void);

void init(void) {
    hal_timer1_millis = 0;
    T1CON = 0x01;
    TMR1H = 0xEC;
    TMR1L = 0x78;
    TMR1IF = 0;
    TMR1IE = 1;
    PEIE = 1;
    GIE = 1;
#if defined(ADCON1)
    ADCON1 = 0x0F;
#endif
}

void main(void) {
    init();
    setup();
    while (1)
        loop();
}
