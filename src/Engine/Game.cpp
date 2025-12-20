#include "Game.h"

#include <Minenet.h>
#include "Timer.h"
#include "Peripherals/Controller.h"

void Game::begin() {
    Timer::begin();
    Minenet.begin();
    Controller.begin();

    onBegin();
}

void Game::tick() {
    SceneManager.tick();
    Controller.tick();

    onTick();
}
