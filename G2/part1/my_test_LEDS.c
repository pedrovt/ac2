// --------------------------------
// Guião 2, Parte 1 : Further testing
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

// Testing the LEDs without using the given .hex file

#include <detpic32.h>

void delay(int ms)
{
  for(; ms > 0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer() < 5000);
  }
}

int main(void) {
  //int v = 0;            // to make LED connected to E0 blink

  TRISEbits.TRISE0 = 0;   //sets E0 as output
  TRISEbits.TRISE1 = 0;   //sets E1 as output
  TRISEbits.TRISE2 = 0;   //sets E2 as output
  TRISEbits.TRISE3 = 0;   //sets E3 as output

  while(1) {
    /* to make LED connected to E0 blink
    LATEbits.LATE0 = v;
    printf("Blinking");
    delay(500);
    v ^= 1;
    */

    LATEbits.LATE0 = 1;
    LATEbits.LATE1 = 1;
    LATEbits.LATE2 = 1;
    LATEbits.LATE3 = 1;

  }
}
