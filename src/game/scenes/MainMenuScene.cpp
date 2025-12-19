#include "MainMenuScene.h"

#include "GameScene.h"
#include "MPMenuScene.h"

void MainMenuScene::onBegin() {
    tft.setCursor(0, 0);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);

    tft.println("Main Menu");
    tft.println("[Z] - Singleplayer");
    tft.println("[C] - Multiplayer");
}

void MainMenuScene::onTick() {
    if (Controller.available()) {
        ControllerAction action = Controller.read();

        switch (action) {
            case PRIMARY:
                this->onPrimaryPress();
                break;
            case SECONDARY:
                this->onSecondaryPress();
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
 | Input Handling
 |--------------------------------------------------------------------------
 */

void MainMenuScene::onPrimaryPress() {
    SceneManager.switchScene(new GameScene(Minesweeper::random));
}

void MainMenuScene::onSecondaryPress() {
    SceneManager.switchScene(new MPMenuScene());
}
