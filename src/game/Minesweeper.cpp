#include "Minesweeper.h"
#include "game/scenes/MainMenuScene.h"

//uint8_t Minesweeper::random = 0;

void Minesweeper::onBegin() {
    Screen.begin();
    SceneManager.switchScene(new MainMenuScene());
}

void Minesweeper::onTick() {
//    Minesweeper::random++;
}


