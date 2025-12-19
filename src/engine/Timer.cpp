#include "Timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>

static volatile uint32_t s_millis = 0;
static constexpr uint8_t WDT_INTERVAL_MS = 16;

void Timer::begin() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        wdt_reset(); // reset WDT
        // Enable change of WDTCSR
        WDTCSR |= (1 << WDCE) | (1 << WDE);
        // Set WDT to interrupt mode only, prescaler = 16ms (WDP bits = 0)
        // WDIE = 1, WDE = 0 -> interrupt on timeout only
        WDTCSR = (1 << WDIE);
    }
    sei(); // ensure interrupts enabled
}

void Timer::stop() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        WDTCSR |= (1 << WDCE) | (1 << WDE);
        WDTCSR = 0x00;
    }
}

uint32_t Timer::millis() {
    uint32_t m;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        m = s_millis;
    }
    return m;
}

ISR(WDT_vect) {
    s_millis += WDT_INTERVAL_MS;
}

class Timer Timer;