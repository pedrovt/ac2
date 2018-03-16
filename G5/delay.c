// --------------------------------
// Delay Function
// Arquitectura de Computadores II
// --------------------------------

# include <detpic32.h>

void delay(int ms) {
  for(; ms > 0; ms--) {
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}
