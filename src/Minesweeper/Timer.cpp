#include "Utils.h"

uint8_t globalSeed = 0;

volatile uint32_t milliseconds = 0;

void initUtils() {
    cli();
    WDTCSR |= (1 << WDCE) | (1 << WDE);  // Enable change of WDT settings (4 clock cycles of protection)
    WDTCSR = (1 << WDIE) | (1 << WDP0);  // WDIE: Interrupt Enable, WDP0: ~16ms timeout
    sei();
}

uint8_t getSeed() {
    return globalSeed;
}

uint8_t incrementSeed() {
    globalSeed++;

    return globalSeed;
}

uint32_t millis() {
    uint32_t millisSnapshot;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        millisSnapshot = milliseconds;
    }

    return millisSnapshot;
}

ISR(WDT_vect) {
    milliseconds += 16;
}

