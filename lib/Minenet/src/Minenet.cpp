#include "Minenet.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

#define VERSION 1

#define US(x) ((x) * 2)  // Convert microseconds to timer ticks on timer 1

#define HDR_MARK_TIME US(9000)   // Duration of header mark
#define HDR_SPACE_TIME US(4500)  // Duration of header space
#define BIT_MARK_TIME US(560)    // Duration of bit mark
#define ONE_SPACE_TIME US(1690)  // Duration of one space
#define ZERO_SPACE_TIME US(560)  // Duration of zero space

#define RX_TOLERANCE US(200)
#define RX_TOL_MIN(x) ((x) - RX_TOLERANCE)
#define RX_TOL_MAX(x) ((x) + RX_TOLERANCE)

/**
 * Attach the carrier signal to the timer output pin
 * When attached will output the pulses at the speed of timer 2
 */
inline void carrier_on() {
    TCCR0A |= (1 << COM0A0);  // Connect to PWM
}

/**
 * Detach the carrier signal from the timer output pin
 * Will stop the carrier signal
 */
inline void carrier_off() {
    TCCR0A &= ~(1 << COM0A0);  // Disconnect from PWM
    PORTD &= ~(1 << PD6);      // Make sure the pin is low
}

MinenetProtocol::MinenetProtocol() : txActive(false), txState(TX_BURST), txSeq(0), txBits(0), txShift(0), rxState(RX_BURST), rxBits(0), rxShift(0), rxAvailable(false) {
    //
}

/**
 * Start the IR protocol with the given session number
 * @param session_id The initial session number
 */
void MinenetProtocol::begin() {
    sei();

    // Set RX
    DDRD &= ~(1 << DDD2);      // Pin 2 (Input)
    PCICR |= (1 << PCIE2);     // Interrupts on register D
    PCMSK2 |= (1 << PCINT18);  // Interrupt on pin 2

    // Timer 1
    // Frequency = 2 MHz = 0.5us ticks
    TCCR1A = 0;            // Normal Mode
    TCCR1B = (1 << CS11);  // Prescaler 8

    // Set TX
    DDRD |= (1 << DDD6);  // Pin 6 (Output)

    // Timer 0 (PWM)
    TCCR0A = (1 << WGM01);     // CTC Mode
    TCCR0B = (1 << CS01);      // Prescaler 8
    OCR0A = 25;                // Compare value (16,000,000 / (2 * 8 * (1 + 25)) = 38,461 Hz)
    TCCR0A &= ~(1 << COM0A0);  // Disconnect from PWM
}

/**
 * Send a packet over the IR protocol
 * @param op OP code of the packet
 * @param payload Payload of the packet
 * @return boolean if the packet can be sent
 */
bool MinenetProtocol::send(uint8_t clientId, uint8_t sessionId, uint8_t opCode, uint8_t payload) {
    if (txActive) {
        return false;
    }

    // | Version | Sequence | Client ID | Session ID | Op Code | Payload | CRC    |
    // | 4 bits  | 1 bit    | 3 bits    | 4 bits     | 4 bits  | 8 bits  | 8 bits |

    uint8_t byte1 = ((VERSION & 0x0F) << 4) | ((txSeq & 0x01) << 3) | (clientId & 0x07);
    uint8_t byte2 = ((sessionId & 0x0F) << 4) | (opCode & 0x0F);
    uint8_t crc = (byte1 + byte2 + payload) & 0xFF;

    uint32_t packet = ((uint32_t)byte1 << 24) | ((uint32_t)byte2 << 16) | ((uint32_t)payload << 8) | crc;

    txBits = 32;
    txShift = packet;
    txState = TX_BURST;
    txActive = true;

    PCMSK2 &= ~(1 << PCINT18);  // Disable RX interrupt while transmitting (half-duplex)

    TCNT1 = 0;                // Reset the timer to 0
    OCR1A = HDR_MARK_TIME;    // First interrupt when the duration of HDR_MARK_TIME is reached
    TIFR1 |= (1 << OCF1A);    // Clear any pending Timer1 Output Compare A flag
    TIMSK1 |= (1 << OCIE1A);  // Enable TX interrupt

    carrier_on();
    return true;
}

/**
 * Decode the packet received
 */
MinenetPacket MinenetProtocol::read() {
    // | Version | Sequence | Client ID | Session ID | Op Code | Payload | CRC    |
    // | 4 bits  | 1 bit    | 3 bits    | 4 bits     | 4 bits  | 8 bits  | 8 bits |

    MinenetPacket packet{};
    uint32_t data;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        data = rxShift;  // Block interrupts while reading to avoid race conditions
    }

    uint8_t byte1 = (data >> 24) & 0xFF;
    packet.version = (byte1 >> 4) & 0x0F;
    packet.seq = (byte1 >> 3) & 0x01;
    packet.clientId = byte1 & 0x07;

    uint8_t byte2 = (data >> 16) & 0xFF;
    packet.sessionId = (byte2 >> 4) & 0x0F;
    packet.opCode = byte2 & 0x0F;

    packet.payload = (data >> 8) & 0xFF;
    packet.crc = data & 0xFF;

    auto calc = (uint8_t)((byte1 + byte2 + packet.payload) & 0xFF);

    packet.valid = (calc == packet.crc);

    return packet;
}

bool MinenetProtocol::available() {
    if (rxAvailable) {
        rxAvailable = false;
        return true;
    }

    return false;
}

/**
 * Handle the timer interrupt for transmitting
 * Triggered every time when shifting to the next state
 */
void MinenetProtocol::handleTxTimer() {
    TCNT1 = 0;  // Reset the timer to 0

    switch (txState) {
        case TX_BURST:
            carrier_off();
            OCR1A = HDR_SPACE_TIME;

            txState = TX_BURST_SPACE;
            break;
        case TX_BURST_SPACE:
            carrier_on();
            OCR1A = BIT_MARK_TIME;

            txState = TX_DATA;
            break;
        case TX_DATA:
            carrier_off();
            OCR1A = txShift & 0x80000000 ? ONE_SPACE_TIME : ZERO_SPACE_TIME;

            txShift <<= 1;
            txState = TX_BIT_MARK;
            break;
        case TX_BIT_MARK:
            carrier_on();
            OCR1A = BIT_MARK_TIME;

            txBits--;
            txState = txBits > 0 ? TX_DATA : TX_COMPLETE;
            break;
        case TX_COMPLETE:
        default:
            carrier_off();
            TIMSK1 &= ~(1 << OCIE1A);  // Disable TX interrupt
            PCIFR |= (1 << PCIF2);     // Clear interrupt flag before enabling RX again
            PCMSK2 |= (1 << PCINT18);  // Re-enable RX interrupt

            txActive = false;
            break;
    }
}

/**
 * Handle the interrupt on pin 2 (RX)
 * Triggered when data is received on the IR receiver
 */
void MinenetProtocol::handleRxInterrupt() {
    if (txActive) {
        return;
    }

    uint16_t width = TCNT1;

    TCNT1 = 0;  // Reset the timer to 0

    bool inputHigh = PIND & (1 << PIND2);

    switch (rxState) {
        case RX_BURST:
            // Check if the header burst matches the expected time with tolerance
            if (inputHigh && width > RX_TOL_MIN(HDR_MARK_TIME) && width < RX_TOL_MAX(HDR_MARK_TIME)) {
                rxState = RX_BURST_SPACE;
            }
            break;
        case RX_BURST_SPACE:
            // Check if the header space matches the expected time with tolerance
            if (!inputHigh && width > RX_TOL_MIN(HDR_SPACE_TIME) && width < RX_TOL_MAX(HDR_SPACE_TIME)) {
                rxBits = 0;
                rxShift = 0;
                rxState = RX_DATA;
            } else {
                rxState = RX_BURST;
            }
            break;
        case RX_DATA:
            if (inputHigh) {
                break;  // Todo: Validate if input here is not noise (BIT_MARK_TIME)
            }

            rxShift <<= 1;

            // Check if the bit matches the expected time with tolerance
            if (width > RX_TOL_MIN(ONE_SPACE_TIME) && width < RX_TOL_MAX(ONE_SPACE_TIME)) {
                rxShift |= 1;  // 1
            } else if (width > RX_TOL_MIN(ZERO_SPACE_TIME) && width < RX_TOL_MAX(ZERO_SPACE_TIME)) {
                // 0
            } else {
                rxState = RX_BURST;
                break;
            }

            rxBits++;

            if (rxBits == 32) {
                rxBits = 0;
                rxAvailable = true;
                rxState = RX_BURST;
            }

            break;
        default:
            rxState = RX_BURST_SPACE;
    }
}

MinenetProtocol Minenet;

ISR(TIMER1_COMPA_vect) {
    Minenet.handleTxTimer();
}

ISR(PCINT2_vect) {
    Minenet.handleRxInterrupt();
}