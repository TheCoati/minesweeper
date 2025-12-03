#include <DemoLED.h>
#include <Screen.h>
#include <IR.h>
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

    if (firstinput && Nunchuk.state.joy_x_axis < 168 && Nunchuk.state.joy_y_axis > 40 ) {
        firstinput = 0;
    }

    if (!firstinput && Nunchuk.state.joy_x_axis > 168 && squaresX <= 236) { //chuk max waarde 254, midden 80 -> (254-80)/80 + 80 = ~168 (50% deadzone) & mag niet van scherm af
        tft.fillRect(squaresX += stap, squaresY, 24, 24, ILI9341_BLUE); //move cursor
        tft.fillRect(squaresX - stap, squaresY, 24, 24, ILI9341_RED); //zet vorig vakje weer normaal
        firstinput = 1;
    }

    if (!firstinput && Nunchuk.state.joy_x_axis < 40 && squaresX >= 5) { //chuk min waarde 0, midden 80 -> 80/2 = 40 (50% deadzone) & mag niet van scherm af
        tft.fillRect(squaresX -= stap, squaresY, 24, 24, ILI9341_BLUE); //move cursor
        tft.fillRect(squaresX + stap, squaresY, 24, 24, ILI9341_RED); //zet vorig vakje weer normaal
        firstinput = 1;
    }

    if (!firstinput && Nunchuk.state.joy_y_axis > 168 && squaresY >= 5) { //chuk max waarde 254, midden 80 -> (254-80)/80 + 80 = ~168 (50% deadzone) & mag niet van scherm af
        tft.fillRect(squaresX, squaresY -= stap, 24, 24, ILI9341_BLUE); //move cursor
        tft.fillRect(squaresX, squaresY + stap, 24, 24, ILI9341_RED); //zet vorig vakje weer normaal
        firstinput = 1;
    }

    if (!firstinput && Nunchuk.state.joy_y_axis < 40 && squaresY <= 236) { //chuk min waarde 0, midden 80 -> 80/2 = 40 (50% deadzone) & mag niet van scherm af
        tft.fillRect(squaresX, squaresY += stap, 24, 24, ILI9341_BLUE); //move cursor
        tft.fillRect(squaresX, squaresY - stap, 24, 24, ILI9341_RED); //zet vorig vakje weer normaal
        firstinput = 1;
    }

    if (Nunchuk.state.z_button) { //schermpie resetten als er op de Z knop wordt gedrukt
        clearScreen();
    }

    if (Nunchuk.state.c_button) { //schermpie resetten als er op de Z knop wordt gedrukt
        sendBit();
    }
}