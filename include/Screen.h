#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

uint8_t dotX = 120; //scherm is 240x320 -> gedeeld door 2 dus 120 en 160 is midden
uint16_t dotY = 160; //deze moet 16 bits zijn helaas, max waarde komt boven de 255 uit (320).

void clearScreen() {
    tft.fillScreen(ILI9341_BLACK); //scherm "wipen"
    dotX = 120; //standaard waardes van de x en y zodat de "cursor" weer vanaf het midden begint
    dotY = 160;
    tft.fillCircle(dotX, dotY, 2, ILI9341_WHITE); //witte cursor weer in het midden
}