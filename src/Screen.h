#ifndef SCREEN_H_
#define SCREEN_H_

#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Imagereader.h>
#include "Game.h"

// For the Adafruit shield, these are the default.
#define SD_CS 4
#define TFT_DC 9
#define TFT_CS 10

extern uint8_t squaresX; //startwaardes voor de x en y zodat we weten waar het eerste vierkantje moet komen
extern uint8_t squaresY;
extern uint8_t stap; //stapgrootte voor de vierkantjes
extern uint8_t huidigvakje;

extern Adafruit_ILI9341 tft;
extern SdFat                SD;

void clearScreen();
void onthulVakje(uint8_t vakje);
uint8_t getVakje(uint8_t vakje);
void checkForMine();
void gridRender();
void initScreen();
void cursorX(uint8_t x);
void cursorY(uint8_t y);

#endif