// --------------------------------
// Guião 4, Ex. 4
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "delay.c"
# include "charUtils.c"

int main(void) {
  unsigned char segment;
  LATDbits.LATD6 = 0; // display high active
  LATDbits.LATD5 = 1; // display low inactive

  // configure RB8-RB14 as outputs
  TRISB = TRISB & 0x00FF;

  // configure RD5-RD6 as outputs
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  while(1) {
    // toggle display selection
    LATDbits.LATD6 = !LATDbits.LATD6;
    LATDbits.LATD5 = !LATDbits.LATD5;

    segment = 1;
    int i = 0;
    for(i = 0; i < 7; i++) {
      // send "segment" value to display
      LATB = (LATB & 0x00FF) | segment << 8;

      // wait 1 second
      delay(1000);

      segment = segment << 1;
    }
  }

  return 1;
}
