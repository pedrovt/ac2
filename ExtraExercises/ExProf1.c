// --------------------------------
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../Utils/utils.h"

void putc(char byte2send) {
  // Wait while UTXBF == 1
  while(U1STAbits.UTXBF == 1);   

  // Copy byte2send to the UxTXREG register
  U1TXREG = byte2send;  
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

int main(void) {
  // Configure UART1 (115200, N, 8, 1)
  configUART1(115200, 'N', 8, 2);

  char c = getc();
  TRISDbits.TRISD11 = 0;
  
  while (1) {
    LATDbits.LATD11 = 0;
    putc(c);
    LATDbits.LATD11 = 1;
    //delay(5);
  }

  return 0;
}
