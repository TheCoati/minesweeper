#ifndef MINESWEEPER_GAME_SCENE_H
#define MINESWEEPER_GAME_SCENE_H

#include "Engine/Scene.h"
#include "Engine/SceneManager.h"
#include "Game/Minesweeper.h"
#include "Peripherals/Controller.h"
#include "Peripherals/Screen.h"
#include "Peripherals/SegmentDisplay.h"

#define GRID_SIZE 9
#define TOTAL_FIELDS (GRID_SIZE * GRID_SIZE)
#define GRID_REGISTER_SIZE 41   // 81 fields, 2 per byte = 41 bytes (1 nibble unused)
#define FIELD_REGISTER_SIZE 11  // 81 fields, 1 bit per field = 11 bytes (7 bits unused)
#define LIVES 2

class GameScene : public Scene {
public:
   explicit GameScene(uint8_t seed, bool multiplayer, bool hasTurn);

protected:
   void onBegin() override;
   void onTick() override;
   void onDestroy() override;

private:
    uint8_t gridRegister[GRID_REGISTER_SIZE]{};
    uint8_t fieldRegister[FIELD_REGISTER_SIZE]{};
    uint8_t cursorPosition = 0;
    uint8_t minesCount = 10;
    uint8_t fieldsOpened = 0;
    uint8_t livesLeft = LIVES;

    uint8_t neightbourQueue[TOTAL_FIELDS]{};
    uint8_t neightbourHead = 0;
    uint8_t neightbourTail = 0;

    uint8_t currentSeed = 183;
    bool multiplayer = false;
    bool hasTurn = true;

    static inline uint8_t mutateSeed(uint8_t seed);
    static inline uint8_t coordsToIndex(int8_t row, int8_t col);
    static inline void indexToCoords(uint8_t index, int8_t *row, int8_t *col);
    static inline void indexToScreenCoords(uint8_t index, uint8_t *row, uint8_t *col);
    uint8_t openFieldAndGetValue(uint8_t index);
    bool isFieldOpen(uint8_t index);
    uint8_t getFieldValue(uint8_t index);
    void redrawTile(uint8_t index);

    void moveCursorTo(uint8_t index);
    void openField(uint8_t index);
    void openEmptyNeighbors(uint8_t index);
    void resetField();

    void fillGrid();
    void incrementFields();
    void incrementFieldValue(uint8_t index);

    static inline String getFieldImage(uint8_t value);
    static inline uint16_t getFieldColor(uint8_t value);
    void drawOpen(uint8_t index, uint8_t value);
    void drawClosed(uint8_t index);
    void drawCursor(uint8_t index);

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    static void onSecondaryPress();
};

#endif //MINESWEEPER_GAME_SCENE_H
