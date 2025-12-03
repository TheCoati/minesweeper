#ifndef GAME_H_
#define GAME_H_

#include <avr/io.h>

extern bool grid[9][9];
extern uint8_t seedNumber;
extern uint8_t columnNumber;
extern uint8_t rowNumber;

void fillField();

#endif