// --------------------------------
// Guide 9
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../Utils/utils.h"
# include "../Utils/comDrv.c"

int main(void) {
  // default "pterm" parameters
  comDrv_config(115200, 'N', 1);
  
  // with TX and RX interrupts disabled
  DisableInterrupts();

  comDrv_flushRx();
  comDrv_flushTx();

  EnableInterrupts();
  DisableUart1TxInterrupt();
  //comDrv_puts("Teste do bloco de transmissao do device driver!...");

  char aux;

  while (1) {
    if (comDrv_getc(&aux) != 0) {
      putChar(aux);
      delay(1000);
      //comDrv_putc(aux);
    }

    //comDrv_putc('c');
  }
  return 1;
}
