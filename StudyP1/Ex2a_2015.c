// --------------------------------
// Teste Prático 2015 2a)
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../Utils/utils.h"

// Importante aqui: como inicializar o array com shifts
int main (void) {
  // F A B G E D C G
  // 6 1 2 7 5 4 3 7
  static char segments1[] = { (1 << 5), 1, (1 << 1), (1 << 6), (1 << 4), (1 << 3), (1 << 2), (1 << 6)};

  // Configure RB8 to RB15 as outputs
  TRISB = (TRISB & 0x00FF);

  // Configure RD5 and RD6 as outputs
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  // Set Display Low
  LATDbits.LATD5 = 1;
  LATDbits.LATD6 = 0;

  int counter = 0;
  while (1) {
    //char counter = getChar() - 48;
    //printStr("\n\nvalue = ");
    //printInt10(counter);
    //printStr("\n\ncounter = ");
    //printInt10(counter);

    //printStr("\nsegments1[counter] = ");
    //printInt10(segments1[counter]);
    LATB = (LATB & 0x00FF) | ((segments1[counter] << 8));
    counter++;

    if (counter % 8 == 0) {
      //printStr("\nreseted counter");
      counter = 0;
    }

    delay(250);     //0.25s
  }

}
