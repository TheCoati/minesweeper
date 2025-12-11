#include "NunchuckControl.h"

#define NUNCHUK_ADDRESS 0x52

uint8_t firstInput = 0;
uint8_t firstZInput = 0;

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

    if (firstInput && Nunchuk.state.joy_x_axis < 168 && Nunchuk.state.joy_x_axis > 40 && Nunchuk.state.joy_y_axis < 168 && Nunchuk.state.joy_y_axis > 40) {
        firstInput = 0;
    }

    //rechts
    if (!firstInput && Nunchuk.state.joy_x_axis > 168 && squaresX < (stap*8)+4) { //chuk max waarde 254, midden 80 -> (254-80)/80 + 80 = ~168 (50% deadzone) & mag niet van scherm af
        cursorX(1); //beweeg cursor rechts
        firstInput = 1;
    }

    //links
    if (!firstInput && Nunchuk.state.joy_x_axis < 40 && squaresX > 4) { //chuk min waarde 0, midden 80 -> 80/2 = 40 (50% deadzone) & mag niet van scherm af
        cursorX(0); //beweeg cursor links
        firstInput = 1;
    }

    //boven
    if (!firstInput && Nunchuk.state.joy_y_axis > 168 && squaresY > 4) { //chuk max waarde 254, midden 80 -> (254-80)/80 + 80 = ~168 (50% deadzone) & mag niet van scherm af
        cursorY(0); //beweeg cursor boven
        firstInput = 1;
    }

    //onder
    if (!firstInput && Nunchuk.state.joy_y_axis < 40 && squaresY < (stap*8)+4) { //chuk min waarde 0, midden 80 -> 80/2 = 40 (50% deadzone) & mag niet van scherm af
        cursorY(1); //beweeg cursor onder
        firstInput = 1;
    }

    if (!Nunchuk.state.z_button && firstZInput) { //Z knop wordt losgelaten en
        firstZInput = 0;
    }

    if (!firstZInput && Nunchuk.state.z_button) { //schermpie resetten als er op de Z knop wordt gedrukt
        //initScreen();
        //tft.fillRect(squaresX, squaresY, 24, 24, ILI9341_BLUE); //cursor
        zPressed = 1;
        firstZInput = 1;
        onthulVakje(huidigvakje);
        checkForMine();
    }
}