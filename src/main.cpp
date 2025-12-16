#include "game/Minesweeper.h"
#include <avr/interrupt.h>
#include <Wire.h>

class Minesweeper Game;

int main() {
    sei();

    Wire.begin();
    Game.begin();

    while (true) {
        Game.tick();
    }
}