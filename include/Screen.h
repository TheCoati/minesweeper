#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

uint8_t squaresX = 5; //startwaardes voor de x en y zodat we weten waar het eerste vierkantje moet komen
uint8_t squaresY = 12;

uint8_t stap = 24; //stapgrootte voor de vierkantjes

void gridRender() {
    tft.fillScreen(ILI9341_BLACK); //scherm "wipen"
    for (uint8_t i = 0; i < 81; i++) { //81 vakjes totaal (9x9)
        tft.fillRect(squaresX, squaresY, 24, 24, ILI9341_RED); //vierkant renderen
        squaresX += stap; //huidige x en waarde met een stap verhogen zodat er geen overlap is
        if ((i+1) % 9 == 0) { //rows van 9 vakjes breed
            squaresX = 5; //x opnieuw resetten
            squaresY += stap; //huidige y en waarde met een stapgrootte van 24 (breedte/hoogte) verhogen zodat er geen overlap is
        }
    }
    squaresX = 5; //cursor reset
    squaresY = 12;
}

void initScreen() {
    tft.setRotation(1); //scherm op goede orientatie zetten
    gridRender();
    tft.fillRect(squaresX, squaresY, 24, 24, ILI9341_BLUE); //cursor
}

void cursorX(uint8_t x) {
    if (x) {
        tft.fillRect(squaresX += stap, squaresY, 24, 24, ILI9341_BLUE); //move cursor
        tft.fillRect(squaresX - stap, squaresY, 24, 24, ILI9341_RED); //zet vorig vakje weer normaal
    } else {
        tft.fillRect(squaresX -= stap, squaresY, 24, 24, ILI9341_BLUE); //move cursor
        tft.fillRect(squaresX + stap, squaresY, 24, 24, ILI9341_RED); //zet vorig vakje weer normaal
    }
}

void cursorY(uint8_t y) {
    if (y) {
        tft.fillRect(squaresX, squaresY += stap, 24, 24, ILI9341_BLUE); //move cursor
        tft.fillRect(squaresX, squaresY - stap, 24, 24, ILI9341_RED); //zet vorig vakje weer normaal
    } else {
        tft.fillRect(squaresX, squaresY -= stap, 24, 24, ILI9341_BLUE); //move cursor
        tft.fillRect(squaresX, squaresY + stap, 24, 24, ILI9341_RED); //zet vorig vakje weer normaal
    }
}