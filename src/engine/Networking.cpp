#include "Networking.h"

void Networking::begin() {
    Minenet.begin();
}

void Networking::tick() {
    if (Minenet.available()) {
        MinenetPacket packet = Minenet.read();
        triggerCallback(packet);
    }
}

void Networking::registerCallback(OpCode opCode, NetworkCallback callback) {
    if (this->count < MAX_OPCODES) {
        this->opCodes[this->count] = opCode;
        this->callbacks[this->count] = callback;
        this->count++;
    }
}

void Networking::triggerCallback(MinenetPacket packet) {
    for (uint8_t i = 0; i < this->count; i++) {
        if (this->opCodes[i] == packet.opCode) {
            this->callbacks[i](packet);
            return;
        }
    }
}

void Networking::clearCallbacks() {
    memset(callbacks, 0, sizeof callbacks);
}

class Networking Networking;