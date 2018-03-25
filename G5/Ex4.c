// --------------------------------
// Guião 5, Ex.3
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "delay.c"
# define numSamples 4

int main(void) {
                           // Step 1  - Configure the A/D module and port RB4 as
                           // analog input
  TRISBbits.TRISB4  = 1;   // Step 1A - RB4 digital output disconnected
  AD1PCFGbits.PCFG4 = 0;   // Step 1B - RB4 configured as analog input (AN4)

  AD1CHSbits.CH0SA  = 4;   // Step 2  - Desired input analog channel (0 to 15)

  AD1CON2bits.SMPI = numSamples - 1;    // Step 3  - numSamples samples will be converted
                                        // and stored in buffer locations ADC1BUF0 to ADC1BUF3

  AD1CON1bits.SSRC = 7;    // Step 4  - Conversion trigger selection bits: in
  // this mode an internal counter ends sampling and
  // starts conversion

  AD1CON1bits.CLRASAM = 1; // Step 5 - Stop conversions when the 1st A/D
  // converter interrupt is generated. At the same
  // time, hardware clears the ASAM bit

  AD1CON3bits.SAMC = 16;   // Step 6 - Sample time is 16 TAD (TAD = 100 ns)

  AD1CON1bits.ON = 1;      // Step 7 (see Page 4) - Enable A/D converter
  // This must the last command of the A/D
  // configuration sequence

  int i;
  while(1) {
    // Start conversion
    AD1CON1bits.ASAM = 1;

    // Wait while conversion not done (AD1IF == 0)
    while(IFS1bits.AD1IF == 0);

    // Read conversion result (ADC1BUF0-FF value) and print it
    int *p = (int *) (&ADC1BUF0);
    for(i = 0; i < 16; i++) {
      // 16 MSBits with the number of chars (4 in this situation)
      // 16 LSBits with the base (10 in this situation)

      printInt(p[i*4], 10 | 4 << 16);
      putChar(' ');
    }

    putChar('\n');

    // Reset AD1IF
    IFS1bits.AD1IF = 0;

    // comment as necessary. used to make debugging easier
    delay(100);
  }

  return 1;
}
