// --------------------------------
// Guião 4, Ex.16
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "delay.c"
# include "send2displays.c"
# include "toBcd.c"

int main(void) {
	// Configure RB15 as output
	TRISBbits.TRISB15 = 0;

	char counter = -1;

	while(1) {
		int i = 0;
		do {
			delay(10);
			send2displays(toBcd(counter));
		} while(++i < 20);
		counter++;
		counter = counter % 60;

		// Toggle on the point
		LATBbits.LATB15 = 1;

		// Counter value is even --> point at Display low
		if (counter % 2 == 0) {
			// Enable Display Low
			LATDbits.LATD5 = 1;
			LATDbits.LATD6 = 0;
		}

		// Counter value is odd --> point at Display High
		else {
			// Enable Display High
			LATDbits.LATD5 = 0;
			LATDbits.LATD6 = 1;
		}

		delay(30);
	}

	return 1;
}
