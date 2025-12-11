#include "Controller.h"

#define NUNCHUK_ADDRESS 0x52

bool waitForNewInput = true;
ControllerDirection direction = IDLE;

/**
 * Initialize the controller.
 */
void initController() {
    Nunchuk.begin(NUNCHUK_ADDRESS);
}

/**
 * Get the direction of the controller.
 * @return The direction of the controller.
 */
ControllerDirection getJoystick() {
    if (!Nunchuk.getState(NUNCHUK_ADDRESS)) {
        // Todo: Alert missing input? [MOSCOW: C]
        waitForNewInput = true;
        return direction = IDLE;
    }

    if (!waitForNewInput && Nunchuk.state.joy_x_axis < 168 && Nunchuk.state.joy_x_axis > 40 && Nunchuk.state.joy_y_axis < 168 && Nunchuk.state.joy_y_axis > 40) {
        waitForNewInput = true;
        return direction = IDLE;
    }

    if (waitForNewInput && Nunchuk.state.joy_x_axis < 40) {
        waitForNewInput = false;
        return direction = LEFT;
    }

    if (waitForNewInput && Nunchuk.state.joy_x_axis > 168) {
        waitForNewInput = false;
        return direction = RIGHT;
    }

    if (waitForNewInput && Nunchuk.state.joy_y_axis > 168) {
        waitForNewInput = false;
        return direction = UP;
    }

    if (waitForNewInput && Nunchuk.state.joy_y_axis < 40) {
        waitForNewInput = false;
        return direction = DOWN;
    }

    return direction = IDLE;
}

/**
 * Check if the Z button is pressed.
 * @return State of the Z button.
 */
bool isPrimaryPressed() {
    if (!Nunchuk.getState(NUNCHUK_ADDRESS)) {
        return false;
    }

    return Nunchuk.state.z_button;
}

/**
 * Check if the C button is pressed.
 * @return State of the C button.
 */
bool isSecondaryPressed() {
    if (!Nunchuk.getState(NUNCHUK_ADDRESS)) {
        return false;
    }

    return Nunchuk.state.c_button;
}
