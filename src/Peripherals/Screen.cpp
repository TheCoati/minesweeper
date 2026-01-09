#include "Screen.h"

#include <config.h>

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

    sdCard = SD.begin(SD_CS, SD_SCK_MHZ(25));

    if (!sdCard) {
        tft.setTextColor(ILI9341_RED);
        tft.println("[ERROR] SD card read failure.");

        _delay_ms(1000);
    }

    tft.setCursor(0, 0);
}

bool Screen::hasSDCard() const {
    return sdCard;
}

Adafruit_ImageReader& Screen::getReader() {
    return reader;
}

void Screen::drawButton(const char *image, uint8_t x, uint8_t y) {
    if (hasSDCard()) {
        getReader().drawBMP(image, tft, x, y);
    }
}

// Todo: Move to game specific helper?
void Screen::drawButtonCursor(uint8_t x, uint8_t y) {
    tft.fillRect(x, y, BUTTON_CURSOR_WIDTH, BUTTON_CURSOR_BORDER, BUTTON_CURSOR_COLOR); // Top
    tft.fillRect(x, y + (BUTTON_CURSOR_HEIGHT - BUTTON_CURSOR_BORDER), BUTTON_CURSOR_WIDTH, BUTTON_CURSOR_BORDER, BUTTON_CURSOR_COLOR);

    tft.fillRect(x, y, BUTTON_CURSOR_BORDER, BUTTON_CURSOR_HEIGHT, BUTTON_CURSOR_COLOR); // Top border
    tft.fillRect(x + (BUTTON_CURSOR_WIDTH - BUTTON_CURSOR_BORDER), y, BUTTON_CURSOR_BORDER, BUTTON_CURSOR_HEIGHT, BUTTON_CURSOR_COLOR); // Bottom border
}

class Screen Screen;