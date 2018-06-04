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
