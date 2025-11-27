#include <NunchuckControl.h>
#include <Minenet.h>
#include <Arduino.h>

void setup() {
    init();

    Minenet.begin();
    Nunchuk.begin(NUNCHUK_ADDRESS);

    tft.begin();
    clearScreen();
}

void loop() {
    nunchukControl();

    _delay_ms(500);
    Minenet.send(0x5, 0x01);
}
