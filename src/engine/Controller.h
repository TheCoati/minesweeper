//
// Created by Kevin on 16 Dec 2025.
//

#include <string.h>
#include <Nunchuk.h>

#ifndef MINESWEEPER_CONTROLLER_H
#define MINESWEEPER_CONTROLLER_H

typedef void (*ControllerCallback)();

enum ControllerAction {
    IDLE = 0,
    LEFT = 1,
    RIGHT = 2,
    UP = 3,
    DOWN = 4,
    PRIMARY = 5,
    SECONDARY = 6,
};

class Controller {
public:
    void begin();
    void tick();
    void registerCallback(ControllerAction action, ControllerCallback callback);
    void clearCallbacks();
private:
    void triggerCallback(ControllerAction action);
    bool waitForNewInput = true;
    ControllerCallback callbacks[6];
};

extern Controller Controller;

#endif //MINESWEEPER_CONTROLLER_H
