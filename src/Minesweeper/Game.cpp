#include "Game.h"
#include "Controller.h"
#include "Screen.h"

#define GRID_SIZE 9

bool active = true;
uint8_t cursorPosition = 0;

uint8_t gridRegister[41];   // Array containing all field types
uint8_t fieldRegister[11];  // Array containing all open/close field states

extern void startGame();
extern void destroyGame();
extern void handleInput();
extern void openField(uint8_t);
extern void openEmptyNeighbors(uint8_t);

/**
 * Converts an index to x and y coordinates.
 * @param index The index to convert.
 * @param x The x coordinate of the index.
 * @param y The y coordinate of the index.
 */
inline void indexToCoords(uint8_t index, uint8_t& x, uint8_t& y) {
    x = index % GRID_SIZE;
    y = index / GRID_SIZE;
}

/**
 * Convert an index to the byte and bit index on the fields register
 * @param index The index to convert
 * @param byte The byte index in the fields register
 * @param bit The bit index in the byte
 */
inline void indexToByteBit(uint8_t index, uint8_t& byte, uint8_t& bit) {
    byte = index / 8;
    bit = index % 8;
}

/**
 * Convert x y coords back to an index
 * @param row x coord
 * @param col y coord
 * @return The index in grid register
 */
inline uint8_t coordsToIndex(int8_t row, int8_t col) {
    return (uint8_t) (row * 9 + col);
}

/**
 * Start the game
 */
void startGame() {
    active = true;

    drawField();

    while (active) {
        onTick();
    }
}

void onTick() {
    handleInput();
}

uint8_t getFieldValue(uint8_t index) {
    if (index > 80)
        return 255;

    bool isHighNibble = (index % 2 == 0);

    return isHighNibble ? (gridRegister[index / 2] & 0xF0) >> 4 : gridRegister[index / 2] & 0x0F;
}


bool isFieldOpen(uint8_t index) {
    if (index > 80)
        return false;

    uint8_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;

    return (fieldRegister[byteIndex] & (0x80 >> bitIndex)) != 0;
}

void redrawTile(uint8_t index) {
    uint8_t isOpen = isFieldOpen(index);

    if (isOpen) {
        uint8_t value = getFieldValue(index);

        drawOpen(index, value);
        return;
    }

    drawClosed(index);
}

void moveCursorTo(int8_t index) {
    redrawTile(cursorPosition);
    drawCursor(index);

    cursorPosition = index;
}

void moveLeft() {
    if (cursorPosition % GRID_SIZE == 0) {
        return;
    }

    moveCursorTo(cursorPosition - 1);
}

void moveRight() {
    if (cursorPosition % GRID_SIZE == GRID_SIZE - 1) {
        return;
    }

    moveCursorTo(cursorPosition + 1);
}

void moveUp() {
    if (cursorPosition / GRID_SIZE == 0) {
        return;
    }

    moveCursorTo(cursorPosition - GRID_SIZE);
}

void moveDown() {
    if (cursorPosition / GRID_SIZE == GRID_SIZE - 1) {
        return;
    }
    moveCursorTo(cursorPosition + GRID_SIZE);
}

void handleInput() {
    ControllerDirection direction = getJoystick();

    switch (direction) {
        case LEFT:
            moveLeft();
            break;
        case RIGHT:
            moveRight();
            break;
        case UP:
            moveUp();
            break;
        case DOWN:
            moveDown();
            break;
        default:
            break;
    }

    if (isPrimaryPressed()) {
        openField(cursorPosition);
    }
}

void openField(uint8_t index) {
    if (index > 80)
        return;

    if (isFieldOpen(index))
        return;

    uint8_t byte, bit;
    indexToByteBit(index, byte, bit);

    fieldRegister[byte] |= (0x80 >> bit);  // Open field
    uint8_t value = getFieldValue(index);  // Get field value

    drawOpen(index, value);

    if (value == 0) {
        // If the field is empty (0) open neighbors
        openEmptyNeighbors(index);
    } else if (value == 9) {
        // If field is bomb (9)
        // TODO: Game over code
    }
}

void openEmptyNeighbors(uint8_t index) {
    if (index > 80)
        return;

    uint8_t value = getFieldValue(index);

    if (value != 0) {
        return;
    }

    uint8_t row, col;
    indexToCoords(index, row, col);

    // Search 3x3 around the opened field
    for (uint8_t r = row - 1; r <= row + 1; r++) {
        for (uint8_t c = col - 1; c <= col + 1; c++) {
            // Check if within bounds of the grid
            if (r >= 0 && r < 9 && c >= 0 && c < 9) {
                uint8_t neighborPos = coordsToIndex(r, c);

                // controleren of we niet in het huidige veld zitten
                if (neighborPos != index) {
                    // veld openen
                    openField(neighborPos);
                }
            }
        }
    }
}

void destroyGame() {
    active = false;
}
