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
    void onPrimaryPress();
    static void onSecondaryPress();
};

#endif //MINESWEEPER_MAIN_MENU_SCENE_H
