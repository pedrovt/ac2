// --------------------------------
// Utils Header File
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include "charUtils.c"
# include "configADC.c"
# include "delay.c"
# include "send2displays.c"
# include "toBcd.c"

// Char Utils
char toLowerCase(char c);
char toUpperCase(char c);

// Config ADC
void configADC(int numSamples);

// Delay
void delay(int ms);

// Send To 7-Seg Displays
void send2displays (unsigned char value, unsigned char decimalFlag);

// To BCD
unsigned char toBcd(unsigned char value);
