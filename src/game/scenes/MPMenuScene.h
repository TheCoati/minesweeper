#ifndef MINESWEEPER_MP_MENU_SCENE_H
#define MINESWEEPER_MP_MENU_SCENE_H

#include "../../engine/Scene.h"
#include "../Screen.h"

class MPMenuScene : public Scene {
protected:
    void onBegin() override;
    void onTick() override;
    void onDestroy() override;
};

#endif //MINESWEEPER_MP_MENU_SCENE_H
