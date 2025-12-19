#include "MainMenuScene.h"

#include <config.h>

#include "GameScene.h"
#include "MPMenuScene.h"

void MainMenuScene::onBegin() {
    tft.fillScreen(MENU_BACKGROUND_COLOR);

    drawButton(0);
    drawButton(1);
    drawCursor(selected);
}

void MainMenuScene::onTick() {
    if (Controller.available()) {
        ControllerAction action = Controller.read();

        switch (action) {
            case UP:
                moveUp();
                break;
            case DOWN:
                moveDown();
                break;
            case PRIMARY:
                onPrimaryPress();
                break;
            default:
                break;
        }
    }
}

void MainMenuScene::onDestroy() {

}

/*
 |--------------------------------------------------------------------------
 | Rendering
 |--------------------------------------------------------------------------
 */

const char* MainMenuScene::getButtonImage(uint8_t index) {
    switch (index) {
        case 0:
            return "btn_sp.bmp";
        case 1:
            return "btn_mp.bmp";
        default:
            return "";
    }
}

void MainMenuScene::drawButton(uint8_t index) {
    uint8_t y = 60 + index * (48 + 24);

    Screen.drawButton(getButtonImage(index), 64, y);
}

void MainMenuScene::drawCursor(uint8_t index) {
    uint8_t x = 64;
    uint8_t y = 60 + index * (48 + 24);

    Screen::drawButtonCursor(x, y);
}

/*
 |--------------------------------------------------------------------------
 | Input Handling
 |--------------------------------------------------------------------------
 */

void MainMenuScene::moveUp() {
    selected = 0;
    drawButton(1);
    drawCursor(selected = 0);
}

void MainMenuScene::moveDown() {
    drawButton(0);
    drawCursor(selected = 1);
}

void MainMenuScene::onPrimaryPress() const {
    switch (selected) {
        case 0:
            SceneManager.unloadScene();
            SceneManager.switchScene(new GameScene(Minesweeper::random));
            break;
        case 1:
            SceneManager.unloadScene();
            SceneManager.switchScene(new MPMenuScene());
            break;
        default:
            break;
    }
}
