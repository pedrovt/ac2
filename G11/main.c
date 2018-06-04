// --------------------------------
// Guide 11
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../Utils/utils.h"
# include "eeprom.c"
# include "spi.c"

# define READ  0x03         // Read  Data from Memory
# define WRITE 0x02         // Write Data to   Memory

# define RDSR  0x05         // Read  Status Register
# define WRSR  0x01         // Write Status Register

# define WREN  0x06         // Enable  Write Operations
# define WRDI  0x04         // Disable Write Operations
# define EEPROM_CLOCK 500000

int main(void) {
    // Config SPI Module
    spi2_init();
    spi2_setClock(EEPROM_CLOCK);

    // Activate write operations
    eeprom_writeStatusCommand(WREN);
    printStr("\nInsira uma opção:\n1. Read Status (S/s)\n2. Write Data (W/w)\n3. Read Data (R/r) \n");
    while (1) {
      // Lê um caracter
      char readChar = inkey();
      if (readChar != 0) {
        // Status
        if ((readChar == 's') | (readChar == 'S')){
          char status = eeprom_readStatus();
          printStr("\nStatus: ");
          printInt(status, 2 | 4 << 16);
          printStr("\n");
          delay(100);
        }

        // Read
        // Se for 'R' (read) lê um endereço (addr), e imprime o valor lido da memória.
        else if ((readChar == 'r') | (readChar == 'R')) {
          printStr("\nRead\n-> Address to read: ");
          int address = readInt10();
          char value = eeprom_readData(address);
          printStr("\n-> Value: ");
          putChar(value);
          printStr("\n-> Done!\n");
        }

        // Write
        // Se for 'W' (write) lê um endereço e um valor (addr, val), e escreve na EEPROM no endereço addr o valor val.
        else if ((readChar == 'w') | (readChar == 'W')) {
          printStr("\nWrite\n-> Address to write: ");
          int address = readInt10();
          printStr("\n-> Value to write: ");
          char value = getChar();
          putChar(value);
          eeprom_writeData(address, value);
          printStr("\n-> Done!\n");
        }
        
        else printStr("\nInvalid Option!\n");
        printStr("\nInsira uma opção:\n1. Read Status (S/s)\n2. Write Data (W/w)\n3. Read Data (R/r) \n");
      }
    }

   return 1;
}

