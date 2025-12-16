#ifndef MINESWEEPER_MAIN_MENU_SCENE_H
#define MINESWEEPER_MAIN_MENU_SCENE_H

#include "../../engine/Scene.h"
#include "../Screen.h"

class MainMenuScene : public Scene {
protected:
    void onBegin() override;
    void onTick() override;
    void onDestroy() override;
private:
    static void onPrimaryPress();
    static void onSecondaryPress();
};

#endif //MINESWEEPER_MAIN_MENU_SCENE_H
