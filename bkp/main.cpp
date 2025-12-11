#include <SPI.h>
#include <avr/delay.h>
#include "Minenet.h"
#include "NunchuckControl.h"
#include "Screen.h"

// PD6 --> led
// PD2 --> sensor

//void InitializeIO()
//{
////  initializeIRIO();
//
//  // INT0 falling edge
//  EICRA |= (1 << ISC01);
//  EICRA &= ~(1 << ISC00);
//  EIMSK |= (1 << INT0);
//
//  sei();
//}



int main(void)
{
//  InitializeIO();
    init();
    Minenet.begin();
    Wire.begin();
    Nunchuk.begin(NUNCHUK_ADDRESS);
    tft.begin();
    SD.begin(SD_CS, SD_SCK_MHZ(25));
    initScreen();
//  InitializeIO();

    while(true)
    {
        if (Minenet.available()) {
            MinenetPacket packet = Minenet.read();

            if (packet.opCode == 0x01) {
                switch (packet.payload) {
                    case 0x01:
                        cursorX(0);
                        break;
                    case 0x02:
                        cursorX(1);
                        break;
                    case 0x03:
                        cursorY(0);
                        break;
                    case 0x04:
                        cursorY(1);
                        break;
                }
            }
        }

        nunchukControl();
    }
    return 0;
}