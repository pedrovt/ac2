// --------------------------------
// ADC_Displays_2
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
void configADC() {
  // Step 1  - Configure the A/D module and port RB4 as
  // analog input
  TRISBbits.TRISB4  = 1;   // Step 1A - RB4 digital output disconnected
  AD1PCFGbits.PCFG4 = 0;   // Step 1B - RB4 configured as analog input (AN4)

  AD1CHSbits.CH0SA  = 4;   // Step 2  - Desired input analog channel (0 to 15)

  AD1CON2bits.SMPI = 3;    // Step 3  - 4 samples will be converted and stored
  // in buffer locations ADC1BUF0 to ADC1BUF3

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
}

void send2displays(unsigned char value) {
  // array attention to the []
  static const char segments[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static char displayFlag = 0;

  // config LATB8-15 as outputs, LATD5 and 6 as outputs
  TRISB = (TRISB & 0x00FF);
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  // !!! low and high value
  int low  = value & 0x0F;    //clean 2nd half
  int high = value >> 4;      // shift 2nd half

  if (displayFlag == 0) {     // low
    LATB = (LATB & 0x00FF) | (segments[low] << 8);
    // !!! shift 8LATDbits.LATD5 = 1;
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;

  }
  else {                      // high
    LATB = (LATB & 0x00FF) | (segments[high] << 8);

    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
  }

  displayFlag = !displayFlag;
}

unsigned int toBcd (unsigned int value) {
  return ((value / 10) << 4) + value % 10;    // +, not |
}

void delay (unsigned int ms){

  for (; ms > 0; ms--) {
    resetCoreTimer();
    while (readCoreTimer() < 20000);
  }
}

int main(void) {
  configADC();

  while(1) {
    AD1CON1bits.ASAM = 1;

    while (IFS1bits.AD1IF == 0);

    // Get average sample
    int j = 0;
    int VAL_AD = 0;
    int *p = (int *) (&ADC1BUF0);
    for(j = 0; j < 4; j++) {
      int v = p[j*4];
      VAL_AD += v;
    }
    VAL_AD /= 4;

    // Get tension amplitude
    int value = (VAL_AD*70 + 511)/1023;

    // Send to displays
    int i = 0;
    do {
      delay(10);
      send2displays(toBcd(value));
    } while (++i < 25);  //250ms

    IFS1bits.AD1IF == 0;
  }
  return 1;
}
