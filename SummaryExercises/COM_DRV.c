// --------------------------------
// COM Driver
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

#include <detpic32.h>

#define TRUE 1
#define FALSE 0
#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1

// Circular Buffers
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)

typedef struct
{
  unsigned char data[BUF_SIZE];
  unsigned int head;
  unsigned int tail;
  unsigned int count;
} circularBuffer;

volatile circularBuffer txb; // Transmission buffer
volatile circularBuffer rxb; // Reception buffer

// Auxiliar Functions Prototypes
void comDrv_flushRx(void);
void comDrv_flushTx(void);
void comDrv_putc(char c);
void comDrv_puts(char *str);
char comDrv_getc(char *pchar);
void comDrv_config(unsigned int baudrate, char parity, unsigned int numStopBits);
void configUART1(unsigned int baudrate, char numBits, char parity, unsigned int numStopBits);
void delay(int ms);

int main(void)
{
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

  while (1)
  {
    if (comDrv_getc(&aux) != 0)
    {
      putChar(aux);
      delay(1000);
      //comDrv_putc(aux);
    }

    //comDrv_putc('c');
  }
  return 1;
}

// -----------------------------------------
// Transmission

// Put a char
void comDrv_putc(char c)
{
  // Wait while buffer is full
  while (txb.count == BUF_SIZE)
    ;

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
  txb.count = txb.count + 1; // ATTENTION: txb.count++ does not work!!
  EnableUart1TxInterrupt();
}

// Put a String
void comDrv_puts(char *str)
{
  while (*str != '\0')
  {
    comDrv_putc(*str);
    str++;
  }
}

// -----------------------------------------
// Reception

// Get a Char
char comDrv_getc(char *pchar)
{
  // Test "count" variable (reception buffer) and return FALSE
  // if it is zero
  if (rxb.count == 0)
    return FALSE;

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
// Interrupt Handler
void _int_(24) isr_uart1(void)
{
  if (IFS0bits.U1RXIF == 1)
  {
    rxb.data[rxb.tail] = U1RXREG; // Read character from UART and
                                  // write it to the "tail" position
                                  // of the reception buffer

    // Increment "tail" variable (mod BUF_SIZE)
    rxb.tail = (rxb.tail + 1) & INDEX_MASK;

    // If reception buffer is not full (e.g. count < BUF_SIZE) then
    if (rxb.count < BUF_SIZE)
    {
      // increment "count" variable
      rxb.count = rxb.count + 1;
    }

    // Else
    else
    {
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
    if (txb.count > 0)
    {
      // Copy character pointed by "head" to U1TXREG register
      U1TXREG = txb.data[txb.head];

      // Increment "head" variable (mod BUF_SIZE)
      txb.head = (txb.head + 1) & INDEX_MASK;

      // Decrement "count" variable
      txb.count = txb.count--;
    }

    // if "count" variable is 0 then
    if (txb.count == 0)
    {
      DisableUart1TxInterrupt();
    }

    // Reset UART1 TX interrupt flag
    IFS0bits.U1TXIF = 0;
  }
}

// -----------------------------------------
// Flush buffers  
void comDrv_flushRx(void)
{
  // Initialize variables of the reception buffer
  rxb.head = 0;
  rxb.tail = 0;
  rxb.count = 0;
}

void comDrv_flushTx(void)
{
  // Initialize variables of the transmission buffer
  txb.head = 0;
  txb.tail = 0;
  txb.count = 0;
}

// -----------------------------------------
// Config
void comDrv_config(unsigned int baudrate, char parity, unsigned int numStopBits)
{
  configUART1(baudrate, 8, parity, numStopBits);
}

// -----------------------------------------
// Auxiliar Functions
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
  //U1STAbits.UTXSEL = 00;
  U1STAbits.URXISEL = 00;
  //U1STAbits.UTXEN = 1; // Transmission Module
  U1STAbits.URXEN = 1; // Reception Module

  // the folowing bits are on the table in page 74 of the PIC32 manual
  IEC0bits.U1RXIE = 1;
  //IEC0bits.U1TXIE = 1;
  IEC0bits.U1EIE = 1;

  // clear interrupt flags
  IFS0bits.U1RXIF = 0;
  //IFS0bits.U1TXIF = 0;
  IFS0bits.U1EIF = 0;

  IPC6bits.U1IP = 2;

  // Enable UART1 (see register U1MODE)
  U1MODEbits.ON = 1;
}

void delay(int ms)
{
  for (; ms > 0; ms--)
  {
    resetCoreTimer();
    while (readCoreTimer() < 20000)
      ;
  }
}
