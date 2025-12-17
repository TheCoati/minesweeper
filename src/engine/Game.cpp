#include "Game.h"
#include "Networking.h"
#include "Controller.h"

void Game::begin() {

    Controller.begin();
//    Networking.begin();

    this->onBegin();
}

void Game::tick() {
    SceneManager.tick();
    Controller.tick();
    Networking.tick();

    this->onTick();
}
