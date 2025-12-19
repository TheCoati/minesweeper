#include "MPMenuScene.h"

#include <config.h>
#include <Minenet.h>

#include "MainMenuScene.h"
#include "GameScene.h"

void MPMenuScene::onBegin() {
    tft.fillScreen(MENU_BACKGROUND_COLOR);

    Screen.getReader().drawBMP("/mp_info.bmp", tft, 17, 40);
    Screen.drawButton("btn_cl.bmp", 64, 168);
    Screen::drawButtonCursor(64, 168);
}

void MPMenuScene::onTick() {
    if (Controller.available()) {
        ControllerAction action = Controller.read();

        switch (action) {
            case PRIMARY:
                onPrimaryPress();
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

/*
 |--------------------------------------------------------------------------
 | Input Handling
 |--------------------------------------------------------------------------
 */

void MPMenuScene::onPrimaryPress() {
    SceneManager.unloadScene();
    SceneManager.switchScene(new MainMenuScene());
}
