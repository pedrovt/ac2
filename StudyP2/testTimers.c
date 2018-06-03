// --------------------------------
// Test Timer Constants
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

#define FREQ (40000000)
#define PBCLK (FREQ / 2)
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../Utils/nearestPowerOf2.c"
#include "../Utils/getTimeConstants.c"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("One argument expected.\n");
        return 1;
    }

    int fout = strtol(argv[1], NULL, 10);
    struct TimerConstants t = getTimerConstantsTimerTypeB(fout);

    // Debug
    printf("Timer Config: \n\tfout = %d", fout);
    printf("\n\tt.FREQ_PRESCALER = %d", t.FREQ_PRESCALER);
    printf("\n\tt.CONST_PRESCALER (TCKPS) = %d", t.CONST_PRESCALER);
    printf("\n\tt.VALUE_PRx (PR1) = %d", t.VALUE_PRx);
    printf("\n\n");
}