#include <DemoLED.h>
#include <Screen.h>
#include <Nunchuk.h>

#define NUNCHUK_ADDRESS 0x52

uint8_t firstinput = 0;

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

    if (firstinput && Nunchuk.state.joy_x_axis < 168 && Nunchuk.state.joy_x_axis > 40 && Nunchuk.state.joy_y_axis < 168 && Nunchuk.state.joy_y_axis > 40) {
        firstinput = 0;
    }

    //rechts
    if (!firstinput && Nunchuk.state.joy_x_axis > 168 && squaresX < (stap*8)+5) { //chuk max waarde 254, midden 80 -> (254-80)/80 + 80 = ~168 (50% deadzone) & mag niet van scherm af
        cursorX(1); //beweeg cursor rechts
        firstinput = 1;
    }

    //links
    if (!firstinput && Nunchuk.state.joy_x_axis < 40 && squaresX > 5) { //chuk min waarde 0, midden 80 -> 80/2 = 40 (50% deadzone) & mag niet van scherm af
        cursorX(0); //beweeg cursor links
        firstinput = 1;
    }

    //boven
    if (!firstinput && Nunchuk.state.joy_y_axis > 168 && squaresY > 12) { //chuk max waarde 254, midden 80 -> (254-80)/80 + 80 = ~168 (50% deadzone) & mag niet van scherm af
        cursorY(0); //beweeg cursor boven
        firstinput = 1;
    }

    //onder
    if (!firstinput && Nunchuk.state.joy_y_axis < 40 && squaresY < (stap*8)+12) { //chuk min waarde 0, midden 80 -> 80/2 = 40 (50% deadzone) & mag niet van scherm af
        cursorY(1); //beweeg cursor onder
        firstinput = 1;
    }

    if (Nunchuk.state.z_button) { //schermpie resetten als er op de Z knop wordt gedrukt
        squaresX = 5;
        squaresY = 12;
        gridRender();
        tft.fillRect(squaresX, squaresY, 24, 24, ILI9341_BLUE); //cursor
    }

//    if (Nunchuk.state.c_button) { //schermpie resetten als er op de Z knop wordt gedrukt
//        sendBit();
//    }
}