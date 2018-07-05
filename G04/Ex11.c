// --------------------------------
// Guião 4, Ex. 11
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "delay.c"
# include "send2displays.c"

int main(void) {
	char counter = 0;

	while(1) {
		send2displays(counter);
		delay(200);
		counter++;
	}

  return 1;
}
