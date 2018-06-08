// --------------------------------
// SPI & Using a EEPROM
// with SPI interface

// Manages the EEPROM 
// (read status, read & write data)

// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>

# define READ 0x03    // Read  Data from Memory
# define WRITE 0x02   // Write Data to   Memory

# define RDSR 0x05    // Read  Status Register
# define WRSR 0x01    // Write Status Register

# define WREN 0x06    // Enable  Write Operations
# define WRDI 0x04    // Disable Write Operations
# define EEPROM_CLOCK 500000

// SPI -----------------------------------------------------------
void spi2_setClock(unsigned int clock_freq)
{
  // Write SPI2BRG register (see introduction for details)
  SPI2BRG = (PBCLK + clock_freq) / (2 * clock_freq) - 1;
}

void spi2_init(void)
{
  volatile char trash;

  // Disable SPI2 module
  SPI2CONbits.ON = 0; // SPI Reference Manual, page 9, bit 15

  // Configure clock idle state as logic level 0 (see introduction)
  SPI2CONbits.CKP = 0; // SPI Reference Manual, page 9, bit 6

  // Configure the clock active transition: from active
  // state to idle state (1 > 0)
  SPI2CONbits.CKE = 1; // SPI Reference Manual, page 9, bit 8

  // Configure SPI data input sample phase bit (middle of data
  // output time)
  SPI2CONbits.SMP = 0; // SPI Reference Manual, page 9, bit 9

  // Configure word length (8 bits)
  SPI2CONbits.MODE32 = 0; // SPI Reference Manual, page 9, bit 11-10
  SPI2CONbits.MODE16 = 0;

  // Enable Enhanced buffer mode (this allows the usage of FIFOs RX,TX)
  SPI2CONbits.ENHBUF = 1;

  // Enable slave select support (Master Mode Slave Select)
  SPI2CONbits.MSSEN = 1; // SPI Reference Manual page 8, bit 28

  // Enable master mode
  SPI2CONbits.MSTEN = 1; // SPI Reference Manual, page 9, bit 5

  // Clear RX FIFO:
  while (SPI2STATbits.SPIRBE == 0) // while RX FIFO not empty read
    trash = SPI2BUF;               // FIFO (discard data)

  // Clear overflow error flag
  SPI2STATbits.SPIROV = 0; // SPI Reference Manual, page 13, bit 6

  // Enable SPI2 module
  SPI2CONbits.ON = 1;
}

// EEPROM ---------------------------------------------------------
char eeprom_readStatus(void)
{
  volatile char trash;
  // Clear RX FIFO (discard all data in the reception FIFO)
  while (SPI2STATbits.SPIRBE == 0) // while RX FIFO not empty read
    trash = SPI2BUF;               // FIFO (discard data)                         
  SPI2STATbits.SPIROV = 0;         // Clear overflow error flag bit

  SPI2BUF = RDSR; // Send RDSR command
  SPI2BUF = 0;    // Send anything so that EEPROM clocks data into SO

  while (SPI2STATbits.SPIBUSY)
    ; // wait while SPI module is working

  trash = SPI2BUF; // First char received is garbage (received while
                   // sending command)

  return SPI2BUF; // Second received character is the STATUS value
}

void eeprom_writeStatusCommand(char command)
{
  while (eeprom_readStatus() & 0x01); // Wait while there's a write in progress

  SPI2BUF = command; // Copy "command" value to SPI2BUF (TX FIFO)

  while (SPI2STATbits.SPIBUSY); // Wait while SPI module is working (SPIBUSY set)
}

void eeprom_writeData(int address, char value)
{
  // Apply a mask to limit address to 9 bits
  address = address & 0x01FF;

  // Read STATUS and wait while there's a write in progress
  while (eeprom_readStatus() & 0x01); // ATTENTION to the mask

  // Enable write operations (activate WEL bit in STATUS register, using eeprom_writeStatusCommand() function)
  eeprom_writeStatusCommand(WREN);

  // Copy WRITE command and A8 address bit to the TX FIFO:
  SPI2BUF = WRITE | ((address & 0x100) >> 5);

  // Copy address (8 LSBits) to the TX FIFO
  SPI2BUF = address & 0x00FF;

  // Copy "value" to the TX FIFO
  SPI2BUF = value;

  // Wait while SPI module is working (SPIBUSY)
  while (SPI2STATbits.SPIBUSY);
}

char eeprom_readData(int address)
{
  volatile char trash;
  // Clear RX FIFO
  while (SPI2STATbits.SPIRBE == 0) // while RX FIFO not empty read
    trash = SPI2BUF;               // FIFO (discard data)

  // Clear overflow error flag bit
  SPI2STATbits.SPIROV = 0;

  // Apply a mask to limit address to 9 bits
  address = address & 0x01FF;

  // Read STATUS and wait while WIP is true (write in progress)
  while (eeprom_readStatus() & 0x01);

  // Copy READ command and A8 address bit to the TX FIFO
  SPI2BUF = READ | ((address & 0x100) >> 5);

  // Copy address (8 LSBits) to the TX FIFO
  SPI2BUF = address & 0x00FF;

  // Copy any value (e.g. 0x00) to the TX FIFO
  SPI2BUF = 0x00;

  // Wait while SPI module is working (SPIBUSY)
  while (SPI2STATbits.SPIBUSY);

  // Read and discard 2 characters from RX FIFO (use "trash" variable)
  trash = SPI2BUF;
  trash = SPI2BUF;

  // Read RX FIFO and return the corresponding value
  return SPI2BUF;
}

// ---------------------------------------------------------------
// Other aux functions
void delay(int ms)
{
  for (; ms > 0; ms--)
  {
    resetCoreTimer();
    while (readCoreTimer() < 20000);
  }
}

// Main -----------------------------------------------------------
int main(void)
{
  // Config SPI Module
  spi2_init();
  spi2_setClock(EEPROM_CLOCK);

  // Activate write operations
  eeprom_writeStatusCommand(WREN);
  printStr("\nEEPROM\nInsert an option:\n1. Read Status (S/s)\n2. Read Data (R/r)\n3. Write Data (W/w)\n4. Exit\n");

  while (1)
  {
    // Read a char
    char readChar = inkey();
    if (readChar != 0)
    {
      // Status
      if ((readChar == '1') | (readChar == 's') | (readChar == 'S'))
      {
        char status = eeprom_readStatus();
        printStr("\nStatus: ");
        printInt(status, 2 | 4 << 16);
        printStr("\n");
        delay(100);
      }

      // Read
      // Se for 'R' (read) lê um endereço (addr), e imprime o valor lido da memória.
      else if ((readChar == '2') | (readChar == 'r') | (readChar == 'R'))
      {
        printStr("\nRead\n-> Address to read: ");
        int address = readInt10();
        char value = eeprom_readData(address);
        printStr("\n-> Value: ");
        putChar(value);
        printStr("\n-> Done!\n");
      }

      // Write
      // Se for 'W' (write) lê um endereço e um valor (addr, val), e escreve na EEPROM no endereço addr o valor val.
      else if ((readChar == '3') | (readChar == 'w') | (readChar == 'W'))
      {
        printStr("\nWrite\n-> Address to write: ");
        int address = readInt10();
        printStr("\n-> Value to write: ");
        char value = getChar();
        putChar(value);
        eeprom_writeData(address, value);
        printStr("\n-> Done!\n");
      }

      else if (readChar == '4') {
        return 0;
      }
      else {
        printStr("\nInvalid Option!\n");
      }

      printStr("\nEEPROM\nInsert an option:\n1. Read Status (S/s)\n2. Read Data (R/r)\n3. Write Data (W/w)\n4. Exit\n");
    }
  }

  return 1;
}
