#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#if defined(__16F877)
  #pragma config FOSC = HS
  #pragma config WDTE = OFF
  #pragma config PWRTE = OFF
  #pragma config CP = OFF
  #pragma config BOREN = OFF
  #pragma config LVP = OFF
  #pragma config CPD = OFF
  #pragma config WRT = OFF
#elif defined(__18F4550)
  #pragma config FOSC = HSPLL_HS
  #pragma config WDT = OFF
  #pragma config PBADEN = OFF
  #pragma config LPT1OSC = OFF
  #pragma config MCLRE = ON
  #pragma config STVREN = ON
  #pragma config LVP = OFF
  #pragma config XINST = OFF
  #pragma config CPUDIV = OSC1_PLL2
  #pragma config CP0 = OFF
  #pragma config CP1 = OFF
  #pragma config CP2 = OFF
  #pragma config CP3 = OFF
  #pragma config CPB = OFF
  #pragma config CPD = OFF
  #pragma config WRT0 = OFF
  #pragma config WRT1 = OFF
  #pragma config WRT2 = OFF
  #pragma config WRT3 = OFF
  #pragma config WRTC = OFF
  #pragma config WRTB = OFF
  #pragma config WRTD = OFF
  #pragma config EBTR0 = OFF
  #pragma config EBTR1 = OFF
  #pragma config EBTR2 = OFF
  #pragma config EBTR3 = OFF
#endif

#endif
