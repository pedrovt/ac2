// --------------------------------
// Guião 4, Ex. 11
// Arquitectura de Computadores II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

#include <detpic32.h>
#include "delay.h"
#include "Ex10.c"

int main(void) {
	// Configure RB0 to RB3 as inputs
  TRISB = (TRISB & 0xFFF0) | 0x000F;

  // Configure RB8 to RB14 and RD5 to RD6 as outputs
  TRISB = (TRISB & 0x80FF);
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

	char counter = 0;

	while(1) {
		send2displays(counter);
		delay(200);
		counter++;
	}
}
