#ifndef MINESWEEPER_MAIN_MENU_SCENE_H
#define MINESWEEPER_MAIN_MENU_SCENE_H

#include "engine/Controller.h"
#include "engine/Scene.h"
#include "engine/SceneManager.h"

#include "game/Minesweeper.h"
#include "game/Screen.h"

class MainMenuScene : public Scene {
protected:
    void onBegin() override;
    void onTick() override;
    void onDestroy() override;
private:
    uint8_t selected = 0;

    static const char* getButtonImage(uint8_t index);
    static void drawButton(uint8_t index);
    static void drawCursor(uint8_t index);

    void onPrimaryPress() const;
    void moveUp();
    void moveDown();
};

#endif //MINESWEEPER_MAIN_MENU_SCENE_H
