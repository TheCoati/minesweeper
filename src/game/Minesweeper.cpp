#include "Minesweeper.h"
#include "game/scenes/MainMenuScene.h"

void Minesweeper::onBegin() {
    Screen.begin();

    SceneManager.switchScene(new MainMenuScene());
}

void Minesweeper::onTick() {

}



