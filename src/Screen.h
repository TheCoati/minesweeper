#ifndef SCREEN_H_
#define SCREEN_H_

#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

extern Adafruit_ILI9341 tft;
extern uint8_t dotX;
extern uint16_t dotY;

void clearScreen();

#endif