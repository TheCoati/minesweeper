#ifndef MINESWEEPER_MP_MENU_SCENE_H
#define MINESWEEPER_MP_MENU_SCENE_H

#include "Engine/Scene.h"
#include "Engine/SceneManager.h"
#include "Game/Minesweeper.h"
#include "Peripherals/Controller.h"
#include "Peripherals/Screen.h"

class MPMenuScene : public Scene {
protected:
    void onBegin() override;
    void onTick() override;
    void onDestroy() override;
private:
    static void onPrimaryPress();
};

#endif //MINESWEEPER_MP_MENU_SCENE_H
