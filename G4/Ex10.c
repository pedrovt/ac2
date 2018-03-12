// --------------------------------
// Guião 4, Ex. 10
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

#include <detpic32.h>
#include "delay.h"

void send2displays(unsigned char value) {

  static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

  // Send to display low
  LATDbits.LATD5 = 0;
  LATDbits.LATD6 = 1;
  LATB = (LATB & 0x00FF) | (display7Scodes[value >> 4 & 0x000F] << 8);

  delay(2);
  // Send to display high
  LATDbits.LATD5 = 1;
  LATDbits.LATD6 = 0;
  LATB = (LATB & 0x00FF) | (display7Scodes[value & 0x00F0] << 8);

  }

  return 0;
}
