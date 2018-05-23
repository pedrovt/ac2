// --------------------------------
// Guide 9, COM Driver
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "comDrv.h"

# define TRUE 1
# define FALSE 0
# define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
# define EnableUart1RxInterrupt()  IEC0bits.U1RXIE = 1
# define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
# define EnableUart1TxInterrupt()  IEC0bits.U1TXIE = 1

// Circular Buffers
# define BUF_SIZE 8
# define INDEX_MASK (BUF_SIZE - 1)

typedef struct {
  unsigned char data[BUF_SIZE];
  unsigned int head;
  unsigned int tail;
  unsigned int count;
} circularBuffer;

volatile circularBuffer txb; // Transmission buffer
volatile circularBuffer rxb; // Reception buffer

// -----------------------------------------
void comDrv_flushRx(void) {
  // Initialize variables of the reception buffer
  rxb.head = 0;
  rxb.tail = 0;
  rxb.count = 0;
}

void comDrv_flushTx(void) {
  // Initialize variables of the transmission buffer
  txb.head  = 0;
  txb.tail  = 0;
  txb.count = 0;
}

// -----------------------------------------
// Transmission

// Put a char
void comDrv_putc(char c) {
  // Wait while buffer is full
  while (txb.count == BUF_SIZE);

  // Copy character to the transmission buffer at position "tail"
  txb.data[txb.tail] = c;

  // Increment "tail" index
  // Mask is used to make sure the index never 
  // goes beyond the BUF_SIZE
  // A simple % operation would be enough
  // but they like to make code unreadable.............
  txb.tail = (txb.tail + 1) & INDEX_MASK;

  // Increment "count" variable (critical section)
  DisableUart1TxInterrupt();
  txb.count = txb.count + 1;    // ATTENTION: txb.count++ does not work!!
  EnableUart1TxInterrupt();
}

// Put a String
void comDrv_puts(char *str) {
  while (*str != '\0') {
    comDrv_putc(*str);
    str++;
  }
}

// -----------------------------------------
// Reception

// Get a Char
char comDrv_getc(char *pchar) {
    // Test "count" variable (reception buffer) and return FALSE
    // if it is zero
    if (rxb.count == 0) return FALSE;

    DisableUart1RxInterrupt(); // Begin of critical section
    
    // Copy character pointed by "head" to *pchar
    *pchar = rxb.data[rxb.head];

    // Decrement "count" variable
    rxb.count = rxb.count - 1;

    // Increment "head" variable (mod BUF_SIZE)
    rxb.head = (rxb.head + 1) & INDEX_MASK;

    EnableUart1RxInterrupt(); // End of critical section
    return TRUE;
}

// -----------------------------------------
// Config UART
void comDrv_config(unsigned int baudrate, char parity, unsigned int numStopBits) {
  configUART1(baudrate, 8, parity, numStopBits);
}

// -----------------------------------------
// Interrupt Handler
void _int_(24) isr_uart1(void) {
  if (IFS0bits.U1RXIF == 1) {
    rxb.data[rxb.tail] = U1RXREG; // Read character from UART and
                                  // write it to the "tail" position
                                  // of the reception buffer

    // Increment "tail" variable (mod BUF_SIZE)
    rxb.tail = (rxb.tail + 1) & INDEX_MASK;

    // If reception buffer is not full (e.g. count < BUF_SIZE) then
    if (rxb.count < BUF_SIZE) {
      // increment "count" variable
      rxb.count = rxb.count + 1;
    }     

    // Else
    else {
    // increment "head" variable (discard oldest character)
      rxb.head = (rxb.head + 1) & INDEX_MASK;
    }

    // reset UART1 RX interrupt flag
    IFS0bits.U1RXIF = 0;
  }
  //---------------------------------------------------
  if (IFS0bits.U1TXIF == 1)
  {

    // if "count" variable (transmission buffer, txb) is greater than 0
    if (txb.count > 0) {
      // Copy character pointed by "head" to U1TXREG register
      U1TXREG = txb.data[txb.head];

      // Increment "head" variable (mod BUF_SIZE)
      txb.head = (txb.head + 1) & INDEX_MASK;

      // Decrement "count" variable
      txb.count = txb.count--;
    }

    // if "count" variable is 0 then
    if (txb.count == 0) {
      DisableUart1TxInterrupt();
    }

    // Reset UART1 TX interrupt flag
    IFS0bits.U1TXIF = 0;
  }
}

// -----------------------------------------

