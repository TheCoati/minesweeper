#include "Controller.h"

#define NUNCHUK_ADDRESS 0x52

void Controller::begin() {
    Nunchuk.begin(NUNCHUK_ADDRESS);
}

/**
 * Get the direction of the controller.
 * @return The direction of the controller.
 */
void Controller::tick() {
    if (!Nunchuk.getState(NUNCHUK_ADDRESS)) {
        waitForNewInput = true;
    }

    if (!waitForNewInput && Nunchuk.state.joy_x_axis < 168 && Nunchuk.state.joy_x_axis > 40 && Nunchuk.state.joy_y_axis < 168 && Nunchuk.state.joy_y_axis > 40) {
        waitForNewInput = true;
    }

    if (waitForNewInput && Nunchuk.state.joy_x_axis < 40) {
        waitForNewInput = false;
        newInput = true;
        lastAction = LEFT;
    }

    if (waitForNewInput && Nunchuk.state.joy_x_axis > 168) {
        waitForNewInput = false;
        newInput = true;
        lastAction = RIGHT;
    }

    if (waitForNewInput && Nunchuk.state.joy_y_axis > 168) {
        waitForNewInput = false;
        newInput = true;
        lastAction = UP;
    }

    if (waitForNewInput && Nunchuk.state.joy_y_axis < 40) {
        waitForNewInput = false;
        newInput = true;
        lastAction = DOWN;
    }

    if (waitForNewInput && Nunchuk.state.z_button) {
        waitForNewInput = false;
        newInput = true;
        lastAction = PRIMARY;
    }

    if (waitForNewInput && Nunchuk.state.c_button) {
        waitForNewInput = false;
        newInput = true;
        lastAction = SECONDARY;
    }
}

bool Controller::available() {
    bool result = newInput;

    newInput = false;

    return result;
}

ControllerAction Controller::read() {
    return lastAction;
}

class Controller Controller;