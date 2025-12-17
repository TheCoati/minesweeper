#ifndef MINESWEEPER_GAME_SCENE_H
#define MINESWEEPER_GAME_SCENE_H

#include "engine/Scene.h"
#include "game/Screen.h"

#define GRID_SIZE 9
#define TOTAL_FIELDS (GRID_SIZE * GRID_SIZE)
#define GRID_REGISTER_SIZE 41   // 81 fields, 2 per byte = 41 bytes (1 nibble unused)
#define FIELD_REGISTER_SIZE 11  // 81 fields, 1 bit per field = 11 bytes (7 bits unused)
#define LIVES 2

class GameScene : public Scene {
protected:
    void onBegin() override;
    void onTick() override;
    void onDestroy() override;
private:
     uint8_t gridRegister[GRID_REGISTER_SIZE];
     uint8_t fieldRegister[FIELD_REGISTER_SIZE];
     uint8_t currentSeed = 0;
     uint8_t cursorPosition = 0;
     uint8_t minesCount = 10;
     uint8_t fieldsOpened = 0;
     uint8_t livesLeft = LIVES;
     uint8_t clientId = 0;
     uint8_t ackOpCode = 0;
     uint8_t getFieldValue(uint8_t index);
     void incrementFieldValue(uint8_t index);
     void onSecondaryPress();

     /**
     * Mutates the seed to randomize mine placement.
     * @param seed The current seed.
     * @return The mutated seed.
     */
     inline uint8_t mutateSeed(uint8_t seed) {
         seed = (seed * 13) + 7;

         return seed;
     }

     /**
     * Converts a 1D index to 2D coordinates.
     * @param index The 1D index.
     * @param row The row coordinate.
     * @param col The column coordinate.
     */
     inline void indexToCoords(uint8_t index, int8_t *row, int8_t *col) {
         *row = index / GRID_SIZE;
         *col = index % GRID_SIZE;
     }

     /**
     * Converts 2D coordinates to a 1D index.
     * @param row The row coordinate.
     * @param col The column coordinate.
     * @return The 1D index.
     */
     inline uint8_t coordsToIndex(int8_t row, int8_t col) {
         return (uint8_t) (row * GRID_SIZE + col);
     }

     /**
     * Opens a field and returns its value.
     * @param index The index of the field to open.
     * @return The value of the opened field.
     */
     inline uint8_t openFieldAndGetValue(uint8_t index) {
         uint8_t byteIndex = index / 8;
         uint8_t bitIndex = index % 8;

         // Open the field
         fieldRegister[byteIndex] |= (0x80 >> bitIndex);
         fieldsOpened++;

         uint8_t fieldValue = getFieldValue(index);

         // drawOpen(index, fieldValue); // TODO

         return fieldValue;
     }
};

#endif //MINESWEEPER_GAME_SCENE_H
