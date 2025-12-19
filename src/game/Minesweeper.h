#ifndef MINESWEEPER_MINESWEEPER_H
#define MINESWEEPER_MINESWEEPER_H

#include "engine/Game.h"
#include "game/SegmentDisplay.h"

class Minesweeper : public Game {
public:
    Minesweeper() = default;
    ~Minesweeper() override = default;

protected:
    void onBegin() override;
    void onTick() override;
};


#endif //MINESWEEPER_MINESWEEPER_H
