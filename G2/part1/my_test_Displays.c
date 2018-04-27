// --------------------------------
// Guião 2, Parte 1 : Further testing
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

// Testing the Displays without using the given .hex file

# include <detpic32.h>
# include "Utils/utils.h"

int main(void) {
  int counter = 0;
  TRISBbits.TRISB0 = 1;

  while (1) {
    int i = 0;
    do {
      delay(10);
      send2displays(counter, 1);
    } while (++i < 50);
    
    
    if (PORTBbits.RB1 == 1) {
      putChar('\n');
      printInt10(counter);
      counter++;
    }
    else {
      printStr("\nStuck in ");
      printInt10(counter);
    }
    if (counter == 9) {
      counter = 0;
      printStr("\nReset! \n");
    }
    
  }

  return 1;
}