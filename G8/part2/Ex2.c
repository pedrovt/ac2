// --------------------------------
// Guide 8, Part 2, Ex. 2
// Based on Guide 7, Part 2, Ex. 4
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../../Utils/utils.h"

# define NUM_SAMPLES  8
# define SAMPLE_FREQ  4
# define REFRESH_FREQ 100

// ------------------------------
// Global variables
volatile unsigned char VOLTAGE = 0;
volatile unsigned char MIN_VOLTAGE = 35;
volatile unsigned char MAX_VOLTAGE = 0;

// ------------------------------
// Interrupt Handlers
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
  // Debug
  // printf("\nEntering Interrupt Handler for ADC\n");

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

  if (VOLTAGE < MIN_VOLTAGE) MIN_VOLTAGE = VOLTAGE;
  if (VOLTAGE > MAX_VOLTAGE) MAX_VOLTAGE = VOLTAGE;
  
  // Reset AD1IF
  IFS1bits.AD1IF = 0;
}

void putc(char byte2send) {
  // Wait while UTXBF == 1
  while(U1STAbits.UTXBF == 1);    

  // Copy byte2send to the UxTXREG register
  U1TXREG = byte2send;  
}

void putStr(char *str) {
  while (*str)
    putc(*str++);
}

void _int_(24) isr_uart1(void) {
  if (U1RXREG == 'L' || U1RXREG == 'l') {
    //putStr("MIN_VOLTAGE: ");
    putc(' ');
    putc((MIN_VOLTAGE / 10) + 0x30);
    putc((MIN_VOLTAGE % 10) + 0x30);
    //printInt10(MIN_VOLTAGE);
    putc(' ');
    //putStr("\nMAX_VOLTAGE: ");
    //printInt10(MAX_VOLTAGE);
    putc((MAX_VOLTAGE / 10)+ 0x30);
    putc((MAX_VOLTAGE % 10 ) + 0x30);
  }

  // Clear UART1 rx interrupt flag
  IFS0bits.U1RXIF = 0;
}

// ------------------------------
// Configs
void configureAll() {
  // Config UART1
  configUART1(115200, 'N', 8, 1);

  // Configure port RB4 as analog input
  TRISBbits.TRISB4 = 1;  // RB4 digital output disconnected
  AD1PCFGbits.PCFG4 = 0; // RB4 configured as analog input (AN4)

  // Config ADCC
  AD1CHSbits.CH0SA = 4;
  configADC(NUM_SAMPLES);

  // Interrupt System for the ADC
  IPC6bits.AD1IP = 1;
  IEC1bits.AD1IE = 1;
  IFS1bits.AD1IF = 0;

  // Config Timers
  configTimerT1(SAMPLE_FREQ);
  configTimerT2(REFRESH_FREQ);

  // Config Timers Interrupts
  configInterrupts_TimerT1();
  configInterrupts_TimerT2();

  // Config OC1 pin
  OC1CONbits.OCM = 6;    // PWM mode on OCx; fault pin disabled
  OC1CONbits.OCTSEL = 0; // Use timer T2 as the time base for PWM generation
  OC1CONbits.ON = 1;     // Enable OC1 module */
}

// ------------------------------
// Main Program
int main(void) {   
  // Configure RB0 and RB1 as inputs
  TRISB = ((TRISB & 0xFFFC) | 0x0003);
  
  configureAll();
  EnableInterrupts();

  int dutyCycle;
  while (1) {
    // Read RB1, RB0 to the variable "portVal"
    char portVal = PORTB & 0x0003;
    switch (portVal) {
    case 0:                                         // Measure input voltage
        IEC0bits.T1IE = 1;                              // Enable T1 interrupts
        OC1RS = getPWMConstant(SAMPLE_FREQ, 0);         // LED OFF
        break;
    case 1:                                         // Freeze
        IEC0bits.T1IE = 0;                              // Disable T1 interrupts
        OC1RS = getPWMConstant(SAMPLE_FREQ, 100);       // LED ON (maximum bright)
        break;
    default:                                         // LED brigthness control
        IEC0bits.T1IE = 1;                              // Enable T1 interrupts
        dutyCycle = VOLTAGE * 3;                        // Values between 0 and 99
        OC1RS = getPWMConstant(SAMPLE_FREQ, dutyCycle);
        break;
    }
  }

  return 1;
}
