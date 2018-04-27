// --------------------------------
// Get Timer Constants for Timers
// Type A and Type B
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------~

# include <math.h>
# include <stdlib.h>
# include "log2Integers.c"

struct TimerConstants
{
    int FREQ_PRESCALER;
    int CONST_PRESCALER;
    int VALUE_PRx;
};

struct TimerConstants getTimerConstantsTimerTypeB(int fout)
{
    // Determine the constants
    int f_PRESCALER = PBCLK / fout;
    
    int k_PRESCALER = ceil(f_PRESCALER / pow(2, 16));
    k_PRESCALER = nearestPowerOf2(k_PRESCALER);

    // validation of k_PRESCALER
    if (k_PRESCALER == 128) 
        k_PRESCALER = 256;
    if (k_PRESCALER > 256) {
        printStr("Can't config Timer!\n");
        exit(1);
    }

    k_PRESCALER = log2Integers(k_PRESCALER);
    int prx_value = ((PBCLK / k_PRESCALER) / fout) - 1;

    // Create the struct
    struct TimerConstants t;
    t.FREQ_PRESCALER  = f_PRESCALER;
    t.CONST_PRESCALER = k_PRESCALER;
    t.VALUE_PRx       = prx_value; 
    return t;
}

struct TimerConstants getTimerConstantsTimerTypeA(int fout)
{
    // Determine the constants
    int f_PRESCALER = PBCLK / fout;

    int k_PRESCALER = ceil(f_PRESCALER / pow(2, 16));
    k_PRESCALER = nearestPowerOf2(k_PRESCALER);

    // validation of k_PRESCALER
    if (k_PRESCALER == 32) {
        k_PRESCALER = 64;
    }
    if (k_PRESCALER == 128) {
        k_PRESCALER = 256;
    }
    if (k_PRESCALER > 256) {
        printStr("Can't config Timer!\n");
        exit(1);
    }

    k_PRESCALER = log2Integers(k_PRESCALER);
    int prx_value = ((PBCLK / k_PRESCALER) / fout) - 1;

    // Create the struct
    struct TimerConstants t;
    t.FREQ_PRESCALER = f_PRESCALER;
    t.CONST_PRESCALER = k_PRESCALER;
    t.VALUE_PRx = prx_value;
    return t;
}
