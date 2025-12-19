#include "MainMenuScene.h"

#include <config.h>

#include "GameScene.h"
#include "MPMenuScene.h"

#define MAIN_MENU_BUTTON_X ((SCREEN_WIDTH - BUTTON_WIDTH) / 2)
#define MAIN_MENU_BUTTON_Y(index) (MAIN_MENU_SCREEN_PADDING + index * (BUTTON_HEIGHT + MAIN_MENU_BUTTON_SPACE))

void MainMenuScene::onBegin() {
    tft.fillScreen(MENU_BACKGROUND_COLOR);

    drawButton(MenuButton::SINGLEPLAYER);
    drawButton(MenuButton::MULTIPLAYER);
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
    //
}

/*
 |--------------------------------------------------------------------------
 | Rendering
 |--------------------------------------------------------------------------
 */

const char* MainMenuScene::getButtonImage(uint8_t index) {
    switch (index) {
        case 0:
            return "/btn_sp.bmp";
        case 1:
            return "/btn_mp.bmp";
        default:
            return "";
    }
}

void MainMenuScene::drawButton(uint8_t index) {
    uint8_t y = MAIN_MENU_BUTTON_Y(index);

    if (Screen.hasSDCard()) {
        const char* imagePath = getButtonImage(index);

        Screen.drawButton(imagePath, MAIN_MENU_BUTTON_X, y);
    }
    #ifdef ALLOW_NO_SD_CARD
    else {
        // TODO
    }
    #endif
}

void MainMenuScene::drawCursor(uint8_t index) {
    uint8_t y = MAIN_MENU_BUTTON_Y(index);

    Screen::drawButtonCursor(MAIN_MENU_BUTTON_X, y);
}

/*
 |--------------------------------------------------------------------------
 | Input Handling
 |--------------------------------------------------------------------------
 */

void MainMenuScene::moveUp() {
    selected = MenuButton::SINGLEPLAYER;

    drawButton(MenuButton::MULTIPLAYER); //  Redraw
    drawCursor(selected = MenuButton::SINGLEPLAYER);
}

void MainMenuScene::moveDown() {
    drawButton(MenuButton::SINGLEPLAYER); //  Redraw
    drawCursor(selected = MenuButton::MULTIPLAYER);
}

void MainMenuScene::onPrimaryPress() const {
    switch (selected) {
        case SINGLEPLAYER:
            SceneManager.unloadScene();
            SceneManager.switchScene(new GameScene(Minesweeper::random));
            break;
        case MULTIPLAYER:
            SceneManager.unloadScene();
            SceneManager.switchScene(new MPMenuScene());
            break;
        default:
            break;
    }
}
