// --------------------------------
// Guide 10, Parte 1
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# include <detpic32.h>
# include "../Utils/utils.h"
# include "../Utils/i2c.c"

# define SENS_ADDRESS 0x4D    // device dependent
# define ADDR_WR ((SENS_ADDRESS << 1) | I2C_WRITE)
# define ADDR_RD ((SENS_ADDRESS << 1) | I2C_READ)
# define TC74_CLK_FREQ 100000 // 100 KHz
# define RTR 0                // Read temperature command

int main(void) {

  TRISDbits.TRISD8 = 0;
  int ack, temperature;
  i2c1_init(TC74_CLK_FREQ);

  while (1) {
    LATDbits.LATD8 = 0;

    // Send Start event
    i2c1_start();

    // Send Address + WR (ADDR_WR); copy return value to "ack" variable
    //i2c1_send(SENS_ADDRESS);
    ack = i2c1_send(ADDR_WR);

    // Send Command (RTR); add return value to "ack" variable
    ack += i2c1_send(RTR);

    // Send Start event (again)
    i2c1_start();

    // Send Address + RD (ADDR_RD); add return value to "ack" variable
    //i2c1_send(SENS_ADDRESS);
    ack += i2c1_send(ADDR_RD);

    // Test "ack" variable; if "ack" != 0 then an error has occurred;
    if (ack != 0) {
      // send the Stop event, print an error message and exit loop
      i2c1_stop();
      printStr("Error!\n");
      break;
    }

    // Receive a value from slave (send NACK as argument); copy
    // received value to "temperature" variable
    temperature = i2c1_receive(I2C_NACK);

    // Send Stop event
    i2c1_stop();

    // Print "temperature" variable (syscall printInt10)
    printStr("\n");
    printInt10(temperature);

    // Wait 250 ms
    delay(1);

    LATDbits.LATD8 = 1;
  }

  return 1;
}
