// --------------------------------
// Guião 4, Ex.17
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

	char counter = 50;		//should be -1, value is for testing purposes

	while(1) {
		int i = 0;
		do {
			delay(10);
			send2displays(toBcd(counter));
		} while(++i < 20);
		counter++;
		counter = counter % 60;

		// Counter starts again at 0
		if (counter == 0) {
			int i;

			// Runs 5 times (5 cycles ON-OFF, each one 1 second long (500 ms + 500 ms))
			for (i = 0; i < 5; i++) {
				// Display ON (shows 0)
				//send2displays(0);
				//send2displays(0);
				LATB = (LATB & 0x00FF) | (0x3F << 8);

				// Both displays can be on, since the same digit is going to be sent
				LATDbits.LATD5 = 1;
				LATDbits.LATD6 = 1;
				delay(500);

				// Display OFF
				LATB = LATB & 0x00FF;
				delay(500);
			}

			counter++;
		}

		else {
			// Toggle on the point
			LATBbits.LATB15 = 1;

			// Counter value is even --> point at Display low
			if (counter % 2 == 0) {
				// Enable Display Low
				LATDbits.LATD5 = 1;
				LATDbits.LATD6 = 0;
				delay(30);
			}

			// Counter value is odd --> point at Display High
			else {
				// Enable Display High
				LATDbits.LATD5 = 0;
				LATDbits.LATD6 = 1;
				delay(30);
			}
		}
	}

	return 1;
}
