// --------------------------------
// Guião 4, Ex. 13_100Hz
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "delay.c"
# include "send2displays.c"

int main(void) {
	char counter = 0;

	while(1) {
		int i = 0;
		do {
			delay(10);
			// call send2displays with counter value as argument
			send2displays(counter);
		} while(++i < 20);
		// increment counter (module 256)
		counter++;
	}

	return 1;
}
