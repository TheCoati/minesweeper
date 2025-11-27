#ifndef MINESWEEPER_GAME_MINENET_H
#define MINESWEEPER_GAME_MINENET_H

#include <stdint.h>

class MinenetProtocol {
public:
    void begin();
    bool send(uint8_t op, uint8_t payload);

    void handleTxTimer();
    void handleRxInterrupt();

    uint8_t clientId() const { return clientId_; }
    void setClientId(uint8_t id) { clientId_ = id; }

    uint8_t sessionId() const { return sessionId_; }
    void setSessionId(uint8_t id) { sessionId_ = id; }

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
private:
    uint8_t clientId_;
    uint8_t sessionId_;

    bool transmit(uint32_t packet);
};

extern MinenetProtocol Minenet;

#endif //MINESWEEPER_GAME_MINENET_H
