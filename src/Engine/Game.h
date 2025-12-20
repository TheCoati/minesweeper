#ifndef MINESWEEPER_GAME_H
#define MINESWEEPER_GAME_H

#include "Engine/SceneManager.h"

class Game {
public:
    Game() = default;
    virtual ~Game() = default;

    void begin();
    void tick();

protected:
    virtual void onBegin() = 0;
    virtual void onTick() = 0;
};


#endif //MINESWEEPER_GAME_H
