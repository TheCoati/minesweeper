#include <Wire.h>
#include "Arduino.h"
#include "game/Minesweeper.h"

class Minesweeper Game;

int main() {
    init();

    Wire.begin();
    Game.begin();

    while (true) {
        Game.tick();
    }
}