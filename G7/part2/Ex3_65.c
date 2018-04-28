// --------------------------------
// Guide 7, Part 1, Ex. 3 (65%)
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../../Utils/utils.h"

int main(void) {
  setPWM_OC1(10, 65);
  while (1);
  return 0;
}
