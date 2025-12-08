#ifndef MINESWEEPER_GAME_MINENET_H
#define MINESWEEPER_GAME_MINENET_H

#include <stdint.h>

struct MinenetPacket {
    uint8_t version;   // 4 bits
    uint8_t seq;       // 1 bit
    uint8_t clientId;  // 3 bits
    uint8_t sessionId; // 4 bits
    uint8_t opCode;    // 4 bits
    uint8_t payload;   // 8 bits
    uint8_t crc;       // 8 bits
    bool valid;        // CRC + version check result
};

class MinenetProtocol {
public:
    void begin();
    bool send(uint8_t clientId, uint8_t sessionId, uint8_t op, uint8_t payload);
    MinenetPacket read();
    bool available();

    void handleTxTimer();
    void handleRxInterrupt();

    enum TxState {
        TX_BURST = 0,
        TX_BURST_SPACE = 1,
        TX_DATA = 2,
        TX_BIT_MARK = 3,
        TX_COMPLETE = 4
    };

    enum RxState {
        RX_BURST = 0,
        RX_BURST_SPACE = 1,
        RX_DATA = 2
    };
};

extern MinenetProtocol Minenet;

#endif //MINESWEEPER_GAME_MINENET_H
