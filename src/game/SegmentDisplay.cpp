#include "SegmentDisplay.h"

void SegmentDisplay::setValue(uint8_t value) {
    Wire.beginTransmission(0x21);
    Wire.write(segmentGetal[value]);
    Wire.endTransmission();
}

class SegmentDisplay SegmentDisplay;