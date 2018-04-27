// --------------------------------
// Guide 7, Part 1, Ex.4
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../../Utils/utils.h"

void _int_(4) isr_T1(void) {
  putChar('1');
  IFS0bits.T1IF = 0; // Reset T3 interrupt flag
}

void _int_(12) isr_T3(void) {
  putChar('3');
  IFS0bits.T3IF = 0; // Reset T3 interrupt flag
}

int main(void) {
  configTimerT1(2);
  configTimerT3(10);

  configInterrupts_TimerT1();
  configInterrupts_TimerT3();
  EnableInterrupts();

  while (1);

  return 1;
}
