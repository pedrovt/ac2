// --------------------------------
// Guide 7, Part 1, Ex.5
// Based on Guide 6, Ex. 4
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../../Utils/utils.h"

# define  numSamples 8

// Global variables
volatile unsigned char VOLTAGE = 0;

// Interrupt Handlers
void _int_(4) isr_T1(void) {
  AD1CON1bits.ASAM = 1;
  IFS0bits.T1IF = 0; // Reset T3 interrupt flag
}

void _int_(12) isr_T3(void) {
  // Send "VOLTAGE" variable to displays
  send2displays(toBcd(VOLTAGE), 1);
  IFS0bits.T3IF = 0; // Reset T3 interrupt flag
}

void _int_(27) isr_adc(void) {
  // Debug
  // printf("\nEntering Interrupt Handler for ADC\n");

  // Get average sample
  int VAL_AD = 0;
  int i = 0;
  int *p = (int *) (&ADC1BUF0);
  for(i = 0; i < numSamples; i++) {
    int value = p[i*4];
    VAL_AD += value;
  }
  VAL_AD /= numSamples;

  // Calculate voltage amplitude
  // Convert voltage amplitude to decimal. Assign it to "voltage"
  VOLTAGE = (VAL_AD*33 + 511)/1023;

  // Reset AD1IF
  IFS1bits.AD1IF = 0;
}

void configureAll() {
  // Configure port RB4 as analog input
  TRISBbits.TRISB4 = 1;  // RB4 digital output disconnected
  AD1PCFGbits.PCFG4 = 0; // RB4 configured as analog input (AN4)

  // Config ADCC
  AD1CHSbits.CH0SA = 4;
  configADC(numSamples);

  // Interrupt System for the ADC
  IPC6bits.AD1IP = 1;
  IEC1bits.AD1IE = 1;
  IFS1bits.AD1IF = 0;

  // Config Timers
  configTimerT1(4);
  configTimerT3(100);

  // Config Timers Interrupts
  configInterrupts_TimerT1();
  configInterrupts_TimerT3();
}

int main(void) {
  configureAll();
  
  EnableInterrupts();

  while (1);

  return 1;
}
