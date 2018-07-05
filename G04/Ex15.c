// --------------------------------
// Guião 4, Ex.15
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "delay.c"
# include "send2displays.c"
# include "toBcd.c"

int main(void) {
	char counter = 0;

	while(1) {
		int i = 0;
		do {
			delay(10);
			// call send2displays with counter value as argument
			send2displays(toBcd(counter));
		} while(++i < 20);
		// increment counter (module 60)
		counter++;
		counter = counter % 60;
	}

	return 1;
}
