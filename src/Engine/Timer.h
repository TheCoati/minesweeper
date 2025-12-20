#ifndef MINESWEEPER_TIMER_H
#define MINESWEEPER_TIMER_H

#include <stdint.h>

class Timer {
public:
    static volatile uint32_t milliseconds;

    static void begin();
    static uint32_t millis();
};

extern Timer Timer;

#endif  //MINESWEEPER_TIMER_H
