// --------------------------------
// Guião 4, Ex. 2
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "delay.h"

int main(void) {
  // Set RE0 to RE3 ports as outputs (could read the whole TRISE and the mask 0xFFF0)
  TRISEbits.TRISE0 = 0;
  TRISEbits.TRISE1 = 0;
  TRISEbits.TRISE2 = 0;
  TRISEbits.TRISE3 = 0;

  // Clean values on RE0 to RE3 ports
  LATE = (LATE & 0xFFF0);

  // Counter
  int counter = 0;

  while (1) {
    delay(1000);
    LATE = (LATE & 0xFFF0) | counter;
    counter++;
  }

  return 1;
}
