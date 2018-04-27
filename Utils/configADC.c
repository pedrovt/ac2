// --------------------------------
// Config ADC Function
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

void configADC(int numSamples) {
  // Step 3  - 4 samples will be converted and stored in buffer locations
  // ADC1BUF0 to ADC1BUF3
  AD1CON2bits.SMPI = numSamples - 1;

  // Step 4  - Conversion trigger selection bits: in this mode an internal
  // counter ends sampling and starts conversion
  AD1CON1bits.SSRC = 7;

  // Step 5 - Stop conversions when the 1st A/D converter interrupt is
  // generated. At the same time, hardware clears the ASAM bit.
  AD1CON1bits.CLRASAM = 1;

  // Step 6 - Sample time is 16 TAD (TAD = 100 ns)
  AD1CON3bits.SAMC = 16;

  // Step 7 - Enable A/D converter
  AD1CON1bits.ON = 1;
}
