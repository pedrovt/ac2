// --------------------------------
// Config UART1
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

void configUART1(unsigned int baudrate, char numBits, char parity, unsigned int numStopBits)
{
    // Configure Division Value 16
    U1MODEbits.BRGH = 0;

    // Configure BaudRate generator
    // U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) – 1
    if (baudrate < 600 || baudrate > 115200) { // validate baudrate
        baudrate = 115200;
    }
    U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;

    // Configure Number of Data Bits and configure parity
    switch (parity) {
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
    else {
        U1MODEbits.STSEL = numStopBits - 1; // 1 stop bit (=1 for 2 stop bits)
    }

    // Enable the trasmitter and receiver modules (see register U1STA)
    U1STAbits.UTXEN = 1;   // Transmission Module
    U1STAbits.URXEN = 1;   // Reception Module

    // Config UART1 interrupts
    IPC6bits.U1IP   = 2;   // configure priority of interrupts
    IFS0bits.U1RXIF = 0;   // clear interrupt flag
    IEC0bits.U1RXIE = 1;   // enable interrupts
    IFS0bits.U1TXIF = 0;   // clear interrupt flag
    IEC0bits.U1TXIE = 1;   // enable interrupts

    // Enable UART1 (see register U1MODE)
    U1MODEbits.ON = 1;
}
