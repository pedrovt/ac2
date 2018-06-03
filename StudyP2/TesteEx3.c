// --------------------------------
// Teste, Ex. 3
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

// Program that configs ADC to make 4 conversions per sample. 
// Values between 0 and 99
// Send the values to the 7-Segments Display, in decimal
// ADC Sample Rate of 4Hz --> Timer 3
// Displays Refresh Rate of 75 Hz
// Read ADC values from interrupts

// b) OC5 signal with 75Hz and duty cycle value
# include <detpic32.h>

# define numSamples 4

volatile int temperature;
void config_adc(void);
void configT2(void);
void configT3(void);
//void configOC(void);
void send2displays(unsigned char value);
void delay(int ms);
unsigned char toBCD(unsigned char val);

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main() {
  configT2();
  configT3();
  config_adc();
  EnableInterrupts();

  while (1);

  return 0;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Configuracao da ADC
void _int_(27) isr_adc(void) {
  temperature = 0;
  int j = 0;
  
  int *p = (int *)(&ADC1BUF0);
  for (; j < numSamples; j++) {
    int temp = p[j * numSamples];
    temperature += temp;
  }
  temperature /= numSamples;

  // Get tension amplitude
  temperature = (temperature * 99 + 511) / 1023;
  printInt10(temperature);

  IFS1bits.AD1IF   = 0; // Reset AD1IF flag
}

void config_adc(void) {
  // Inicializacao da ADC
  TRISBbits.TRISB4 = 1;  // RBx digital output disconnected
  AD1PCFGbits.PCFG4 = 0; // RBx configured as analog input (AN4)
  AD1CON1bits.SSRC = 7;  // Conversion trigger selection bits: in this
  // mode an internal counter ends sampling and
  // starts conversion
  AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
  // interrupt is generated. At the same time,
  // hardware clears the ASAM bit
  AD1CON3bits.SAMC = 16;             // Sample time is 16 TAD (TAD = 100 ns)
  AD1CON2bits.SMPI = numSamples - 1; // Interrupt is generated after XX samples
  // (replace XX by the desired number of
  // consecutive samples)
  AD1CHSbits.CH0SA = 4; // replace x by the desired input
  // analog channel (0 to 15)
  AD1CON1bits.ON = 1; // Enable A/D converter
  // This must the last command of the A/D
  // configuration sequence

  IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
  IEC1bits.AD1IE = 1; // enable A/D interrupts
  IFS1bits.AD1IF = 0; // clear A/D interrupt flag
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Timers Configs
// Displays
void _int_(8) isr_t2(void) {
  //printStr("Timer 2");
  send2displays(toBCD(temperature));
  IFS0bits.T2IF = 0;
}

void configT2(void)   {  // 75 Hz
  T2CONbits.TCKPS = 3;
  PR2 = 33333;
  TMR2 = 0;            
  T2CONbits.TON = 1;   
  IPC2bits.T2IP = 2;   
  IEC0bits.T2IE = 1;   
  IFS0bits.T2IF = 0;   
}

// ADC
void _int_(12) isr_t3(void) {
  //printStr("Timer 3");
  AD1CON1bits.ASAM = 1;
  IFS0bits.T3IF = 0;
}

void configT3(void) {                      
  // 4 Hz
  T3CONbits.TCKPS = 8;
  PR3 = 19530;
  TMR3 = 0;            
  T3CONbits.TON = 1;  

  IPC3bits.T3IP = 2; 
  IEC0bits.T3IE = 1; 
  IFS0bits.T3IF = 0; 
}

// Configuracao do modulo OC (INCOMPLETA)
/*void configOC(void)
{
  T2CONbits.TCKPS = 5;   // 1:32 prescaler (i.e Fout_presc = 625 KHz)
  PR2 = 62499;           // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
  TMR2 = 0;              // Reset timer T2 count register
  T2CONbits.TON = 1;     // Enable timer T2 (must be the last command of the timer configuration sequence)
  OC5CONbits.OCM = 6;    // PWM mode on OCx; fault pin disabled
  OC5CONbits.OCTSEL = 0; // Use timer T2 as the time base for PWM generation
  OC5RS = 12500;         // Ton constant
  OC5CONbits.ON = 1;     // Enable OC1 module
}
*/

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Funcao send2Displays (NAO CONFIGURA OS PORTOS DE I/O)
void send2displays(unsigned char value) {
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  TRISB = TRISB & 0x00FF;

  static char codes7S[] = {0x3F, 0x06, 0x5b, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static char displayFlag = 0;
  unsigned int digit_low, digit_high;
  digit_low = value & 0x0F;
  digit_high = value >> 4;
  if (displayFlag == 0)
  {
    LATDbits.LATD6 = 0;
    LATB = (LATB & 0x00FF) | (codes7S[digit_low] << 8);
    LATDbits.LATD5 = 1;
 }
 else {
    LATDbits.LATD5=0;
    LATB = (LATB & 0x00FF) | (codes7S[digit_high] << 8);
    LATDbits.LATD6 = 1;
 }
 displayFlag = !displayFlag;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Utils
// Funcao de conversao de binario para BCD
unsigned char toBCD(unsigned char val) {
 return ((val / 10) << 4) + (val % 10);
}

// Funcao para gerar um atraso em multiplos de 1 ms
void delay(int ms) {
 for(; ms > 0; ms--) {
    resetCoreTimer();
    while(readCoreTimer() < 20000);
 }
}
