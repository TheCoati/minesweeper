//#include "MinesweeperIR.h"
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include "Arduino.h"
//
//#define US(x) ((x) * 2) // Timer 1 = 0.5us per tick
//
//// Timings
//#define HDR_MARK    US(9000)
//#define HDR_SPACE   US(4500)
//#define BIT_MARK    US(560)
//#define ONE_SPACE   US(1690)
//#define ZERO_SPACE  US(560)
//#define TOL_MIN(x)  ((x) - US(150))
//#define TOL_MAX(x)  ((x) + US(150))
//
//#define RETRY_INTERVAL 10 // Time in ticks of ~32.7ms - 10 ticks * 32.7ms = ~327ms
//
//MinesweeperIR Minenet;
//
//// TX State
//volatile bool     _tx_active = false;
//volatile uint32_t _tx_shift = 0;
//volatile int8_t   _tx_bits = 0;
//volatile uint8_t  _tx_state = 0;
//
//// RX State
//volatile uint8_t  _rx_state = 0;
//volatile uint32_t _rx_shift = 0;
//volatile uint8_t  _rx_bits = 0;
//
//// ARQ State
//volatile uint32_t _last_packet = 0;
//volatile bool     _waiting_ack = false;
//volatile uint8_t  _tx_seq = 0;
//volatile uint16_t _retry_counter = 0;
//
//inline void carrier_on()  {
//    TCNT2 = 0;
//    TIMSK2 |= (1 << OCIE2A);
//}
//
//inline void carrier_off() {
//    TIMSK2 &= ~(1 << OCIE2A);
//    PORTD &= ~(1 << PORTD6);
//}
//
//
//void MinesweeperIR::_handleOverflow() {
//    // This fires every ~32.7ms (65536 * 0.5us) IF timer is not reset
//    // If we are receiving or sending, TCNT1 is reset frequently, so this won't fire.
//    // That is perfect: we only count retries when the line is IDLE.
//
//    if (_waiting_ack && !_tx_active) {
//        _retry_counter++;
//
//        if (_retry_counter > RETRY_INTERVAL) {
//            _retry_counter = 0;
//            _startTransmit(_last_packet);
//        }
//    }
//}
//
//// ==========================================
//// CLASS IMPLEMENTATION
//// ==========================================
//
//void MinesweeperIR::begin(uint8_t sessionID) {
//    _sessionID = sessionID;
//    _cmdReady = false;
//
//    // 1. TIMER 2: Software Carrier (TX on Pin 6)
//    // CTC Mode, Prescaler 1, OCR=210 -> ~76kHz interrupt
//    DDRD |= (1 << DDD6);
//    TCCR2A = (1 << WGM21);
//    TCCR2B = (1 << CS20);
//    OCR2A = 210;
//
//    // 2. TIMER 1: Pulse Timing
//    // Normal Mode, Prescaler 8 -> 0.5us ticks
//    TCCR1A = 0;
//    TCCR1B = (1 << CS11);
//
//    // 3. RX INTERRUPT (Pin 2)
//    DDRD &= ~(1 << DDD2);
//    PCICR |= (1 << PCIE2);
//    PCMSK2 |= (1 << PCINT18);
//
//    sei();
//}
//
//bool MinesweeperIR::send(uint8_t type, uint8_t data) {
//    if (_tx_active) return false;
//    uint32_t pkt = _buildPacket(type, data, _tx_seq);
//    _startTransmit(pkt);
//    return true;
//}
//
//bool MinesweeperIR::available() {
//    if (_cmdReady) {
//        _cmdReady = false;
//        return true;
//    }
//    return false;
//}
//
//uint8_t MinesweeperIR::getCommandType() { return _lastType; }
//uint8_t MinesweeperIR::getCommandData() { return _lastData; }
//
//// -- Internal Logic --
//
//uint32_t MinesweeperIR::_buildPacket(uint8_t type, uint8_t data, uint8_t seq) {
//    uint8_t control = (_sessionID << 4) | ((seq & 1) << 3) | (type & 0x7);
//    uint8_t crc = (MAGIC_BYTE + control + data) & 0xFF;
//
//    return ((uint32_t) MAGIC_BYTE << 24) | ((uint32_t) control << 16) | ((uint32_t) data << 8) | crc;
//}
//
////uint32_t MinesweeperIR::_buildPacket(uint8_t type, uint8_t data) {
////    uint8_t b1 = (_sessionID << 4) | ((_tx_seq & 1) << 3) | (type & 0x7);
////    uint8_t crc = (MAGIC_BYTE + b1 + data) & 0xFF;
////    return ((uint32_t)MAGIC_BYTE << 24) | ((uint32_t)b1 << 16) | ((uint32_t)data << 8) | crc;
////}
//
//void MinesweeperIR::_startTransmit(uint32_t packet) {
//    if (_tx_active) return;
//    _last_packet = packet;
//    _tx_shift = packet;
//    _tx_bits = 32;
//    _tx_state = 0;
//    _tx_active = true;
//
//    // Disable RX during TX
//    PCMSK2 &= ~(1 << PCINT18);
//
//    TCNT1 = 0;
//    OCR1A = HDR_MARK;
//    TIMSK1 |= (1 << OCIE1A);
//    carrier_on();
//}
//
//// -- Interrupt Handlers (Trampolines) --
//
//void MinesweeperIR::_handleTxTimer() {
//    TCNT1 = 0;
//
//    switch (_tx_state) {
//        case 0: carrier_off(); OCR1A = HDR_SPACE; _tx_state = 1; break;
//        case 1: carrier_on();  OCR1A = BIT_MARK;  _tx_state = 2; break;
//        case 2:
//            carrier_off();
//            if (_tx_shift & 0x80000000) OCR1A = ONE_SPACE; else OCR1A = ZERO_SPACE;
//            _tx_shift <<= 1;
//            _tx_state = 3;
//            break;
//        case 3:
//            _tx_bits--;
//            if (_tx_bits > 0) { carrier_on(); OCR1A = BIT_MARK; _tx_state = 2; }
//            else { carrier_on(); OCR1A = BIT_MARK; _tx_state = 4; }
//            break;
//        case 4:
//            carrier_off();
//            _tx_active = false;
//            TIMSK1 &= ~(1 << OCIE1A);
//            PCIFR  |= (1 << PCIF2);
//            PCMSK2 |= (1 << PCINT18); // Re-enable RX
//            _waiting_ack = true;
//            _retry_counter = 0;
//            break;
//    }
//}
//
//void MinesweeperIR::_handleCarrierTimer() {
//    PIND = (1 << PIND6); // Toggle Pin 6
//}
//
//void MinesweeperIR::_handleRxInterrupt() {
//    uint16_t width = TCNT1;
//
//    TCNT1 = 0;
//
//    bool pin_high = PIND & (1 << PIND2); // Read Pin 2
//
//    if (_rx_state == 0) {
//        if (pin_high && width > TOL_MIN(HDR_MARK) && width < TOL_MAX(HDR_MARK)) {
//            _rx_state = 1;
//        } else if (pin_high) {
////            Serial.println(width);
//        }
//    }
//    else if (_rx_state == 1) {
//        if (!pin_high && width > TOL_MIN(HDR_SPACE) && width < TOL_MAX(HDR_SPACE)) {
//            _rx_bits = 0;
//            _rx_shift = 0;
//            _rx_state = 2;
//        } else {
//            _rx_state = 0;
//        }
//    }
//    else if (_rx_state == 2) {
//        if (!pin_high) {
//            _rx_shift <<= 1;
//
//            if (width > TOL_MIN(ONE_SPACE) && width < TOL_MAX(ONE_SPACE)) {
//                _rx_shift |= 1;
//            }
//                // Check for '0' (Short Space)
//            else if (width > TOL_MIN(ZERO_SPACE) && width < TOL_MAX(ZERO_SPACE)) {
//                // It is a 0, do nothing (bit is already 0 from shift)
//            }
//            else {
//                _rx_state = 0;
//                return;
//            }
//
//            _rx_bits++;
//
//            // Check if we have a complete packet
//            if (_rx_bits == 32) {
//                uint8_t r_sess  = (_rx_shift >> 20) & 0xF;
//                uint8_t r_seq   = (_rx_shift >> 19) & 1;
//                uint8_t r_type  = (_rx_shift >> 16) & 0x7;
//                uint8_t r_data  = (_rx_shift >> 8)  & 0xFF;
//                uint8_t r_magic = (_rx_shift >> 24);
//
//                // Verify the magic byte and session ID
//                if (r_magic == MAGIC_BYTE && (r_sess == _sessionID || r_sess == 0)) {
//                    // Check if a received packet is an acknowledgement
//                    if (r_type == MSG_ACK) {
//                        if (_waiting_ack && r_seq == _tx_seq) {
//                            _waiting_ack = false;
//                            _tx_seq = !_tx_seq;
//                        }
//                    } else {
//                        // Acknowledge the packet with the same sequence number
//                        uint32_t ack = _buildPacket(MSG_ACK, 0, r_seq);
//
//                        _startTransmit(ack);
//                        _waiting_ack = false;
//
//                        if (!_cmdReady) {
//                            _lastType = r_type;
//                            _lastData = r_data;
//                            _cmdReady = true;
//                        }
//                    }
//                }
//
//                _rx_state = 0;
//            }
//        }
//    }
//}
//
//ISR(TIMER1_COMPA_vect) { Minenet._handleTxTimer(); }
//ISR(TIMER2_COMPA_vect) { Minenet._handleCarrierTimer(); }
//ISR(PCINT2_vect)       { Minenet._handleRxInterrupt(); }
//ISR(TIMER1_OVF_vect)   { Minenet._handleOverflow(); }
