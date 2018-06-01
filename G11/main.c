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
    char status;
   
    // Config SPI Module
    spi2_init();
    spi2_setClock(EEPROM_CLOCK);

    // Activate write operations
    eeprom_writeStatusCommand(WREN);
    
    while (1) {
      status = eeprom_readStatus();
     // printInt10(status);
      printInt(status, 2 | 4 << 16);
      printStr("\n");
      delay(100);
    }
  
  return 1;
}
