// --------------------------------
// Teste, Ex. 4
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

// 8 Bits Binary Counter
// Receive chars with UART
// UART 1200 bps, 'O', 8 data bits, 1 stop bit
// When receives a char:
//   -> if <, shift left
//        -> if 4 MSBit is 1, print Quarto bit ativo
//   -> if +, increment value
//        -> if value == 15, print Valor maximo

// Reception using interruptions
// Transmission using polling

#include <detpic32.h>

volatile char counter;
static char debug = 0;
void configUART1(unsigned int baudrate, unsigned char parity, unsigned int numStopBits)
{
    // 1) Configurar o gerador de baudrate de acordo com a taxa de transmissão/receção
    // pretendida (registo UxBRG e bit BRGH do registo UxMODE).
    U1MODEbits.BRGH = 0;
    U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;

    // 2) Configurar os parâmetros da trama: dimensão da palavra a transmitir (número de data
    // bits) e tipo de paridade (bits PDSEL<1:0> do registo UxMODE); número de stop bits (bit
    // STSEL do registo UxMODE).
    switch (parity)
    {
    case 'O':
        U1MODEbits.PDSEL = 10;
        break;
    case 'E':
        U1MODEbits.PDSEL = 01;
        break;
    case 'N':
        U1MODEbits.PDSEL = 00;
        break;
    }

    U1MODEbits.STSEL = numStopBits - 1;

    // 3) Ativar os módulos de transmissão e receção (bits UTXEN e URXEN do registo UxSTA).
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;

    // 4) Ativar a UART (bit ON do registo UxMODE)
    U1MODEbits.ON = 1;

    // 5) Activar Interrupções
    U1STAbits.UTXSEL = 00;
    U1STAbits.URXISEL = 00;
    //U1STAbits.UTXEN = 1; // Transmission Module
    U1STAbits.URXEN = 1; // Reception Module

    // the folowwing bits are on the table in page 74 of the PIC32 manual
    IEC0bits.U1RXIE = 1;
    //IEC0bits.U1TXIE = 1;
    IEC0bits.U1EIE = 1;

    IFS0bits.U1RXIF = 0; // clear interrupt flag
    //IFS0bits.U1TXIF = 0; // clear interrupt flag
    IFS0bits.U1EIF = 0; // clear interrupt flag

    IPC6bits.U1IP = 2;
}

void putc(char byte2send)
{
    // wait while UTXBF == 1
    while (U1STAbits.UTXBF)
        ;
    // Copy byte2send to the UxTXREG register
    U1TXREG = byte2send;
}

void putStr(char *str)
{
    // use putc() function to send each charater ('\0' should not
    // be sent)
    while (*str)
        putc(*str++);
}

void _int_(24) isr_uart(void)
{
    if (IFS0bits.U1RXIF)
    {
        switch (U1RXREG)
        {
        case '<': // <, shift left
            counter = (counter << 1);
            if (debug)
                printStr("<");
            // -> if 4 MSBit is 1, print Quarto bit ativo
            if ((counter & 0x08) == 8)
            {
                putStr("Quarto bit ativo\n");
            }
            break;
        case '+': // +, increment value
            counter++;
            // if value == 15, print Valor maximo
            if (debug)
                printStr("+");
            if (counter == 15)
            {
                putStr("Valor maximo\n");
            }
            // reset counter
            if (counter == 16)
            {
                counter = 0;
            }
            break;
        }
        IFS0bits.U1RXIF = 0;
    }
}

int main(void)
{
    configUART1(1200, 'O', 1);

    counter = 0;
    TRISE = TRISE & 0xFFF0;

    EnableInterrupts();

    while (1)
    {
        // Send counter value to LEDS (RE3 to RE0)
        if (debug)
            printInt10(counter);
        LATE = ((LATE & 0xFFF0) | counter);
    }
    return 0;
}
