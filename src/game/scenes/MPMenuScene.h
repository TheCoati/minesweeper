#ifndef MINESWEEPER_MP_MENU_SCENE_H
#define MINESWEEPER_MP_MENU_SCENE_H

#include "engine/Controller.h"
#include "engine/Scene.h"
#include "engine/SceneManager.h"

#include "game/Minesweeper.h"
#include "game/Screen.h"

class MPMenuScene : public Scene {
protected:
    void onBegin() override;
    void onTick() override;
    void onDestroy() override;
private:
    static void onPrimaryPress();
};

#endif //MINESWEEPER_MP_MENU_SCENE_H
