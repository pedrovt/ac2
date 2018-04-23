// --------------------------------
// Teste Prático 2015 2c)
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../Utils/utils.h"

// Importante aqui: como inicializar o array com shifts
int main (void) {
  // f a b e d c b a f c d e
  //0     1     2     3     4     5
  //a     b     c     d     e     f

  static char values[] = {0xF1, 0xA1, 0xB1, 0xE2, 0xD2, 0xC2, 0xB2, 0xA2 , 0xF2, 0xC1, 0xD1, 0xE1};
  //static char letters[] = {0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  //char segments1[] = {letters[5], letters[0],letters[1],letters[4],letters[3],letters[2],letters[1],letters[0],letters[5],letters[2],letters[3],letters[4]};

  // 1 1 1 2 2 2 2 2 1 1 1
  //static char segments2[] = {0x06, 0x06, 0x06, 0x5B, 0x5B, 0x5B, 0x5B, 0x5B};

  // Configure RB8 to RB15 as outputs
  TRISB = (TRISB & 0x00FF);

  // Configure RD5 and RD6 as outputs
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  int counter = 0;
  while (1) {
    // if (displayFlag == 0) {
    //   // Set Display Low
    //   LATDbits.LATD5 = 1;
    //   LATDbits.LATD6 = 0;
    //   LATB = (LATB & 0x00FF) | ((segments2[counter] << 8));
    // }
    // else {
    //   // Set Display high
    //   LATDbits.LATD5 = 0;
    //   LATDbits.LATD6 = 1;
    //   LATB = (LATB & 0x00FF) | ((segments1[counter] << 8));
    // }
    int i = 0;
    do {
			delay(20);
			// call send2displays with counter value as argument
			send2displays(values[counter], 0);
		} while(++i < 10);

    	//send2displays(values[counter], 0);
    // Update Counter
    counter++;
    if (counter % 12 == 0) {
      //printStr("\nreseted counter");
      counter = 0;
    }

    // Toggle displayFlag
  //  displayFlag = !displayFlag;


    delay(1000);
  }

}
