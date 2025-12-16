#include <Wire.h>
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
    updateDisplay(10); //10 is uit.
    drawMenu();

    while (true) {
        globalSeed++;

        // Todo
        if (isPrimaryPressed()) {
            startGame(globalSeed);
        }
    }
}