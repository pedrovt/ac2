// --------------------------------
// Guide 9, comDrv Header File
// Computer Architecture II
// Pedro Teixeira, 84715, MIECT
// --------------------------------

void comDrv_flushRx(void);

void comDrv_flushTx(void);

void comDrv_putc(char ch);

char comDrv_getc(char *pchar);

void comDrv_puts(char *s);

void comDrv_config(unsigned int baudrate, char parity, unsigned int numStopBits);
