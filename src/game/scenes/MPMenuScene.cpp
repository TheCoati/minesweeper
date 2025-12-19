#include "MPMenuScene.h"

#include "engine/Controller.h"
#include "engine/SceneManager.h"
#include "MainMenuScene.h"
#include "MPHostMenuScene.h"
#include "GameScene.h"
#include <Minenet.h>
#include "game/Minesweeper.h"

void MPMenuScene::onBegin() {
    tft.setCursor(0, 0);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);

    tft.println("Multiplayer Menu");
    tft.println("[Z] - Host game");
    tft.println("[C] - Exit");
}

void MPMenuScene::onTick() {
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

    if (Minenet.available()) {
        MinenetPacket packet = Minenet.read();

        if (packet.opCode == 0x01) {
            SceneManager.switchScene(new GameScene(Minesweeper::random));
        }
    }
}

void MPMenuScene::onDestroy() {

}

void MPMenuScene::onPrimaryPress() {
    SceneManager.switchScene(new MPHostMenuScene());
}

void MPMenuScene::onSecondaryPress() {
    SceneManager.switchScene(new MainMenuScene());
}
