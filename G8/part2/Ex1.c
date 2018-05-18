// --------------------------------
// Guide 8, Part 2, Ex. 1
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

char getc(void) {
  // If OERR == 1 then reset OERR
  if (U1STAbits.OERR == 1) {
    U1STAbits.OERR = 0;
  }

  // Wait while URXDA == 0
  while (U1STAbits.URXDA == 0);

  // If FERR or PERR then
  if ((U1STAbits.FERR == '1') | (U1STAbits.PERR == '1'))
  {
    // read UxRXREG (to discard the character) and return 0
    printStr("You misbehaved!! :(\n");
    char temp = U1RXREG;
    return 0;
  }
  // Return U1RXREG
  return U1RXREG;
}

void _int_(24) isr_uart1(void) {
  putc(U1RXREG);

  // Clear UART1 rx interrupt flag
  IFS0bits.U1RXIF = 0;
}

int main(void) {
  // Configure UART1 (115200, N, 8, 1)
  configUART1(115200, 'N', 8, 1);
  EnableInterrupts();

  while (1);

  return 0;
}
