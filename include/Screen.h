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

uint8_t squaresX = 5; //startwaardes voor de x en y zodat we weten waar het eerste vierkantje moet komen
uint8_t squaresY = 5;

uint8_t stap = 26; //stapgrootte voor de vierkantjes

void firstRender() {
    tft.fillScreen(ILI9341_BLACK); //scherm "wipen"
    for (uint8_t i = 0; i < 81; ++i) {
        if (squaresX >= 236)
        {
            squaresY += stap; //huidige y en waarde met een stapgrootte van 24 (breedte/hoogte) verhogen zodat er geen overlap is
            squaresX = 5; //x opnieuw resetten
        }
        tft.fillRect(squaresY, squaresX, 24, 24, ILI9341_RED); //eerste vierkantje
        squaresX += stap; //huidige x en waarde met een stapgrootte van 24 (breedte/hoogte) verhogen zodat er geen overlap is
    }
    squaresX = 5; //cursor reset
    squaresY = 5;
}