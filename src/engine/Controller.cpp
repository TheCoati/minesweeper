#include "Controller.h"

#define NUNCHUK_ADDRESS 0x52

void Controller::begin() {
    Nunchuk.begin(NUNCHUK_ADDRESS);
}

void Controller::registerCallback(ControllerAction action, ControllerCallback callback) {
    callbacks[action] = callback;
}

void Controller::triggerCallback(ControllerAction action) {
    if (callbacks[action] == nullptr) {
        return;
    }

    callbacks[action]();
}

void Controller::clearCallbacks() {
    memset(callbacks, 0, sizeof callbacks);
}

/**
 * Get the direction of the controller.
 * @return The direction of the controller.
 */
void Controller::tick() {
    if (!Nunchuk.getState(NUNCHUK_ADDRESS)) {
        waitForNewInput = true ;
    }

    if (!waitForNewInput && Nunchuk.state.joy_x_axis < 168 && Nunchuk.state.joy_x_axis > 40 && Nunchuk.state.joy_y_axis < 168 && Nunchuk.state.joy_y_axis > 40) {
        waitForNewInput = true;
    }

    if (waitForNewInput && Nunchuk.state.joy_x_axis < 40) {
        waitForNewInput = false;
        triggerCallback(LEFT);
    }

    if (waitForNewInput && Nunchuk.state.joy_x_axis > 168) {
        waitForNewInput = false;
        triggerCallback(RIGHT);
    }

    if (waitForNewInput && Nunchuk.state.joy_y_axis > 168) {
        waitForNewInput = false;
        triggerCallback(UP);
    }

    if (waitForNewInput && Nunchuk.state.joy_y_axis < 40) {
        waitForNewInput = false;
        triggerCallback(DOWN);
    }

    if (waitForNewInput && Nunchuk.state.z_button) {
        waitForNewInput = false;
        triggerCallback(PRIMARY);
    }

    if (waitForNewInput && Nunchuk.state.c_button) {
        waitForNewInput = false;
        triggerCallback(SECONDARY);
    }
}

class Controller Controller;