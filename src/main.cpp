#include <Minenet.h>
#include "Minesweeper/Game.h"
#include "Minesweeper/Controller.h"

uint8_t globalSeed = 0;

int main() {
    init();

    // Libraries
    Wire.begin();
    Minenet.begin();

    // Minesweeper
    initScreen();
    initController();

    drawMenu();

    while (true) {
        globalSeed++;

        // Todo
        if (isPrimaryPressed()) {
            startGame(globalSeed);
        }
    }
}