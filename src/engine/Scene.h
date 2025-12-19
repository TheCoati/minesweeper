#ifndef MINESWEEPER_SCENE_H
#define MINESWEEPER_SCENE_H

#include <stdint.h>
class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    void begin();
    void tick();
    void destroy();

protected:
    virtual void onBegin() = 0;
    virtual void onTick() = 0;
    virtual void onDestroy() = 0;

private:
    uint32_t lastSendPacket = 0;
};

#endif //MINESWEEPER_SCENE_H
