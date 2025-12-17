#include "GameScene.h"
#include "engine/Controller.h"
#include "engine/SceneManager.h"

#include "MainMenuScene.h"

void GameScene::onBegin() {
    tft.setCursor(0, 0);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);

    tft.println("Game");
    tft.println("[C] - Exit");
}

void GameScene::onTick() {
    if (Controller.available()) {
        ControllerAction action = Controller.read();

        switch (action) {
            case SECONDARY:
                this->onSecondaryPress();
                break;
            default:
                break;
        }
    }
}

void GameScene::onDestroy() {

}

void GameScene::onSecondaryPress() {
    SceneManager.switchScene(new MainMenuScene());
}
