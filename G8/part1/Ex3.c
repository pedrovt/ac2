// --------------------------------
// Guide 8, Ex. 3
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../../Utils/utils.h"

void putc(char byte2send) {
  // Wait while UTXBF == 1
  while(U1STAbits.UTXBF == 1);    // FIXME

  // Copy byte2send to the UxTXREG register
  U1TXREG = byte2send;  // FIXME
}

void putStr(char *str) {
  // use putc() function to send each charater ('\0' should not
  // be sent)
  while (*str)
    putc(*str++);
}

int main(void) {
  // configUART1(115200, 'N', 8, 1);
  // Configure UART1 (115200, N, 8, 1)
  while (1) {
    //putc('+');
    putStr("\nString de teste\n");
    // wait 1 s
    delay(1000);
  }

  return 0;
}
