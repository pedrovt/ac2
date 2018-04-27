// --------------------------------
// Utils Header File
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include "charUtils.c"
# include "configADC.c"
# include "delay.c"
# include "send2displays.c"
# include "toBcd.c"
# include "nearestPowerOf2.c"
# include "getTimeConstants.c"
# include "configTimers.c"

// Char Utils
char toLowerCase(char c);
char toUpperCase(char c);

// Config ADC
void configADC(int numSamples);

// Delay
void delay(int ms);

// Send To 7-Seg Displays
void send2displays (unsigned char value, unsigned char decimalFlag);

// To BCD
unsigned char toBcd(unsigned char value);

// Compute the next highest power of 2 of 32-bit v
unsigned int nearestPowerOfs2 (unsigned int v);

// Get Time Constants
struct TimerConstants getTimerConstantsTypeA(int fout);
struct TimerConstants getTimerConstantsTypeB(int fout);

// Config Timers
void configTimerT1(int fout);
void configTimerT3(int fout);
void configInterrupts_TimerT1();
void configInterrupts_TimerT3();
