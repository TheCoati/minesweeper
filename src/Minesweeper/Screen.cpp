#include "Screen.h"

#define SD_CS 4
#define TFT_DC 9
#define TFT_CS 10

#define GRID_SIZE 9

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
SdFat SD;
Adafruit_ImageReader reader(SD);

bool hasSDCard = false;

uint8_t segmentGetal[11] = {
    0b11000000, //0
    0b11111001, //1
    0b10100100, //2
    0b10110000, //3
    0b10011001, //4
    0b10010010, //5
    0b10000010, //6
    0b11111000, //7
    0b10000000, //8
    0b10010000, //9
    0b11111111, //uit
};

/**
 * Initializes the screen.
 */
void initScreen() {
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("[INFO] Booting Minesweeper...");

    hasSDCard = SD.begin(SD_CS, SD_SCK_MHZ(25));

    if (!hasSDCard) {
        tft.setTextColor(ILI9341_RED);
        tft.println("[ERROR] SD card read failure.");

        _delay_ms(1000);
    }
}

void updateDisplay(uint8_t value) {
    Wire.beginTransmission(0x21); //7-segment display aanpassen
    Wire.write(segmentGetal[value]);
    Wire.endTransmission();
}

/**
 * Converts an index to x and y coordinates.
 * @param index The index to convert.
 * @param x The x coordinate of the index.
 * @param y The y coordinate of the index.
 */
inline void indexToCoord(uint8_t index, uint8_t &x, uint8_t &y) {
    x = index % GRID_SIZE;
    y = index / GRID_SIZE;

    if (hasSDCard) {
        x *= 24;
        y *= 24;

        x += 4;
        y += 4;
    } else {
        x *= 25;
        y *= 25;
    }
}

/**
 * Get the image path of a field based on its value.
 * @param value The value of the field.
 * @return The image path of the field.
 */
String getFieldImage(int value) {
    if (value == 0) {
        return "/open.bmp";
    }

    if (value == 9) {
        return "/mine.bmp";
    }

    return "/open_" + String(value) + ".bmp";
}

/**
 * Get the color of a field based on its value.
 * @param value The value of the field.
 * @return The color of the field.
 */
uint16_t getFieldColor(int value) {
    switch (value) {
        case 0:
            return ILI9341_WHITE;
        case 1:
            return ILI9341_BLUE;
        case 2:
            return ILI9341_GREEN;
        case 3:
            return ILI9341_YELLOW;
        case 4:
            return ILI9341_PINK;
        case 5:
            return ILI9341_ORANGE;
        case 6:
            return ILI9341_PURPLE;
        case 7:
            return ILI9341_CYAN;
        case 8:
            return ILI9341_LIGHTGREY;
        case 9:
            return ILI9341_DARKGREY;
    }

    return ILI9341_WHITE;
}

/**
 * Draws an open field on the screen.
 * @param index The index of the field to draw.
 * @param value The value of the field to draw.
 */
void drawOpen(uint8_t index, uint8_t value) {
    uint8_t x, y;
    indexToCoord(index, x, y);

    if (hasSDCard) {
        String path = getFieldImage(value);
        reader.drawBMP(path.c_str(), tft, x, y);
        return;
    }

    uint16_t color = getFieldColor(value);
    tft.fillRect(x, y, 24, 24, color);

    if (value > 0) {
        tft.setCursor(x + 10, y + 7);
        tft.println(String(value));
    }
}

/**
 * Draws a closed field on the screen.
 * @param index The index of the field to draw.
 */
void drawClosed(uint8_t index) {
    uint8_t x, y;
    indexToCoord(index, x, y);

    if (hasSDCard) {
        reader.drawBMP("/slot.bmp", tft, x, y);
        return;
    } else {
        tft.fillRect(x, y, 24, 24, ILI9341_BLACK);
    }
}

/**
 * Draws the entire field on the screen at the beginning of the game.
 */
void drawField() {
    tft.fillScreen(ILI9341_WHITE);

    if (hasSDCard) {
        reader.drawBMP("/outline.bmp", tft, 0, 0);
    }
}

/**
 * Draws the cursor on the screen.
 * @param index The index of the cursor to draw.
 */
void drawCursor(uint8_t index) {
    uint8_t x, y;
    indexToCoord(index, x, y);
    tft.fillRect(x, y, 24, 3, ILI9341_BLACK); //bovenste lijn
    tft.fillRect(x, y + 21, 24, 3, ILI9341_BLACK); //onderste lijn
    tft.fillRect(x, y, 3, 24, ILI9341_BLACK); //rechter lijn
    tft.fillRect(x + 21, y, 3, 24, ILI9341_BLACK); //linker lijn
}

/**
 * Draws the main menu on the screen.
 */
void drawMenu() {
    // Todo
    tft.fillScreen(ILI9341_WHITE);
    tft.setCursor(10, 10);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_BLACK);
    tft.println("Druk op Z om de game te starten.");
}
