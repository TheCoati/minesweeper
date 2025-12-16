#include <Wire.h>
#include <Minenet.h>
#include "Minesweeper/Controller.h"
#include "Minesweeper/Game.h"

uint8_t globalSeed = 0;

int main() {
    // Libraries
    Wire.begin();

    // Minesweeper
    init();
    initScreen();
    initController();
    updateDisplay(10); //10 is uit.
    drawMenu();

    Minenet.begin();

    while (true) {
        globalSeed++;

        // Todo
        if (isPrimaryPressed()) {
            startGame(globalSeed);
        }
    }
}