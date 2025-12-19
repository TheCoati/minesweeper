#include "Screen.h"

#define SD_CS 4
#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
SdFat SD;

Screen::Screen(): reader(SD) {
    //
}

void Screen::begin() {
    tft.begin();
    tft.setRotation(1);
    tft.setTextColor(ILI9341_WHITE);
    tft.fillScreen(ILI9341_BLACK);

    sdCard = SD.begin(SD_CS, SD_SCK_MHZ(25));

    if (!sdCard) {
        tft.setTextColor(ILI9341_RED);
        tft.println("[ERROR] SD card read failure.");

        _delay_ms(1000);
    }

    tft.setCursor(0, 0);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
}

bool Screen::hasSDCard() {
    return sdCard;
}

Adafruit_ImageReader& Screen::getReader() {
    return reader;
}

class Screen Screen;