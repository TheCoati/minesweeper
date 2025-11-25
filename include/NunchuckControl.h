#include <DemoLED.h>
#include <Screen.h>
#include <IR.h>
#include <Nunchuk.h>

#define NUNCHUK_ADDRESS 0x52

void nunchukControl() {
    if (IRreceived)
    {
        ledState = ~ledState;
        I2C_WriteToAdress(ledState, PCF8564_ADDRESS);
        IRreceived = false;
    }
    if (!Nunchuk.getState(NUNCHUK_ADDRESS)) { //refreshen
         return;
    }

    if (Nunchuk.state.joy_x_axis > 168 && dotX < 240) { //chuk max waarde 254, midden 80 -> (254-80)/80 + 80 = ~168 (50% deadzone) & mag niet van scherm af
        tft.fillCircle(dotX++, dotY, 2, ILI9341_WHITE);
    }

    if (Nunchuk.state.joy_x_axis < 40 && dotX > 0) { //chuk min waarde 0, midden 80 -> 80/2 = 40 (50% deadzone) & mag niet van scherm af
        tft.fillCircle(dotX--, dotY, 2, ILI9341_WHITE);
    }

    if (Nunchuk.state.joy_y_axis > 168 && dotY > 0) {
        tft.fillCircle(dotX, dotY--, 2, ILI9341_WHITE);
    }

    if (Nunchuk.state.joy_y_axis < 40 && dotY < 320) {
        tft.fillCircle(dotX, dotY++, 2, ILI9341_WHITE);
    }

    if (Nunchuk.state.z_button) { //schermpie resetten als er op de Z knop wordt gedrukt
        clearScreen();
    }

    if (Nunchuk.state.c_button) { //schermpie resetten als er op de Z knop wordt gedrukt
        sendBit();
    }
}