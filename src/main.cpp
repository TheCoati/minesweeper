#include <Arduino.h>
#include <Wire.h>
#include "Game/Minesweeper.h"

class Minesweeper Game;

int main() {
    init();

    Wire.begin();
    Game.begin();

    while (true) {
        Game.tick();
    }
}