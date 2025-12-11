#ifndef MINESWEEPER_CONTROLLER_H
#define MINESWEEPER_CONTROLLER_H

enum ControllerDirection {
    IDLE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

ControllerDirection getJoystick();
bool isPrimaryPressed();
bool isSecondaryPressed();

#endif  //MINESWEEPER_CONTROLLER_H
