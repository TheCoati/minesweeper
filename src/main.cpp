#include "Minesweeper/Game.h"
#include "Minesweeper/Screen.h"

int main() {
    Wire.begin();

    init();
    initScreen();

    // Todo: Start from main menu not on device boot
    startGame();
}