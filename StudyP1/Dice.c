// --------------------------------
// Dice v1
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

/* How to Use
 * p/P -> Pause/Unpause
 * l/L -> Throw the dice (you choose the number)
 * 1-8 -> Change the Speed
 */

/* Goals:
 * Ccontador em que exibe no valor dos displays o valor dos dígitos de um dado:
 * -> Diferentes botões alteram a frequencia com que a sequência 1-6 é mostrada.
 * -> Noutra tecla lança o dado começando numa determinada face.
 * -> Noutra tecla pára numa face do dado.
 */


//do while com delay 10. mexer na condição do while
# include <detpic32.h>
# include "../Utils/utils.h"

int main (void) {
  int counter = 1;
  int value = 0;
  int stopped = 0;

  while (1) {
    char readChar = inkey();
    if (readChar != 0) {
      //printStr("\n\nDetected key ");
      //char readChar = getChar();
      //printStr("\n\nKEY DETECTED!\nRead char: ");
      //putChar(readChar);

      // Pause
      if ((readChar == 'p') | (readChar == 'P')) {
        if (!stopped) printStr("Paused\n");
        else printStr("Resumed\n");
        stopped = !stopped;
      }

      else {
        // Throw Dice
        if ((readChar == 'l') | (readChar == 'L')) {
          printStr("\nThrow the Dice! Press a number between 1 and 6 to start the dice.");
          while (1) {
            readChar = inkey();
            while ((readChar = inkey()) == 0) {
              //printStr(".");
              // Display ON (shows 0)
              //LATB = (LATB & 0x00FF) | (0x3F << 8);
              LATDbits.LATD5 = 1;
              LATDbits.LATD6 = 1;

              // Display OFF
              LATB = LATB & 0x00FF;
              delay(500);

              // Both displays can be on, since the same digit is going to be sent
              LATB = (LATB & 0x00FF) | (0x3F << 8);
              delay(500);

            }
            //printStr("\nbye\nRead char: ");
            //putChar(readChar);
            int newCounter = readChar - 48;
            if (newCounter >= 1 && newCounter <= 6) counter = newCounter;   // validation
            //printStr("\nCounter: ");
            //printInt10 (counter);
            break;
          }
          //printStr("\nbye again");
        }

        // Change Speed
        else {
          int readValue = readChar - 48;
          if (readValue >= 0 && readValue <= 8) {
            //printStr("\n\nSpeed Level (0-8): ");
            //printInt10(readValue);

            value = readValue;

            printStr("\nChanged Speed to (ms): ");
            printInt10 ((25 + 25 * value) * (10));
            printStr(" ");
          }
        }
      }
    }

    int i = 0;
    do {
      delay(10);
      send2displays(toBcd(counter), 0);
    } while (i++ < (25 + 25 * value));

    //delay(250 * value);

    //printStr("\nSTOPPED: ");
    //printInt10 (stopped);
    if (!stopped) {
      //printStr("\nCounter: ");
      //printInt10 (counter);
      counter++;                            // update counter
      if (counter % 7 == 0) {counter = 1;   // reset counter
      //printStr("\nReseted Counter\n");
      }
    }

  }

  return 1;
}
