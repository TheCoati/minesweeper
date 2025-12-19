#ifndef MINESWEEPER_SCREEN_H
#define MINESWEEPER_SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_Imagereader.h>

extern Adafruit_ILI9341 tft;

class Screen {
public:
    Screen();
    void begin();
    bool hasSDCard();
    Adafruit_ImageReader& getReader();
private:
    bool sdCard = false;
    Adafruit_ImageReader reader;
};

extern Screen Screen;

#endif //MINESWEEPER_SCREEN_H
