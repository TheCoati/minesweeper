#ifndef MINESWEEPER_SCREEN_H
#define MINESWEEPER_SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_Imagereader.h>

extern Adafruit_ILI9341 tft;

void initScreen();
void drawField();
void drawOpen(uint8_t index, uint8_t value);
void drawClosed(uint8_t index);
void drawCursor(uint8_t index);
void drawMenu();
void updateDisplay(uint8_t value);

#endif // MINESWEEPER_SCREEN_H
