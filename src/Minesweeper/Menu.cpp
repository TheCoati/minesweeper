#include "Menu.h"

void openMainMenu() {
    drawMenu();
    // TODO - inplementatie & drawMenu aanpassen
}

void openLobby() {

}

void joinLobby() {
    
}

void handeInput() {

    ControllerDirection direction = getJoystick();
    
    switch(direction)
    {
        case UP:
            // TODO - knop selecteren
            break;
        case DOWN:
            // TODO - knop selecteren
            break;
        default:
            break;
    }
}