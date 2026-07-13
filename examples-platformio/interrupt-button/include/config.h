#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#if defined(__16F877)
  #pragma config FOSC = HS
  #pragma config WDTE = OFF
  #pragma config PWRTE = OFF
  #pragma config CP = OFF
  #pragma config CPD = OFF
  #pragma config BOREN = OFF
  #pragma config LVP = OFF
#elif defined(__18F4550)
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
#endif

#endif
