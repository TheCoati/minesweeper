#include <MinesweeperIR.h>
#include <NunchuckControl.h>
#include <SPI.h>

int main() {
    init();

    Wire.begin();
    Minenet.begin(0xA);
    Nunchuk.begin(NUNCHUK_ADDRESS);

    tft.begin();
    clearScreen();

    while(true) {
//        _delay_ms(500);
//        Minenet.send(0x5, 0x00);

//        while (Minenet.available()) {
//            Serial.println(Minenet.getCommandData());
//            Serial.println(Minenet.getCommandType());
//        }

        nunchukControl();
    }
}
