// --------------------------------
// Guide 7, Part 1, Ex.1
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

// Solution using polling
# include <detpic32.h>
# include "../../Utils/utils.h"

int main(void) {
  // Events with a 2Hz frequency
  configTimerT3(2);

  while (1)
  {
    // Wait until T3IF = 1
    while (IFS0bits.T3IF == 0);

    // Reset T3IF
    IFS0bits.T3IF = 0;

    putChar('.');
  }
    
  return 1;
}
