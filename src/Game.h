#ifndef GAME_H_
#define GAME_H_

#include <avr/io.h>

extern uint8_t grid[41];
extern uint8_t seedNumber;

void fillField();
uint8_t getFieldValue(uint8_t index);

#endif