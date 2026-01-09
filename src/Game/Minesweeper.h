#ifndef MINESWEEPER_MINESWEEPER_H
#define MINESWEEPER_MINESWEEPER_H

#include "Engine/Game.h"
#include "Peripherals/SegmentDisplay.h"

class Minesweeper : public Game {
public:
    Minesweeper() = default;
    ~Minesweeper() override = default;
    static uint8_t random;
protected:
    void onBegin() override;
    void onTick() override;

};

#endif //MINESWEEPER_MINESWEEPER_H
