#ifndef MINESWEEPER_TIMER_H
#define MINESWEEPER_TIMER_H

#include <stdint.h>

class Timer {
public:
    void begin();
    void stop();
    uint32_t millis();
};

extern Timer Timer;

#endif  //MINESWEEPER_TIMER_H
