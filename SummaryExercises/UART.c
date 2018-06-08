// --------------------------------
// UART

// Reads a char from the keyboard
// and prints it

// Reception & Transmission using
// interrupts 

// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>

void configUART1(unsigned int baudrate, char numBits, char parity, unsigned int numStopBits)
{
  // Configure Division Value 16
  U1MODEbits.BRGH = 0;

  // Configure BaudRate generator
  // U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) – 1
  if (baudrate < 600 || baudrate > 115200)
  { // validate baudrate
    baudrate = 115200;
  }
  U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;

  // Configure Number of Data Bits and configure parity
  switch (parity)
  {
  case 'N':
    U1MODEbits.PDSEL = 00; //no parity
    break;
  case 'O':
    U1MODEbits.PDSEL = 10; //odd parity
    break;
  case 'E':
    U1MODEbits.PDSEL = 01; //even parity
    break;
  }

  // Configure Number of Stop Bits
  if (numStopBits > 2)
    numStopBits = 1;
  else
  {
    U1MODEbits.STSEL = numStopBits - 1; // 1 stop bit (=1 for 2 stop bits)
  }

  // Enable the trasmitter and receiver modules (see register U1STA)
  U1STAbits.UTXEN = 1; // Transmission Module
  U1STAbits.URXEN = 1; // Reception Module

  // Enable Interrupts
  U1STAbits.UTXSEL = 00;
  U1STAbits.URXISEL = 00;
  U1STAbits.UTXEN = 1; // Transmission Module
  U1STAbits.URXEN = 1; // Reception Module

  // the folowing bits are on the table in page 74 of the PIC32 manual
  IEC0bits.U1RXIE = 1;
  IEC0bits.U1TXIE = 1;
  IEC0bits.U1EIE = 1;

  // clear interrupt flags
  IFS0bits.U1RXIF = 0;
  IFS0bits.U1TXIF = 0;
  IFS0bits.U1EIF = 0;

  IPC6bits.U1IP = 2;

  // Enable UART1 (see register U1MODE)
  U1MODEbits.ON = 1;
}

void _int_(24) isr_uart1(void) {
  // Errors
  // If U1EIF set then
  if (IFS0bits.U1EIF) {
    // if overrun error then clear OERR flag
    if (U1STAbits.OERR) {
      U1STAbits.OERR = 0;
    }
    // else read U1RXREG to a dummy variable
    else {
      char temp = U1RXREG;
    }
    // clear UART1 error interrupt flag
    IFS0bits.U1EIF = 0;
  }
  
  // Reception
  if (IFS0bits.U1RXIF) {
    U1TXREG = U1RXREG;   // Equivalent to putChar(getChar())
    IFS0bits.U1RXIF = 0; // Clear UART1 RX interrupt flag
  }

  // Transmission
  if (IFS0bits.U1TXIF) {
    IFS0bits.U1TXIF = 0; // Clear UART1 TX interrupt flag
  } 
}

int main(void) {
  // Configure UART1 (115200, N, 8, 1)
  configUART1(115200, 'N', 8, 1);
  EnableInterrupts();
  
  while (1);

  return 1;
}

