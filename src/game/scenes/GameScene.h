#ifndef MINESWEEPER_GAME_SCENE_H
#define MINESWEEPER_GAME_SCENE_H

#include "engine/Networking.h"
#include "engine/Scene.h"
#include "../Screen.h"

class GameScene : public Scene {
protected:
    void onBegin() override;
    void onTick() override;
    void onDestroy() override;
private:
    void onSecondaryPress();
};

#endif //MINESWEEPER_GAME_SCENE_H
