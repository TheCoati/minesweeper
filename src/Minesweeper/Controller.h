#ifndef MINESWEEPER_CONTROLLER_H
#define MINESWEEPER_CONTROLLER_H

#include <Nunchuk.h>

enum ControllerDirection {
    IDLE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

void initController();
ControllerDirection getJoystick();
bool isPrimaryPressed();
bool isSecondaryPressed();

#endif  //MINESWEEPER_CONTROLLER_H
