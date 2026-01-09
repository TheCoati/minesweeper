#include "SegmentDisplay.h"

#define DISPLAY_I2C_ADDRESS 0x21

void SegmentDisplay::setValue(uint8_t value) {
    Wire.beginTransmission(DISPLAY_I2C_ADDRESS);
    Wire.write(segmentValues[value]);
    Wire.endTransmission();
}

class SegmentDisplay SegmentDisplay;