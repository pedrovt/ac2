// --------------------------------
// Config Timers
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

void configTimerT1(int fout) {
    struct TimerConstants t = getTimerConstantsTimerTypeA(fout);

    // Configure Timer T1
    T1CONbits.TCKPS = t.CONST_PRESCALER; 
    PR1 = t.VALUE_PRx;                   
    TMR1 = 0;                            
    T1CONbits.TON = 1;

    // Debug
    printStr("Timer 1 Config (Debug Info): \n\tfout = ");  printInt10(fout);
    printStr("\n\tt.FREQ_PRESCALER = ");                   printInt10(t.FREQ_PRESCALER);
    printStr("\n\tt.CONST_PRESCALER (TCKPS) = ");          printInt10(t.CONST_PRESCALER);
    printStr("\n\tt.VALUE_PRx (PR1) = ");                  printInt10(t.VALUE_PRx);
    printStr("\n\n");
}

void configTimerT2(int fout) {
    struct TimerConstants t = getTimerConstantsTimerTypeB(fout);

    // Configure Timer T2
    T2CONbits.TCKPS = t.CONST_PRESCALER; 
    PR2 = t.VALUE_PRx;                   
    TMR2 = 0;                            
    T2CONbits.TON = 1;                  

    // Debug
    printStr("Timer 2 Config (Debug Info): \n\tfout = ");
    printInt10(fout);
    printStr("\n\tt.FREQ_PRESCALER = ");
    printInt10(t.FREQ_PRESCALER);
    printStr("\n\tt.CONST_PRESCALER (TCKPS) = ");
    printInt10(t.CONST_PRESCALER);
    printStr("\n\tt.VALUE_PRx (PR2) = ");
    printInt10(t.VALUE_PRx);
    printStr("\n\n");
}

void configTimerT3(int fout) {
    struct TimerConstants t = getTimerConstantsTimerTypeB(fout);

    // Configure Timer T3
    T3CONbits.TCKPS = t.CONST_PRESCALER; // 1:K prescaler
    PR3 = t.VALUE_PRx;                   // Fout = f_PBCLK / (k_PRESCALER * (62499 + 1))
    TMR3 = 0;                            // Reset timer T3 count register
    T3CONbits.TON = 1;                   // Enable timer T3 (must be the last command)

    // Debug
    printStr("Timer 3 Config (Debug Info): \n\tfout = ");  printInt10(fout);
    printStr("\n\tt.FREQ_PRESCALER = ");                   printInt10(t.FREQ_PRESCALER);
    printStr("\n\tt.CONST_PRESCALER (TCKPS) = ");          printInt10(t.CONST_PRESCALER);
    printStr("\n\tt.VALUE_PRx (PR3) = ");                  printInt10(t.VALUE_PRx);
    printStr("\n\n");
}

void configInterrupts_TimerT1(){
    IPC1bits.T1IP = 2; 
    IEC0bits.T1IE = 1;
    IFS0bits.T1IF = 0; 
}

void configInterrupts_TimerT2() {
    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
}

void configInterrupts_TimerT3() {
    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag
}
