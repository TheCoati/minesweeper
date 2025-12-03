#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <Minenet.h>
#include <avr/delay.h>

#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int main(void) {
    init();

    tft.begin();
    tft.fillScreen(ILI9341_BLACK);

    Minenet.begin();

    while (true) {
        if (Minenet.available()) {
            MinenetPacket packet = Minenet.read();

            tft.println(packet.payload);
        }

        _delay_ms(500);
    }
}
