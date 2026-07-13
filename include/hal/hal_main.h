#ifndef HAL_MAIN_H
#define HAL_MAIN_H

/* ------------------------------------------------------------------ *
 *  #define HAL_MAIN_IMPLEMENTATION in exactly ONE translation unit
 *  to pull in main() and init().
 * ------------------------------------------------------------------ */
#ifdef HAL_MAIN_IMPLEMENTATION

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

#endif /* HAL_MAIN_IMPLEMENTATION */

#endif /* HAL_MAIN_H */
