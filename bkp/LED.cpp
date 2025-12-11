#include "LED.h"

uint8_t ledState = 255;
bool IRreceived = false;

void I2C_start(void) 
{
    // I2C instellen
    TWSR = 0x00; // prescaler = 1
    TWBR = 72; // 100 kHz
    TWCR = (1 << TWEN); // enable TWI
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
}

void I2C_stop(void) 
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void I2C_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
}

void I2C_WriteToAdress(uint8_t value, uint8_t adress) {
    I2C_start();
    I2C_write((adress << 1) | 0); // write mode
    I2C_write(value);
    I2C_stop();
}

ISR(INT0_vect)
{
    IRreceived = true;
}