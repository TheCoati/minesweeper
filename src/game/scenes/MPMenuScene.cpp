#include "MPMenuScene.h"
#include "engine/Controller.h"
#include "engine/SceneManager.h"

#include "MainMenuScene.h"

void MPMenuScene::onBegin() {
    tft.setCursor(0, 0);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);

    tft.println("Multiplayer Menu");
    tft.println("[C] - Exit");
}

void MPMenuScene::onTick() {
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

void MPMenuScene::onDestroy() {

}

void MPMenuScene::onSecondaryPress() {
    SceneManager.switchScene(new MainMenuScene());
}
