// --------------------------------
// Guide 7, Part 1, Ex.2
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

// Solution using interrupts
# include <detpic32.h>
# include "../../Utils/utils.h"

void _int_(12) isr_T3(void) {
  putChar('.');
  IFS0bits.T3IF = 0;    // Reset T3 interrupt flag
}

int main(void) {
  configTimerT3(2);

  configInterrupts_TimerT3();
  EnableInterrupts();

  while (1);
  
  return 1;
}
