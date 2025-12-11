#include "Minesweeper/Game.h"
#include "Minesweeper/Screen.h"
#include "Minesweeper/Utils.h"
#include <Minenet.h>

int main() {
    Wire.begin();
    Minenet.begin();

    init();
    initScreen();

    // Todo: Start from main menu not on device boot
    startGame();

    while (true) {
        seed++;
        onTick();
    }
}