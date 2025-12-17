//
// Created by Kevin on 16 Dec 2025.
//

#include <string.h>
#include <Nunchuk.h>

#ifndef MINESWEEPER_CONTROLLER_H
#define MINESWEEPER_CONTROLLER_H

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
    bool available();
    ControllerAction read();
private:
    bool newInput = false;
    bool waitForNewInput = true;
    ControllerAction lastAction;
};

extern Controller Controller;

#endif //MINESWEEPER_CONTROLLER_H
