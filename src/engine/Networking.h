#ifndef MINESWEEPER_NETWORKING_H
#define MINESWEEPER_NETWORKING_H

#include <Minenet.h>
#include "String.h"

#define MAX_OPCODES 10

typedef uint8_t OpCode;
typedef void (*NetworkCallback)(MinenetPacket packet);

class Networking {
public:
    void begin();
    void tick();
    void registerCallback(OpCode opCode, NetworkCallback callback);
    void clearCallbacks();
private:
    void triggerCallback(MinenetPacket packet);
    OpCode opCodes[MAX_OPCODES];
    NetworkCallback callbacks[MAX_OPCODES];
    uint8_t count;
};

extern Networking Networking;

#endif //MINESWEEPER_NETWORKING_H
