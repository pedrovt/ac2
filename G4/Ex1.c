// --------------------------------
// Guião 4, Ex. 1
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "delay.h"

int main(void) {
  LATDbits.LATD0 = 0;         // The initial value should be set
                              // before configuring the port as output

  TRISDbits.TRISD0 = 0;       // RD0 configured as output

  while(1) {
    delay(500);               // half period = 0.5s
    LATDbits.LATD0 = !LATDbits.LATD0;
  }

  return 0;
}
