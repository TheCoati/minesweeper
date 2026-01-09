#ifndef MINESWEEPER_MAIN_MENU_SCENE_H
#define MINESWEEPER_MAIN_MENU_SCENE_H

#include "Engine/Scene.h"
#include "Engine/SceneManager.h"
#include "Game/Minesweeper.h"
#include "Peripherals/Controller.h"
#include "Peripherals/Screen.h"

class MainMenuScene : public Scene {
protected:
    void onBegin() override;
    void onTick() override;
    void onDestroy() override;
private:
    enum MenuButton {
        SINGLEPLAYER = 0,
        MULTIPLAYER = 1,
    };

    MenuButton selected = MenuButton::SINGLEPLAYER;

    static const char* getButtonImage(uint8_t index);
    static void drawButton(uint8_t index);
    static void drawCursor(uint8_t index);

    void onPrimaryPress() const;
    void moveUp();
    void moveDown();
};

#endif //MINESWEEPER_MAIN_MENU_SCENE_H
