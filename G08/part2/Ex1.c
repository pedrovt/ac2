// --------------------------------
// Guide 8, Part 2, Ex. 1
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../../Utils/utils.h"

void putc(char byte2send) {
  // Wait while UTXBF == 1
  while(U1STAbits.UTXBF == 1);    

  // Copy byte2send to the UxTXREG register
  U1TXREG = byte2send;  
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
