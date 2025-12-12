#include "Game.h"

#define GRID_SIZE 9
#define TOTAL_FIELDS (GRID_SIZE * GRID_SIZE)
#define GRID_REGISTER_SIZE 41   // 81 fields, 2 per byte = 41 bytes (1 nibble unused)
#define FIELD_REGISTER_SIZE 11  // 81 fields, 1 bit per field = 11 bytes (7 bits unused)

uint8_t active = true;
uint8_t gridRegister[GRID_REGISTER_SIZE];
uint8_t fieldRegister[FIELD_REGISTER_SIZE];
uint8_t currentSeed = 0;
uint8_t cursorPosition = 0;
uint8_t minesCount = 10;

extern void destroyGame();
extern void handleInput();
extern uint8_t getFieldValue(uint8_t);
extern void openField(uint8_t);
extern void openEmptyNeighbors(uint8_t);
extern void resetField();

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

    uint8_t fieldValue = getFieldValue(index);

    drawOpen(index, fieldValue);

    return fieldValue;
}

/**
 * Get the value of a field.
 * 0 = Empty
 * 1-8 = Number of adjacent mines
 * 9 = Mine
 * @param index The index of the field.
 * @return The value of the field.
 */
uint8_t getFieldValue(uint8_t index) {
    if (index > TOTAL_FIELDS - 1)
        return 255;

    bool isHighNibble = (index % 2 == 0);

    return (isHighNibble) ? (gridRegister[index / 2] & 0xF0) >> 4 : gridRegister[index / 2] & 0x0F;
}

/**
 * Increments the adjacent mines value a field by 1.
 * @param index The index of the field.
 */
void incrementFieldValue(uint8_t index) {
    if (index > TOTAL_FIELDS - 1)
        return;

    uint8_t value = getFieldValue(index);

    if (value >= 8)
        return;  // Do not increment if the value is already 8 or a mine

    // Check if high or low nibble
    bool isHighNibble = (index % 2 == 0);

    value++;

    uint8_t byteValue = gridRegister[index / 2];

    // Mask local stored value
    byteValue &= (isHighNibble) ? 0x0F : 0xF0;
    byteValue |= (isHighNibble) ? value << 4 : value;

    // Write local value back to the grid
    gridRegister[index / 2] = byteValue;
}

/**
 * Increments the adjacent mine counts for all fields around each mine.
 */
void incrementFields() {
    for (uint8_t index = 0; index < TOTAL_FIELDS; index++) {
        uint8_t value = getFieldValue(index);

        if (value == 9) {
            int8_t row, col;
            indexToCoords(index, &row, &col);

            // Loop through 3x3 grid around the mine
            for (int8_t r = row - 1; r <= row + 1; r++) {
                if (r < 0 || r >= 9)
                    continue;  // Skip out of bounds rows

                for (int8_t c = col - 1; c <= col + 1; c++) {
                    if (c < 0 || c >= 9)
                        continue;  // Skip out-of-bounds columns

                    uint8_t neighborPos = coordsToIndex(r, c);

                    if (neighborPos != index) {
                        incrementFieldValue(neighborPos);
                    }
                }
            }
        }
    }
}

/**
 * Fills the field with mines and numbers based on a seed.
 * @param seed The seed to use for mine placement.
 */
void fillGrid() {
    // Fill the grid with empty fields
    for (uint8_t i = 0; i < GRID_REGISTER_SIZE; i++) {
        gridRegister[i] = 0;
    }

    uint8_t minesPlaced = 0;

    while (minesPlaced < minesCount) {
        // Generate a random position for mine
        currentSeed = mutateSeed(currentSeed);
        uint8_t pos = currentSeed % TOTAL_FIELDS;

        // Get byte index in grid register
        uint8_t byteIndex = pos / 2;

        // Check if high or low nibble
        bool isHighNibble = (pos % 2 == 0);

        if (isHighNibble) {
            // Get the value of the nibble and check if it's already a mine
            uint8_t isMine = (gridRegister[byteIndex] & 0x90);

            if (isMine == 0) {
                // Place mine on a given position
                gridRegister[byteIndex] |= 0x90;
                minesPlaced++;
            }
        } else {
            // Get the value of the nibble and check if it's already a mine
            uint8_t isMine = (gridRegister[byteIndex] & 0x09);

            if (isMine == 0) {
                // Place mine on a given position
                gridRegister[byteIndex] |= 0x09;
                minesPlaced++;
            }
        }
    }

    incrementFields();
}

/**
 * Checks if a field is open.
 * @param index The index of the field.
 * @return True if the field is open, false otherwise.
 */
bool isFieldOpen(uint8_t index) {
    if (index > TOTAL_FIELDS - 1)
        return false;

    uint8_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;

    return (fieldRegister[byteIndex] & (0x80 >> bitIndex)) != 0;
}

/**
 * Opens a field at the given index.
 * @param index The index of the field to open.
 */
void openField(uint8_t index) {
    if (index > TOTAL_FIELDS - 1)
        return;

    if (isFieldOpen(index))
        return;

    uint8_t fieldValue = openFieldAndGetValue(index);

    // Open neighboring fields if the field is empty
    if (fieldValue == 0) {
        openEmptyNeighbors(index);
    } else if (fieldValue == 9) {
        // TODO: game over code
        _delay_ms(1000);
        resetField();
    }
}

/**
 * Opens all empty neighboring fields around a given index.
 * @param index The index of the field to check neighbors for.
 */
void openEmptyNeighbors(uint8_t index) {
    if (index > TOTAL_FIELDS - 1)
        return;

    // Queue to hold field indices to visit
    uint8_t queue[TOTAL_FIELDS];
    uint8_t queueStart = 0;
    uint8_t queueEnd = 0;

    // Add the initial field to the queue
    queue[queueEnd++] = index;

    while (queueStart != queueEnd) {
        // Dequeue a field index.
        uint8_t currentIndex = queue[queueStart++];

        // Get coordinates for the current field
        int8_t row, col;
        indexToCoords(currentIndex, &row, &col);

        // Iterate through all neighbors (3x3 grid)
        for (int8_t r = row - 1; r <= row + 1; r++) {
            if (r < 0 || r >= GRID_SIZE)
                continue;

            for (int8_t c = col - 1; c <= col + 1; c++) {
                if (c < 0 || c >= GRID_SIZE)
                    continue;

                uint8_t neighborIndex = coordsToIndex(r, c);

                // Skip if the neighbor is already open
                if (isFieldOpen(neighborIndex))
                    continue;

                // Open the neighbor field
                uint8_t neighborValue = openFieldAndGetValue(neighborIndex);

                // If the neighbor is also empty, add it to the queue to process its neighbors
                if (neighborValue == 0) {
                    queue[queueEnd++] = neighborIndex;
                }
            }
        }
    }
}

/**
 * Redraws a tile at the given index based on its state (open/closed).
 * @param index The index of the tile to redraw.
 */
void redrawTile(uint8_t index) {
    uint8_t isOpen = isFieldOpen(index);

    if (isOpen) {
        uint8_t value = getFieldValue(index);

        drawOpen(index, value);
        return;
    }

    drawClosed(index);
}

/**
 * Moves the cursor to a new index.
 * @param index The index to move the cursor to.
 */
void moveCursorTo(uint8_t index) {
    if (index < 0 || index > 80)
        return;

    redrawTile(cursorPosition);
    drawCursor(index);

    cursorPosition = index;
}

/**
 * Moves the cursor left.
 */
void moveLeft() {
    if (cursorPosition % GRID_SIZE == 0) {
        return;
    }

    moveCursorTo(cursorPosition - 1);
}

/**
 * Moves the cursor right.
 */
void moveRight() {
    if (cursorPosition % GRID_SIZE == GRID_SIZE - 1) {
        return;
    }

    moveCursorTo(cursorPosition + 1);
}

/**
 * Moves the cursor up.
 */
void moveUp() {
    if (cursorPosition / GRID_SIZE == 0) {
        return;
    }

    moveCursorTo(cursorPosition - GRID_SIZE);
}

/**
 * Moves the cursor down.
 */
void moveDown() {
    if (cursorPosition / GRID_SIZE == GRID_SIZE - 1) {
        return;
    }
    moveCursorTo(cursorPosition + GRID_SIZE);
}

/**
 * Handles input from the controller.
 */
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

    if (isSecondaryPressed()) {
        destroyGame();  // Todo: Remove reset on final product
    }
}

/**
 * Called on each game tick.
 */
void onTick() {
    handleInput();
}

/**
 * Resets the field to its initial state.
 */
void resetField() {
    cursorPosition = 0;

    for (uint8_t i = 0; i < FIELD_REGISTER_SIZE; i++) {
        fieldRegister[i] = 0;
    }

    fillGrid();

    for (uint8_t i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        redrawTile(i);
    }

    drawCursor(cursorPosition);
}

/**
 * Starts the game with a given seed.
 * @param seed The seed to use for mine placement.
 */
void startGame(uint8_t seed) {
    currentSeed = seed;

    drawField();
    resetField();

    active = true;

    // Todo: Move game ticking back to main loop?
    while (active) {
        onTick();
    }
}

/**
 * Destroy the game instance.
 */
void destroyGame() {
    active = false;

    drawMenu();
}
