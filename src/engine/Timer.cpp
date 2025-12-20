#include "Timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>

#define WDT_INTERVAL_MS 16

volatile uint32_t Timer::milliseconds = 0;

void Timer::begin() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        wdt_reset(); //  Reset WDT to prevent immediate reset
        WDTCSR |= (1 << WDCE) | (1 << WDE); //  Enable configuration changes
        WDTCSR = (1 << WDIE); //  Set WDT to interrupt mode with ~16ms interval
    }

    sei();
}

uint32_t Timer::millis() {
    uint32_t cached;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        cached = milliseconds;
    }

    return cached;
}

ISR(WDT_vect) {
    Timer::milliseconds += WDT_INTERVAL_MS;
}

class Timer Timer;