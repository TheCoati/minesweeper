#include "Game.h"
#include "Controller.h"
#include "Timer.h"
#include <Minenet.h>

void Game::begin() {
    Timer.begin();
    Minenet.begin();
    Controller.begin();

    this->onBegin();
}

void Game::tick() {
    SceneManager.tick();
    Controller.tick();

    this->onTick();
}
