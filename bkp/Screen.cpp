

//// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
//
//uint8_t dotX = 120; //scherm is 240x320 -> gedeeld door 2 dus 120 en 160 is midden
//uint16_t dotY = 160; //deze moet 16 bits zijn helaas, max waarde komt boven de 255 uit (320).
//
//void clearScreen() {
//    tft.fillScreen(ILI9341_BLACK); //scherm "wipen"
//    dotX = 120; //standaard waardes van de x en y zodat de "cursor" weer vanaf het midden begint
//    dotY = 160;
//    tft.fillCircle(dotX, dotY, 2, ILI9341_WHITE); //witte cursor weer in het midden
//}


#include "Screen.h"

// For the Adafruit shield, these are the default.
#define SD_CS 4
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

SdFat                SD;         // SD card filesystem
Adafruit_ImageReader reader(SD); // Image-reader object, pass in SD filesys


uint8_t squaresX = 4; //startwaardes voor de x en y zodat we weten waar het eerste vierkantje moet komen
uint8_t squaresY = 4;

uint8_t zPressed = 0; //of de Z knop ingedrukt is

uint8_t stap = 24; //stapgrootte voor de vierkantjes

uint8_t huidigvakje = 0;

uint8_t levens = 2;

uint8_t vakjes[11]; //met 11 bits kun je 88 vakjes opslaan (we gebruiken hiervan maar 81)

void drawNumber(int number, int x, int y) {
    switch (number) {
    case 0: reader.drawBMP("/open.bmp", tft, x, y); break;
    case 1: reader.drawBMP("/open_1.bmp", tft, x, y); break;
    case 2: reader.drawBMP("/open_2.bmp", tft, x, y); break;
    case 3: reader.drawBMP("/open_3.bmp", tft, x, y); break;
    case 4: reader.drawBMP("/open_4.bmp", tft, x, y); break;
    case 5: reader.drawBMP("/open_5.bmp", tft, x, y); break;
    case 6: reader.drawBMP("/open_6.bmp", tft, x, y); break;
    case 7: reader.drawBMP("/open_7.bmp", tft, x, y); break;
    case 8: reader.drawBMP("/open_8.bmp", tft, x, y); break;
    case 9: reader.drawBMP("/mine.bmp",   tft, x, y); break;
    default: break;
    }
}

void checkForMine() {
    if (getFieldValue(huidigvakje) == 9) {
        levens--;
        drawNumber(levens, 296, 0);
    }
    if (levens == 0) {
        drawNumber(9, 296, 0);
    }
}

void onthulVakje(uint8_t vakje) { //onthul permanent een vakje
    vakjes[vakje/8] |=  1 << (vakje % 8);  // zet bit
}

uint8_t getVakje(uint8_t vakje) {
    return vakjes[vakje/8] >> (vakje %  8) & 1;
}

void gridRender() {
    for (uint8_t i = 0; i < 81; i++) { //81 vakjes totaal (9x9)
        //tft.fillRect(squaresX, squaresY, 24, 24, ILI9341_RED); //vierkant renderen
        reader.drawBMP("/slot.bmp", tft, squaresX, squaresY);
        squaresX += stap; //huidige x en waarde met een stap verhogen zodat er geen overlap is
        if ((i+1) % 9 == 0) { //rows van 9 vakjes breed
            squaresX = 4; //x opnieuw resetten
            squaresY += stap; //huidige y en waarde met een stapgrootte van 24 (breedte/hoogte) verhogen zodat er geen overlap is
        }
    }
    squaresX = 4; //cursor reset
    squaresY = 4;
    fillField(seedNumber);
}

void initScreen() {
    squaresX = 4; //cursor reset
    squaresY = 4;
    tft.setRotation(1); //scherm op goede orientatie zetten
    tft.fillScreen(ILI9341_WHITE); //scherm "wipen"
    reader.drawBMP("/outline.bmp", tft, 0, 0);
    gridRender();
    //tft.fillRect(squaresX, squaresY, 24, 24, ILI9341_BLUE); //cursor
    reader.drawBMP("/selected.bmp", tft, squaresX, squaresY);
    reader.drawBMP("/open_2.bmp", tft, 296, 0);
}

void cursorX(uint8_t x) {
    if (x) {
        reader.drawBMP("/selected.bmp", tft, squaresX += stap, squaresY);
        huidigvakje++;
        if (!zPressed && !getVakje(huidigvakje - 1)) {
            reader.drawBMP("/slot.bmp", tft, squaresX - stap, squaresY);
        } else {
            drawNumber(getFieldValue(huidigvakje - 1), squaresX - stap, squaresY);
            //reader.drawBMP("/open.bmp", tft, squaresX - stap, squaresY);
            zPressed = 0;
        }
        // Minenet.send(0, 0, 0x01, 0x01);
    } else {
        reader.drawBMP("/selected.bmp", tft, squaresX -= stap, squaresY);
        huidigvakje--;
        if (!zPressed && !getVakje(huidigvakje + 1)) {
            reader.drawBMP("/slot.bmp", tft, squaresX + stap, squaresY);
        } else {
            drawNumber(getFieldValue(huidigvakje + 1), squaresX + stap, squaresY);
            //reader.drawBMP("/open.bmp", tft, squaresX + stap, squaresY);
            zPressed = 0;
        }
        // Minenet.send(0, 0, 0x01, 0x02);
    }
}

void cursorY(uint8_t y) {
    if (y) {
        reader.drawBMP("/selected.bmp", tft, squaresX, squaresY += stap);
        huidigvakje = huidigvakje + 9;
        if (!zPressed && !getVakje(huidigvakje - 9)) {
            reader.drawBMP("/slot.bmp", tft, squaresX, squaresY - stap);
        } else {
            drawNumber(getFieldValue(huidigvakje - 9), squaresX, squaresY - stap);
            //reader.drawBMP("/open.bmp", tft, squaresX, squaresY - stap);
            zPressed = 0;
        }
        // Minenet.send(0, 0, 0x01, 0x03);
    } else {
        huidigvakje = huidigvakje - 9;
        reader.drawBMP("/selected.bmp", tft, squaresX, squaresY -= stap);
        if (!zPressed && !getVakje(huidigvakje + 9)) {
            reader.drawBMP("/slot.bmp", tft, squaresX, squaresY + stap);
        } else {
            drawNumber(getFieldValue(huidigvakje + 9), squaresX, squaresY + stap);
            //reader.drawBMP("/open.bmp", tft, squaresX, squaresY + stap);
            zPressed = 0;
        }
        // Minenet.send(0, 0, 0x01, 0x04);
    }
}