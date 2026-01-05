#include "MPMenuScene.h"

#include <config.h>
#include <Minenet.h>
#include "Game/Scenes/GameScene.h"
#include "Game/Scenes/MainMenuScene.h"

#define INFO_BMP_X 17 // 17px
#define CANCEL_BUTTON_X ((SCREEN_WIDTH - BUTTON_WIDTH) / 2)
#define CANCEL_BUTTON_Y (SCREEN_HEIGHT - MP_MENU_SCREEN_PADDING - BUTTON_HEIGHT)

void MPMenuScene::onBegin() {
    tft.fillScreen(MENU_BACKGROUND_COLOR);

    if (Screen.hasSDCard()) {
        Screen.getReader().drawBMP("/mp_info.bmp", tft, INFO_BMP_X, MP_MENU_SCREEN_PADDING);
        Screen.drawButton("/btn_cl.bmp", CANCEL_BUTTON_X, CANCEL_BUTTON_Y);
    }
    #ifdef ALLOW_NO_SD_CARD
    else {
        // TODO
    }
    #endif

    Screen::drawButtonCursor(CANCEL_BUTTON_X, CANCEL_BUTTON_Y);
}

void connect() {
    Minenet.send(0x00, 0x00, 0x01, 0x00);
}

void MPMenuScene::onTick() {
    if (Controller.available()) {
        ControllerAction action = Controller.read();

        switch (action) {
            case PRIMARY:
                onPrimaryPress();
                break;
            case SECONDARY:
                connect();
            default:
                break;
        }
    }

    if (Minenet.available()) {
        MinenetPacket packet = Minenet.read();

        if (packet.opCode == 0x01) {
            SceneManager.switchScene(new GameScene(Minesweeper::random, true, false));
        }

        if (packet.opCode == 0x02) {
            SceneManager.switchScene(new GameScene(packet.payload, true, true));
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


