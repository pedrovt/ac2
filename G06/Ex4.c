// --------------------------------
// Guião 6, Ex.4
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../Utils/utils.h"

# define  numSamples 8

// Global variables
volatile unsigned char VOLTAGE = 0;

// Interrupt Handler
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

int main(void) {
  // Configure port RB4 as analog input
  TRISBbits.TRISB4  = 1;   // RB4 digital output disconnected
  AD1PCFGbits.PCFG4 = 0;   // RB4 configured as analog input (AN4)

  // ADC Desired input analog channel (0 to 15)
  AD1CHSbits.CH0SA  = 4;

  configADC(numSamples);

  // Interrupt System
  // Step 1. Set Priority Level of the ADC Interrupts: value between 1 and 6 (7
  // is max priority and should NOT be used and 0 means no interrupts are accepted
  IPC6bits.AD1IP = 2;

  // Step 2. Authorize the interrupts generated by the ADC
  IEC1bits.AD1IE = 1;

  // Step 3. Reset ADC interrupt flag
  IFS1bits.AD1IF = 0;

  // Step 4. Global enable of the interrupt system
  EnableInterrupts();

  int i = 0;
  while (1) {
    delay(10);      // 10 ms

    if(i++ == 25) { // 250 ms (4 samples/second)
      // Start ADC conversion
      AD1CON1bits.ASAM = 1;

      i = 0;
    }

    // Send "VOLTAGE" variable to displays
    send2displays(toBcd(VOLTAGE), 1);
  }

  return 1;
}
