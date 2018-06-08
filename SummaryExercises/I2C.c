// --------------------------------
// I2C & Using a Temp Sensor
// with I2C interface

// Prints the Temperature
// obtained from the sensor

// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

#include <detpic32.h>

#define SENS_ADDRESS 0x4D // device dependent
#define ADDR_WR ((SENS_ADDRESS << 1) | I2C_WRITE)
#define ADDR_RD ((SENS_ADDRESS << 1) | I2C_READ)
#define TC74_CLK_FREQ 100000 // 100 KHz
#define RTR 0                // Read temperature command

#define I2C_READ 1
#define I2C_WRITE 0
#define I2C_ACK 0
#define I2C_NACK 1

// I2C -----------------------------------------------------------
// Init I2C Module
void i2c1_init(unsigned int clock_freq)
{
    // Config baudrate generator (see introduction for details)
    I2C1BRG = (PBCLK + clock_freq) / (2 * clock_freq) - 1;

    // Enable I2C1 module
    I2C1CONbits.ON = 1;
}

// Generate the Start Event
// 1) ativar o start; 2) esperar que o evento termine.
void i2c1_start(void)
{
    // Wait until the lower 5 bits of I2CxCON are all 0 (the lower 5 bits
    // of I2CxCON must be 0 before attempting to set the SEN bit)
    while ((I2C1CON & 0x001F) != 0);

    // Activate Start event (I2C1CON, bit SEN)
    I2C1CONbits.SEN = 1;

    // Wait for completion of the Start event (I2C1CON, bit SEN)
    while (I2C1CONbits.SEN == 1);
}

// Generate the Stop Event
void i2c1_stop(void)
{
    // Wait until the lower 5 bits of I2CxCON are all 0 (the lower 5 bits
    // of I2CxCON must be 0 before attempting to set the PEN bit)
    while ((I2C1CON & 0x001F) != 0);

    // Activate Stop event (I2C1CON, bit PEN)
    I2C1CONbits.PEN = 1;

    // Wait for completion of the Stop event (I2C1CON, bit PEN)
    while (I2C1CONbits.PEN == 1);
}

// Send a byte
int i2c1_send(unsigned char value)
{
    // Copy "value" to I2C1TRN register
    I2C1TRN = value;

    // Wait while master transmission is in progress (8 bits + ACK\)
    // (I2C1STAT, bit TRSTAT – transmit status bit)
    while (I2C1STATbits.TRSTAT == 1);

    // Return acknowledge status bit (I2C1STAT, bit ACKSTAT)
    return I2C1STATbits.ACKSTAT;
}

// Receive a byte
char i2c1_receive(char ack_bit)
{
    // Wait util the lower 5 bits of I2C1CON are all 0 (the lower 5 bits
    // of I2C1CON must be 0 before attempting to set the RCEN bit)
    while ((I2C1CON & 0x001F) != 0);

    // Activate RCEN bit (receive enable bit, I2C1CON register)
    I2C1CONbits.RCEN = 1;

    // Wait while byte not received (I2C1STAT, bit RBF – receive buffer
    // full status bit)
    while (I2C1STATbits.RBF == 1);

    // Send ACK / NACK bit. For that:
    // 1. Copy "ack_bit" to I2C1CON, bit ACKDT (be sure "ack_bit" value
    // is only 0 or 1)
    if (ack_bit != 0 && ack_bit != 1)
        ack_bit = 1; // TODO
    I2C1CONbits.ACKDT = ack_bit;

    // 2. Wait until the lower 5 bits of I2C1CON are all 0 (the lower
    // 5 bits of I2C1CON must be 0 before attempting to
    // set the ACKEN bit.
    while ((I2C1CON & 0x001F) != 0);

    // 3. Start Acknowledge sequence (I2C1CON register, bit ACKEN=1)
    I2C1CONbits.ACKEN = 1;

    // Wait for completion of Acknowledge sequence (I2C1CON, bit ACKEN)
    while (I2C1CONbits.ACKEN == 1);

    // Return received value (I2C1RCV)
    return I2C1RCV;
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

// ---------------------------------------------------------------
int main(void)
{

    TRISDbits.TRISD8 = 0;
    int ack, temperature;
    i2c1_init(TC74_CLK_FREQ);

    while (1)
    {
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
        if (ack != 0)
        {
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
