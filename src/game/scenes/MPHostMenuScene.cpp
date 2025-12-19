#include "MPHostMenuScene.h"

#include "engine/Controller.h"
#include "engine/SceneManager.h"
#include "MainMenuScene.h"
#include <Minenet.h>

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

void MPHostMenuScene::onSecondaryPress() {
    SceneManager.switchScene(new MainMenuScene());
}
