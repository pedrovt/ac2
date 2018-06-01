// --------------------------------
// EEPROM Module
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>

# define READ  0x03         // Read  Data from Memory
# define WRITE 0x02         // Write Data to   Memory

# define RDSR  0x05         // Read  Status Register
# define WRSR  0x01         // Write Status Register

# define WREN  0x06         // Enable  Write Operations
# define WRDI  0x04         // Disable Write Operations

// Read Status Register
char eeprom_readStatus(void) {
    volatile char trash;

    SPI2BUF = 0;                          // Clear RX FIFO (discard all data in the reception FIFO)
    SPI2STATbits.SPIROV = 0;              // Clear overflow error flag bit

    SPI2BUF = RDSR;                       // Send RDSR command
    SPI2BUF = 0;                          // Send anything so that EEPROM clocks data into SO

    while (SPI2STATbits.SPIBUSY);         // wait while SPI module is working

    trash = SPI2BUF;                      // First char received is garbage (received while
                                          // sending command)

    return SPI2BUF;                       // Second received character is the STATUS value
}

void eeprom_writeStatusCommand(char command) {
    while (eeprom_readStatus() & WRSR);   // Wait while there's a write in progress
    
    SPI2BUF = command;                    // Copy "command" value to SPI2BUF (TX FIFO)

    while (SPI2STATbits.SPIBUSY);         // Wait while SPI module is working (SPIBUSY set)
}

void eeprom_writeData(int address, char value) {
    // Apply a mask to limit address to 9 bits
    address = address & 0x01FF;

    // Read STATUS and wait while there's a write in progress
    while (eeprom_readStatus() & 0x01);     // ATTENTION to the mask

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

char eeprom_readData(int address) {
    volatile char trash;
    // Clear RX FIFO
    SPI2BUF = 0;

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
