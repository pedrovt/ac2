// --------------------------------
// ADC & TIMERS
// Based on Guide 7, Part 2, Ex. 4

// Based on the value from the dip-switches:
// 00 – funciona como voltímetro (o LED deve ficar OFF)
// 01 – congela o valor atual da tensão (LED ON com o brilho no máximo) 
// 1X - brilho do LED depende do valor da tensão medido pelo sistema

// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>

# define NUM_SAMPLES  8
# define SAMPLE_FREQ  4
# define REFRESH_FREQ 100

// Auxiliar Functions prototypes
void send2displays(unsigned char value, unsigned char decimalFlag);
unsigned char toBcd(unsigned char value);
void configADC();
void configTimers();
void configureAll();

// ------------------------------
// Global variables
volatile unsigned char VOLTAGE = 0;

// ------------------------------
int main(void) {
  // Configure RB0 and RB1 as inputs
  TRISB = ((TRISB & 0xFFFC) | 0x0003);

  configureAll();
  EnableInterrupts();

  int dutyCycle;
  while (1)
  {
    // Read RB1, RB0 to the variable "portVal"
    char portVal = PORTB & 0x0003;
    switch (portVal)
    {
    case 0:              // Measure input voltage
      IEC0bits.T1IE = 1; // Enable T1 interrupts
      //OC1RS = getPWMConstant(SAMPLE_FREQ, 0);
        OC1RS = ((PR1 + 1) * 0) / 100;                  // LED OFF
        break;
    case 1:              // Freeze
      IEC0bits.T1IE = 0; // Disable T1 interrupts
      //OC1RS = getPWMConstant(SAMPLE_FREQ, 100);       // LED ON (maximum bright)
        OC1RS = ((PR1 + 1) * 100) / 100;                // LED ON (maximum bright)
        break;
    default:                   // LED brigthness control
      IEC0bits.T1IE = 1;       // Enable T1 interrupts
      dutyCycle = VOLTAGE * 3; // Values between 0 and 99
        OC1RS = ((PR1 + 1) * dutyCycle) / 100;
        break;
    }
  }

  return 1;
}

// ------------------------------
// ISRs
void _int_(4) isr_T1(void) {
  AD1CON1bits.ASAM = 1;
  IFS0bits.T1IF = 0; // Reset T3 interrupt flag
}

void _int_(8) isr_T2(void) {
  // Send "VOLTAGE" variable to displays
  send2displays(toBcd(VOLTAGE), 1);
  IFS0bits.T2IF = 0; // Reset T2 interrupt flag
}

void _int_(27) isr_adc(void) {
  // Get average sample
  int VAL_AD = 0;
  int i = 0;
  int *p = (int *) (&ADC1BUF0);
  for(i = 0; i < NUM_SAMPLES; i++) {
    int value = p[i*4];
    VAL_AD += value;
  }
  VAL_AD /= NUM_SAMPLES;

  // Calculate voltage amplitude
  // Convert voltage amplitude to decimal. Assign it to "voltage"
  VOLTAGE = (VAL_AD*33 + 511)/1023;

  // Reset AD1IF
  IFS1bits.AD1IF = 0;
}

// ------------------------------
// Configs
void configADC() {
  // Config ADC

  // Step 1 - Configure port RB4 as analog input
  TRISBbits.TRISB4 = 1;  // RB4 digital output disconnected
  AD1PCFGbits.PCFG4 = 0; // RB4 configured as analog input (AN4)

  // Step 2 - Choose the analog channel
  AD1CHSbits.CH0SA = 4;

  // Step 3  - 4 samples will be converted and stored in buffer locations
  // ADC1BUF0 to ADC1BUF3
  AD1CON2bits.SMPI = NUM_SAMPLES - 1;

  // Step 4  - Conversion trigger selection bits: in this mode an internal
  // counter ends sampling and starts conversion
  AD1CON1bits.SSRC = 7;

  // Step 5 - Stop conversions when the 1st A/D converter interrupt is
  // generated. At the same time, hardware clears the ASAM bit.
  AD1CON1bits.CLRASAM = 1;

  // Step 6 - Sample time is 16 TAD (TAD = 100 ns)
  AD1CON3bits.SAMC = 16;

  // Step 7 - Enable A/D converter
  AD1CON1bits.ON = 1;

  // Interrupt System for the ADC
  IPC6bits.AD1IP = 1;
  IEC1bits.AD1IE = 1;
  IFS1bits.AD1IF = 0;
}

void configTimers() {
  T1CONbits.TCKPS = 8;
  PR1 = 19531;
  TMR1 = 0;
  T1CONbits.TON = 1;

  T2CONbits.TCKPS = 2;
  PR2 = 50000;
  TMR2 = 0;
  T2CONbits.TON = 1;

  // Config Timers Interrupts
  IPC1bits.T1IP = 2;
  IEC0bits.T1IE = 1;
  IFS0bits.T1IF = 0;

  IPC2bits.T2IP = 2;
  IEC0bits.T2IE = 1;
  IFS0bits.T2IF = 0;
}

void configureAll() {
  configADC();
  configTimers();

  // Config OC1 pin
  OC1CONbits.OCM = 6;    // PWM mode on OCx; fault pin disabled
  OC1CONbits.OCTSEL = 0; // Use timer T2 as the time base for PWM generation
  OC1CONbits.ON = 1;     // Enable OC1 module */
}

// ------------------------------
// Auxiliar functions
void send2displays(unsigned char value, unsigned char decimalFlag)
{
  static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static char displayFlag = 0;

  // Configure RB8 to RB15 as outputs
  TRISB = (TRISB & 0x00FF);

  // Configure RD5 and RD6 as outputs
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  // Determine digits
  int digit_low = value & 0x0F; // should be char but pcompile gives a warning and I don't like warnings :P
  int digit_high = value >> 4;  // value >> 4 & 0x000F would be the same, obviously

  // Send digit_low to display low
  if (displayFlag == 0)
  {
    LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
  }

  // Send digit_high to display high
  else
  {
    LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;

    if (decimalFlag == 1)
    { // Decimal point
      LATBbits.LATB15 = 1;
    }
  }

  // Toggle displayFlag
  displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value) {
  return ((value / 10) << 4) + (value % 10);
}
