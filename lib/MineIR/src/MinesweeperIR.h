#ifndef MINESWEEPER_IR_H
#define MINESWEEPER_IR_H

#include <stdint.h>

// ==========================================
// CONFIGURATION
// ==========================================
// Protocol Definitions
#define MAGIC_BYTE  0x4D
#define MSG_ACK     0x7

// TX on Pin 6, RX on Pin 2
// These are hardcoded because they depend on specific hardware timers/interrupts.

class MinesweeperIR {
public:
    // -- Setup --
    void begin(uint8_t sessionID);

    // -- Sending --
    // Queues a packet to be sent in the background.
    // Returns true if successful, false if busy.
    bool send(uint8_t type, uint8_t data);

    // -- Receiving --
    // Returns true if a new command is available.
    bool available();

    // Returns the last received command type.
    uint8_t getCommandType();

    // Returns the last received data.
    uint8_t getCommandData();

    // -- Internal / ISR Helpers (Do not call manually) --
    void _handleTxTimer();
    void _handleCarrierTimer();
    void _handleRxInterrupt();
    void _handleOverflow();

private:
    uint8_t _sessionID;

    // Bridge to Main
    volatile bool _cmdReady;
    volatile uint8_t _lastType;
    volatile uint8_t _lastData;

    // Protocol State
    uint32_t _buildPacket(uint8_t type, uint8_t data, uint8_t seq);
    void _startTransmit(uint32_t packet);
};

extern MinesweeperIR Minenet;

#endif