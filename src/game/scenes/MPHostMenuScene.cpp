#include "MPHostMenuScene.h"

#include <Minenet.h>

#include "MainMenuScene.h"

void MPHostMenuScene::onBegin() {
    tft.setCursor(0, 0);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);

    tft.println("Zoeken naar tegenspeler...");
    tft.println("[C] - Exit");
}

void MPHostMenuScene::onTick() {
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

    Minenet.send(0, 0, 0x01, 0);
}

void MPHostMenuScene::onDestroy() {

}

/*
 |--------------------------------------------------------------------------
 | Input Handling
 |--------------------------------------------------------------------------
 */

void MPHostMenuScene::onSecondaryPress() {
    SceneManager.unloadScene();
    SceneManager.switchScene(new MainMenuScene());
}
