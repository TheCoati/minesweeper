#ifndef MINESWEEPER_SCREEN_H
#define MINESWEEPER_SCREEN_H

#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Imagereader.h>

void initScreen();
void drawField();
void drawOpen(uint8_t index, uint8_t value);
void drawClosed(uint8_t index);
void drawCursor(uint8_t index);

#endif // MINESWEEPER_SCREEN_H
