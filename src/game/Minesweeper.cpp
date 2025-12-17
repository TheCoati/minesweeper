#include "Minesweeper.h"
#include "game/scenes/MainMenuScene.h"

void Minesweeper::onBegin() {
    SceneManager.switchScene(new MainMenuScene());
}

void Minesweeper::onTick() {

}



