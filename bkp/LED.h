#ifndef LED_H_
#define LED_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define PCF8564_ADDRESS 0x21

extern uint8_t ledState;

extern bool IRreceived;

void I2C_start(void);
void I2C_stop(void);
void I2C_write(uint8_t data);
void I2C_WriteToAdress(uint8_t value, uint8_t adress);

#endif