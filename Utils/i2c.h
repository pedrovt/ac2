// --------------------------------
// i2c Header File
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

# ifndef _I2C_H
# define _I2C_H

# include <detpic32.h>

# define I2C_READ   1
# define I2C_WRITE  0
# define I2C_ACK    0
# define I2C_NACK   1

// Init I2C Module
void i2c1_init(unsigned int clock_freq);

// Generate the Start Event
void i2c1_start(void);

// Generate the Stop Event
void i2c1_stop(void);

// Send a byte
int i2c1_send(unsigned char value);

// Receive a byte 
char i2c1_receive(char ack_bit);

# endif
