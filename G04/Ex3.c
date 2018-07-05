// --------------------------------
// Guião 4, Ex. 3
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "delay.c"
# include "charUtils.c"

int main(void) {
  // Sets ports RB8 to RB15, RD5 and RD6 as outputs
  TRISB = TRISB & 0x00FF;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;


  // Comment what is not needed
  // Select Display Low (RD5=1, i.e. "CNTL_DISP_L"=1, e RD6=0)
  // LATDbits.LATD5 = 1;
  // LATDbits.LATD6 = 0;

  // Select Display Hight (RD5=0, i.e. "CNTL_DISP_L"=1, e RD6=1)
  LATDbits.LATD5 = 0;
  LATDbits.LATD6 = 1;

  // Em ciclo infinito, execute as seguintes tarefas:
  //printStr("HELLO!");
  while (1) {
    // delay
    delay(250);

  //  printStr("HELLO loop");

    // reset displays
    LATB = LATB & 0x00FF;

    // Ler um caracter do teclado e esperar que seja digitada uma letra
    // entre 'a' e 'g' (ou 'A' e 'G') ou o caracter '.' Use o system call getChar().

    char c = toLowerCase(getChar());
    while (c != 'a' && c != 'b' && c != 'c' && c != 'd' && c != 'e' && c != 'f' && c != 'g' && c != '.') {
      c = getChar();

      // Debug
      printStr("\nRead char: ");
      putChar(c);

      // Convert from upper to lower case
      c = toLowerCase(c);

      // Debug
      printStr("\nChar after lower case treatment: ");
      putChar(c);
    }

    // Escrever no porto B a combinação binária que ative
    // apenas o segmento do display correspondente ao caracter lido

    switch (c) {
      case 'a':
        printStr("\nON a RB8");
        LATBbits.LATB8 = 1;
        break;
      case 'b':
        printStr("\nON b RB9");
        LATBbits.LATB9 = 1;
        break;
      case 'c':
        printStr("\nON c RB10");
        LATBbits.LATB10 = 1;
        break;
      case 'd':
        printStr("\nON d RB11");
        LATBbits.LATB11 = 1;
        break;
      case 'e':
        printStr("\nON e RB12");
        LATBbits.LATB12 = 1;
        break;
      case 'f':
        printStr("\nON f RB13");
        LATBbits.LATB13 = 1;
        break;
      case 'g':
        printStr("\nON g RB14");
        LATBbits.LATB14 = 1;
        break;
      case '.':
        printStr("\nON . RB15");
        LATBbits.LATB15 = 1;
        break;
    }

  }
  return 1;
}
