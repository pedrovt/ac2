// --------------------------------
// Teste 2015, Ex. 2
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

//  UART1 115200 N 8 1
//  read dip-switches 2Hz (timer)

# include <detpic32.h>

volatile char inKey;
volatile char value;
volatile char blink     = 0;
volatile char displayOn = 1;

// Funcao send2Displays (NAO CONFIGURA OS PORTOS DE I/O)
void send2displays(unsigned char value) {
    //send2displays
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISB = TRISB & 0x00FF;
    static char codes7S[] = {0x3F, 0x06, 0x5b, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    LATB = (LATB & 0x00FF) | (codes7S[value & 0x0F] << 8);
}

// Funcao de conversao de binario para BCD

unsigned char toBCD(unsigned char val)
{
 return ((val / 10) << 4) + (val % 10);
}


// Funcao para gerar um atraso em multiplos de 1 ms
/*
void delay(int ms)
{
 for(; ms > 0; ms--)
 {
 resetCoreTimer();
 while(readCoreTimer() < 20000);
 }
}
*/

// --------------------------------
// Configs
void configUART (unsigned int baudrate, unsigned char parity, unsigned char numStopBits) {
    // 1) Configurar o gerador de baudrate de acordo com a taxa de transmissão/receção
    // pretendida (registo UxBRG e bit BRGH do registo UxMODE).
    U1MODEbits.BRGH = 0;
    U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;

    // 2) Configurar os parâmetros da trama: dimensão da palavra a transmitir (número de data bits) e tipo de paridade (bits PDSEL<1:0> do registo UxMODE); número de stop bits (bit STSEL do registo UxMODE).
    switch (parity) {
        //assumed 8 data bits
        case 'O': 
            U1MODEbits.PDSEL = 10; 
            break;
        case 'E': 
            U1MODEbits.PDSEL = 01; 
            break;
        case 'N': 
            U1MODEbits.PDSEL = 00; 
            break;
        default :
            U1MODEbits.PDSEL = 00;
            break;
    }
   
    if (numStopBits > 2) U1MODEbits.STSEL = 0;
    else U1MODEbits.STSEL = numStopBits - 1;

    // 3) Ativar os módulos de transmissão e receção (bits UTXEN e URXEN do registo UxSTA).
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;

    // 4) Ativar a UART (bit ON do registo UxMODE).
    U1MODEbits.ON = 1;
}

void configIO() {
    // dip-switches
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    // timer 3 (2HZ)
    T3CONbits.TCKPS = 8; 
    PR3 = 39063;         
    TMR3 = 0;            
    T3CONbits.TON = 1;

    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1; 
    IFS0bits.T3IF = 0; 

    // uart1
    configUART(115200, 'N', 1);
}

// --------------------------------
// UART
char getc(void) {
    // If OERR == 1 then reset OERR
    if (U1STAbits.OERR == 1) {
        U1STAbits.OERR = 0;
    }

    // Wait while URXDA == 0
    while (U1STAbits.URXDA == 0);

    // Return U1RXREG
    return U1RXREG;
}

void putc(char byte2send)
{
    // wait while UTXBF == 1
    while (U1STAbits.UTXBF == 1); 

    // Copy byte2send to the UxTXREG register
    U1TXREG = byte2send;
}

// --------------------------------
// ISR Timers
#include "../utils/delay.c"
void _int_(12) isr_t3() {
    TRISB = (TRISB & 0xFFF0) | 0x000F;
    char value = PORTB & 0x000F;

    // somar em módulo 16, valor de 0 a 9
    if (inKey >= '0' && inKey <= '9') {
        value += inKey;
    }

    // contador módulo 16
    if ((value - 48) == 16)
    {
        value = 0;
        //printStr("\nReseted counter!");
    }

    // toggle mode
    if (inKey == 'p')
    {
        blink = !blink;
        //printStr("\nToggled blink ");
        //printInt10(blink);
        inKey = ' ';
    }

    if (blink == 1) {
        if (displayOn) {
            send2displays(value);
            displayOn = 0;
        }
        else {
            LATB = LATB & 0x00FF;
            displayOn = 1;
        }
        delay(500);
        //printStr("\nToggled display on ");
        //printInt10(displayOn);
    }
    else {
        send2displays(value);
    }

    IFS0bits.T3IF = 0;
}


// --------------------------------
int main(void) {
    configIO();
    EnableInterrupts();
    blink = 0;
    while (1) {
        inKey = getc();
    }
    return 0;
}
