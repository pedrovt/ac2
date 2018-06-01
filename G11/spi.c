// --------------------------------
// SPI Module
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
void spi2_setClock(unsigned int clock_freq) {
  // Write SPI2BRG register (see introduction for details)
  SPI2BRG = (PBCLK + clock_freq) / (2 * clock_freq) - 1;
}

void spi2_init(void) {
  volatile char trash;

  // Disable SPI2 module
  SPI2CONbits.ON = 0;             // SPI Reference Manual, page 9, bit 15

  // Configure clock idle state as logic level 0 (see introduction)
  SPI2CONbits.CKP = 0;            // SPI Reference Manual, page 9, bit 6

  // Configure the clock active transition: from active
  // state to idle state (1 > 0)
  SPI2CONbits.CKE = 1;            // SPI Reference Manual, page 9, bit 8

  // Configure SPI data input sample phase bit (middle of data
  // output time)
  SPI2CONbits.SMP = 0;            // SPI Reference Manual, page 9, bit 9

  // Configure word length (8 bits)
  SPI2CONbits.MODE32 = 0;          // SPI Reference Manual, page 9, bit 11-10
  SPI2CONbits.MODE16 = 0;

  // Enable Enhanced buffer mode (this allows the usage of FIFOs RX,TX)
  SPI2CONbits.ENHBUF = 1;
  
  // Enable slave select support (Master Mode Slave Select)
  SPI2CONbits.MSSEN = 1; // SPI Reference Manual page 8, bit 28

  // Enable master mode
  SPI2CONbits.MSTEN = 1;           // SPI Reference Manual, page 9, bit 5

  // Clear RX FIFO:
  while (SPI2STATbits.SPIRBE == 0) // while RX FIFO not empty read
    trash = SPI2BUF;               // FIFO (discard data)
  
  // Clear overflow error flag
  SPI2STATbits.SPIROV = 0;         // SPI Reference Manual, page 13, bit 6

  // Enable SPI2 module
  SPI2CONbits.ON = 1;
}
