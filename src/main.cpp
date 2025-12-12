#include <Minenet.h>
#include "Minesweeper/Controller.h"
#include "Minesweeper/Game.h"

uint8_t globalSeed = 0;

int main() {
    // Libraries
    Wire.begin();
    Minenet.begin();

    // Minesweeper
    init();
    initScreen();
    initController();
    segmentGetal(2);
    drawMenu();

    while (true) {
        globalSeed++;

        // Todo
        if (isPrimaryPressed()) {
            startGame(globalSeed);
        }
    }
}