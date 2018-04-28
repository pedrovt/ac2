// --------------------------------
// Config PWM Pins
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

void setPWM_OC1(unsigned int freq, unsigned int dutyCycle) {
    configTimerT2(freq);
    configInterrupts_TimerT2();

    // Get OC1RS value
    struct TimerConstants t = getTimerConstantsTimerTypeB(freq);
    if (dutyCycle < 0 || dutyCycle > 100)
        dutyCycle = 50;                            // dutycycle: range [0, 100]
    OC1RS = ((t.VALUE_PRx + 1) * dutyCycle) / 100; // OC1RS as a function of "dutyCycle"

    // Debug Info
    printStr("PWM Config (Debug Info):\n\tDuty Cycle = ");
    printInt10(dutyCycle);
    printStr("\%\n\tOC1RS = ");
    printInt10(OC1RS);
    printStr("\n\n");

    // Config OC1 pin
    OC1CONbits.OCM = 6;    // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL = 0; // Use timer T2 as the time base for PWM generation
    OC1CONbits.ON = 1;     // Enable OC1 module */
}
