// --------------------------------
// Teste Prático 2015 2b)
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../Utils/utils.h"
# define NUMSAMPLES 1

void myConfigADC(void);

// Importante aqui: como obter 8 valores diferentes de tensão com a ADC (número de samples e bits a utilizar)
// MUITO IMPORTANTE: & != &&

int main (void) {
  // Segments
  static char segments[] = { (1 << 5), 1, (1 << 1), (1 << 6), (1 << 4), (1 << 3), (1 << 2), (1 << 6)};

  // Configure RB8 to RB15 as outputs
  TRISB = (TRISB & 0x00FF);

  // Configure RD5 and RD6 as outputs
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  // Configure RB4 as Analog Input
  TRISBbits.TRISB4  = 1;   // RB4 digital output disconnected
  AD1PCFGbits.PCFG4 = 0;   // RB4 configured as analog input (AN4)

  // Config ADC
  myConfigADC();

  // Set Display Low
  LATDbits.LATD5 = 1;
  LATDbits.LATD6 = 0;

  // Counter
  int counter = 0;

  while (1) {
    // Start conversion. Wait while conversion not done
    AD1CON1bits.ASAM = 1;
    while(IFS1bits.AD1IF == 0);
    IFS1bits.AD1IF = 0;                 // Reset AD1IF

    // Compute Delay Value
    int value = 250 + 250 * ((ADC1BUF0 & 0x380) >> 7);

    // Debug
    printStr("\nvalue to delay: ");
    printInt10(value);

    // Send to display
    LATB = (LATB & 0x00FF) | ((segments[counter] << 8));

    // Update Counter
    counter++;
    if (counter % 8 == 0) counter = 0;

    // 8 levels (0.25s-2s)
    delay(value);
  }

  return 1;
}

void myConfigADC(void) {
  AD1CHSbits.CH0SA  = 4;          // Step 2  - Desired input analog channel (0 to 15)

  AD1CON2bits.SMPI = NUMSAMPLES;  // Step 3  - 1 sample will be converted and stored
                                  // in buffer location ADC1BUF0

  AD1CON1bits.SSRC = 7;           // Step 4  - Conversion trigger selection bits: in
                                  // this mode an internal counter ends sampling and
                                  // starts conversion

  AD1CON1bits.CLRASAM = 1;        // Step 5 - Stop conversions when the 1st A/D
                                  // converter interrupt is generated. At the same
                                  // time, hardware clears the ASAM bit

  AD1CON3bits.SAMC = 16;          // Step 6 - Sample time is 16 TAD (TAD = 100 ns)

  AD1CON1bits.ON = 1;             // Step 7 (see Page 4) - Enable A/D converter
                                  // This must the last command of the A/D
                                  // configuration sequence
}
