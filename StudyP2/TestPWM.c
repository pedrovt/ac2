// --------------------------------
// Guide Y, Ex. Y
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../Utils/utils.h"

int main(void) {
  int dutyCycle = 0;
  T2CONbits.TCKPS = 3; 
  PR2 = 50000;                   
  TMR2 = 0;                            
  T2CONbits.TON = 1;

  OC1RS = ((PR2 + 1) * dutyCycle) / 100; // OC1RS as a ;

  // Config OC1 pin
  OC1CONbits.OCM = 6;    // PWM mode on OCx; fault pin disabled
  OC1CONbits.OCTSEL = 0; // Use timer T2 as the time base for PWM generation
  OC1CONbits.ON = 1;     // Enable OC1 module */

  while(1) {
    dutyCycle = readInt10();
    OC1RS = ((PR2 + 1) * dutyCycle) / 100; // OC1RS as a ;
    putChar('\n');
  }
  return 0;
}

