#ifndef MINESWEEPER_SEGMENTDISPLAY_H
#define MINESWEEPER_SEGMENTDISPLAY_H

#include "Wire.h"

#define DISPLAY_OFF 10

class SegmentDisplay {
public:
    void setValue(uint8_t value);
private:
    uint8_t segmentValues[11] = {
        0b11000000,  //  0
        0b11111001,  //  1
        0b10100100,  //  2
        0b10110000,  //  3
        0b10011001,  //  4
        0b10010010,  //  5
        0b10000010,  //  6
        0b11111000,  //  7
        0b10000000,  //  8
        0b10010000,  //  9
        0b11111111, //  OFF
    };
};

extern SegmentDisplay SegmentDisplay;

#endif  //MINESWEEPER_SEGMENTDISPLAY_H
