// --------------------------------
// Guide 8, Ex. 1
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../Utils/utils.h"

void configUART1(int baudrate) {
  // Configure Division Value 16
  U1MODEbits.BRGH  = 0; 

  // Configure BaudRate generator
  // U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) – 1
  U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;
  
  // Configure Number of Data Bits and configure parity
  U1MODEbits.PDSEL = 00;  // See page 6 of PIC32 Reference Manual, UART Section
  
  // Configure Number of Stop Bits
  U1MODEbits.STSEL = 0;    // 1 stop bit (=1 for 2 stop bits)

  // Enable the trasmitter and receiver modules (see register U1STA)
  U1STAbits.UT1EN  = 1;    // Transmission Module
  U1STAbits.UR1EN  = 1;    // Reception Module

  // Enable UART1 (see register U1MODE)
  U1MODEbits.ON    = 1;
}

int main(void) {
  configUART1(115200);
  return 0;
}