#include "GameScene.h"

#include <config.h>
#include <Minenet.h>
#include "Game/Scenes/MainMenuScene.h"

GameScene::GameScene(uint8_t seed, bool multiplayer, uint8_t clientId) {
    this->currentSeed = seed;
    this->multiplayer = multiplayer;
    this->clientId = clientId;
    this->hasTurn = multiplayer && clientId == 0x01;
}

void GameScene::onBegin() {
    tft.setCursor(0, 0);
    tft.fillScreen(MENU_BACKGROUND_COLOR);

    if (Screen.hasSDCard()) {
        Screen.getReader().drawBMP("/grid.bmp", tft, GRID_MARGIN, GRID_MARGIN);
    }

    if (multiplayer) {
        Minenet.send(clientId, 0x00, 0x02, currentSeed);
    }

    resetField();
}

void GameScene::onTick() {
    if (hasTurn && Controller.available())  {
        ControllerAction action = Controller.read();

        switch (action) {
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
            case SECONDARY:
                onSecondaryPress();
                break;
            case PRIMARY:
                openField(cursorPosition);
            default:
                break;
        }
    }

    if (multiplayer && Minenet.available()) {
        MinenetPacket packet = Minenet.read();

        if (packet.clientId == clientId) {
            return;
        }

        switch (packet.opCode) {
            case 0x03: // Move cursor
                moveCursorTo(packet.payload);
                break;
            case 0x04: // Open field
                openField(packet.payload);
                hasTurn = true;
                break;
        }
    }
}

void GameScene::onDestroy() {
    SegmentDisplay.setValue(DISPLAY_OFF);

    memset(gridRegister, 0, sizeof(gridRegister));
    memset(fieldRegister, 0, sizeof(fieldRegister));
}

/*
 |--------------------------------------------------------------------------
 | Helpers
 |--------------------------------------------------------------------------
 */

/**
 * Mutates the seed to randomize mine placement.
 * @param seed The current seed.
 * @return The mutated seed.
 */
inline uint8_t GameScene::mutateSeed(uint8_t seed) {
    seed = (seed * 13) + 7;

    return seed;
}

/**
 * Converts a 1D index to 2D coordinates.
 * @param index The 1D index.
 * @param row The row coordinate.
 * @param col The column coordinate.
 */
inline void GameScene::indexToCoords(uint8_t index, int8_t *row, int8_t *col) {
    *row = index / GRID_SIZE;
    *col = index % GRID_SIZE;
}

/**
 * Converts 2D coordinates to a 1D index.
 * @param row The row coordinate.
 * @param col The column coordinate.
 * @return The 1D index.
 */
inline uint8_t GameScene::coordsToIndex(int8_t row, int8_t col) {
    return (uint8_t) (row * GRID_SIZE + col);
}

/**
 * Converts a 1D index to 2D on screen coordinates.
 * @param index The 1D index.
 * @param x
 * @param y
 */
inline void GameScene::indexToScreenCoords(uint8_t index, uint8_t *x, uint8_t *y) {
    *x = index % GRID_SIZE;
    *y = index / GRID_SIZE;

    if (Screen.hasSDCard()) {
        *x *= GRID_COL_SIZE;
        *y *= GRID_COL_SIZE;

        *x += (GRID_BORDER_PADDING + GRID_MARGIN);
        *y += (GRID_BORDER_PADDING + GRID_MARGIN);
    }
    #ifndef ALLOW_NO_SD_CARD
    else {
        *x *= GRID_COL_SIZE + 1; // +1 Grid spacing
        *y *= GRID_COL_SIZE + 1; // +1 Grid spacing
    }
    #endif
}

/**
 * Opens a field and returns its value.
 * @param index The index of the field to open.
 * @return The value of the opened field.
 */
uint8_t GameScene::openFieldAndGetValue(uint8_t index) {
    uint8_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;

    // Open the field in the register
    fieldRegister[byteIndex] |= (0x80 >> bitIndex);
    fieldsOpened++;

    uint8_t fieldValue = getFieldValue(index);

    drawOpen(index, fieldValue);

    return fieldValue;
}

/**
 * Checks if a field is open.
 * @param index The index of the field.
 * @return True if the field is open, false otherwise.
 */
bool GameScene::isFieldOpen(uint8_t index) {
    if (index > TOTAL_FIELDS - 1)
        return false;

    uint8_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;

    return (fieldRegister[byteIndex] & (0x80 >> bitIndex)) != 0;
}

/**
 * Get the value of a field.
 * 0 = Empty
 * 1-8 = Number of adjacent mines
 * 9 = Mine
 * @param index The index of the field.
 * @return The value of the field.
 */
uint8_t GameScene::getFieldValue(uint8_t index) {
    if (index > TOTAL_FIELDS - 1)
        return 255;

    bool isHighNibble = ((index & 1) == 0);
    uint8_t byte = gridRegister[index >> 1];

    return isHighNibble ? ((byte >> 4) & 0x0F) : (byte & 0x0F);
}

/**
 * Redraws a tile at the given index based on its state (open/closed).
 * @param index The index of the tile to redraw.
 */
void GameScene::redrawTile(uint8_t index) {
    uint8_t isOpen = isFieldOpen(index);

    if (isOpen) {
        uint8_t value = getFieldValue(index);

        drawOpen(index, value);
        return;
    }

    drawClosed(index);
}

/*
 |--------------------------------------------------------------------------
 | Game Logic
 |--------------------------------------------------------------------------
 */

/**
 * Moves the cursor to a new index.
 * @param index The index to move the cursor to.
 */
void GameScene::moveCursorTo(uint8_t index) {
    if (index < 0 || index > 80)
        return;

    if (multiplayer && hasTurn) {
        Minenet.send(clientId, 0x00, 0x03, index);
    }

    redrawTile(cursorPosition);
    drawCursor(index);

    cursorPosition = index;
}

/**
 * Opens a field at the given index.
 * @param index The index of the field to open.
 */
void GameScene::openField(uint8_t index) {
    if (index > TOTAL_FIELDS - 1)
        return;

    if (isFieldOpen(index))
        return;

    uint8_t fieldValue = openFieldAndGetValue(index);

    // Draw cursor over opened field
    drawCursor(index);

    if (multiplayer && hasTurn) {
        hasTurn = false;

        Minenet.send(clientId, 0x00, 0x04, index);
    }

    if (fieldValue == 0) {
        // Hit an empty field
        // Open neighboring fields if the field is empty
        openEmptyNeighbors(index);
    } else if (fieldValue == 9) {
        // Hit a mine
        if (livesLeft == 0) {
            // Game over - no more lives left
            _delay_ms(1000);  // Intentional blocking
            SceneManager.unloadScene();
            SceneManager.switchScene(new MainMenuScene()); // Todo: Game over scene?
            return;
        }

        SegmentDisplay.setValue(livesLeft -= 1);

        _delay_ms(1000);  // Intentional blocking
        resetField();
        return;
    }

    if (fieldsOpened >= (TOTAL_FIELDS - minesCount))
    {
        _delay_ms(1000);  // Intentional blocking
        resetField();
    }
}

/**
 * Opens all empty neighboring fields around a given index.
 * @param index The index of the field to check neighbors for.
 */
void GameScene::openEmptyNeighbors(uint8_t index) {
    if (index > TOTAL_FIELDS - 1)
        return;

    neightbourHead = 0;
    neightbourTail = 0;

    // Add the initial field to the queue
    neightbourQueue[neightbourTail++] = index;

    if (neightbourTail == TOTAL_FIELDS) {
        neightbourTail = 0;
    }

    while (neightbourHead != neightbourTail) {
        // Dequeue a field index.
        uint8_t currentIndex = neightbourQueue[neightbourHead++];

        if (neightbourHead == TOTAL_FIELDS) {
            neightbourHead = 0;
        }

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
                    uint8_t nextTail = neightbourTail + 1;

                    if (nextTail == TOTAL_FIELDS) {
                        nextTail = 0;
                    }

                    if (nextTail == neightbourHead) {
                        continue;
                    }

                    neightbourQueue[neightbourTail] = neighborIndex;
                    neightbourTail = nextTail;
                }
            }
        }
    }
}

/**
 * Resets the field to its initial state.
 */
void GameScene::resetField() {
    cursorPosition = TOTAL_FIELDS / 2;
    fieldsOpened = 0;

    memset(fieldRegister, 0, sizeof(fieldRegister));

    fillGrid();

    for (uint8_t i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        redrawTile(i);
    }

    SegmentDisplay.setValue(livesLeft);

    drawCursor(cursorPosition);
}

/*
 |--------------------------------------------------------------------------
 | Game Setup
 |--------------------------------------------------------------------------
 */

/**
 * Fills the field with mines and numbers based on a seed.
 * @param seed The seed to use for mine placement.
 */
void GameScene::fillGrid() {
    // Fill the grid with empty fields
    memset(gridRegister, 0, sizeof(gridRegister));

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
 * Increments the adjacent mine counts for all fields around each mine.
 */
void GameScene::incrementFields() {
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
 * Increments the adjacent mines value a field by 1.
 * This method is used to increment the value of a field when a mine is adjacent to it.
 * @param index The index of the field.
 */
void GameScene::incrementFieldValue(uint8_t index) {
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

/*
 |--------------------------------------------------------------------------
 | Rendering
 |--------------------------------------------------------------------------
 */

#ifndef ALLOW_NO_SD_CARD

/**
 * Get the color of a field based on its value.
 * @note This method is only used in dev mode.
 * @param value The value of the field.
 * @return The color of the field.
 */
inline uint16_t GameScene::getFieldColor(uint8_t value) {
    switch (value) {
        case 0:
            return ILI9341_WHITE;
        case 1:
            return ILI9341_BLUE;
        case 2:
            return ILI9341_GREEN;
        case 3:
            return ILI9341_YELLOW;
        case 4:
            return ILI9341_PINK;
        case 5:
            return ILI9341_ORANGE;
        case 6:
            return ILI9341_PURPLE;
        case 7:
            return ILI9341_CYAN;
        case 8:
            return ILI9341_LIGHTGREY;
        case 9:
            return ILI9341_DARKGREY;
    }

    return ILI9341_WHITE;
}

#endif

/**
 * Get the image path of a field based on its value.
 * @param value The value of the field.
 * @return The image path of the field.
 */
inline String GameScene::getFieldImage(uint8_t value) {
    switch (value) {
        case 0:
            return F("/open.bmp");
        case 1:
            return F("/open_1.bmp");
        case 2:
            return F("/open_2.bmp");
        case 3:
            return F("/open_3.bmp");
        case 4:
            return F("/open_4.bmp");
        case 5:
            return F("/open_5.bmp");
        case 6:
            return F("/open_6.bmp");
        case 7:
            return F("/open_7.bmp");
        case 8:
            return F("/open_8.bmp");
        default:
        case 9:
            return F("/mine.bmp");
    }
}

/**
 * Draws an open field on the screen.
 * @param index The index of the field to draw.
 * @param value The value of the field to draw.
 */
void GameScene::drawOpen(uint8_t index, uint8_t value) {
    uint8_t x, y;
    indexToScreenCoords(index, &x, &y);

    if (Screen.hasSDCard()) {
        String path = getFieldImage(value);
        Screen.getReader().drawBMP(path.c_str(), tft, x, y);
        return;
    }

    #ifndef ALLOW_NO_SD_CARD

    uint16_t color = getFieldColor(value);
    tft.fillRect(x, y, GRID_COL_SIZE, GRID_COL_SIZE, color);

    if (value > 0) {
        // Draw the value on top of the field
        tft.setCursor(x + (GRID_COL_SIZE / 2 - 2), y + (GRID_COL_SIZE / 2 - 5));
        tft.println(String(value));
    }

    #endif
}

/**
 * Draws a closed field on the screen.
 * @param index The index of the field to draw.
 */
void GameScene::drawClosed(uint8_t index) {
    uint8_t x, y;
    indexToScreenCoords(index, &x, &y);

    if (Screen.hasSDCard()) {
        Screen.getReader().drawBMP("/slot.bmp", tft, x, y);
        return;
    }
    #ifndef ALLOW_NO_SD_CARD
    else {
        tft.fillRect(x, y, 24, 24, ILI9341_LIGHTGREY);
    }
    #endif
}

/**
 * Draws the cursor on the screen.
 * @param index The index of the cursor to draw.
 */
void GameScene::drawCursor(uint8_t index) {
    uint8_t x, y;
    indexToScreenCoords(index, &x, &y);

    // Draw the cursor with individual rectangles
    tft.fillRect(x, y, GRID_COL_SIZE, GRID_CURSOR_WIDTH, GRID_CURSOR_COLOR);
    tft.fillRect(x, y + (GRID_COL_SIZE - GRID_CURSOR_WIDTH), GRID_COL_SIZE, GRID_CURSOR_WIDTH, GRID_CURSOR_COLOR);
    tft.fillRect(x, y, GRID_CURSOR_WIDTH, GRID_COL_SIZE, GRID_CURSOR_COLOR);
    tft.fillRect(x + (GRID_COL_SIZE - GRID_CURSOR_WIDTH), y, GRID_CURSOR_WIDTH, GRID_COL_SIZE, GRID_CURSOR_COLOR);
}

/*
 |--------------------------------------------------------------------------
 | Input Handling
 |--------------------------------------------------------------------------
 */

/**
 * Moves the cursor left.
 */
void GameScene::moveLeft() {
    if (cursorPosition % GRID_SIZE == 0) {
        return;
    }

    moveCursorTo(cursorPosition - 1);
}

/**
 * Moves the cursor right.
 */
void GameScene::moveRight() {
    if (cursorPosition % GRID_SIZE == GRID_SIZE - 1) {
        return;
    }

    moveCursorTo(cursorPosition + 1);
}

/**
 * Moves the cursor up.
 */
void GameScene::moveUp() {
    if (cursorPosition / GRID_SIZE == 0) {
        return;
    }

    moveCursorTo(cursorPosition - GRID_SIZE);
}

/**
 * Moves the cursor down.
 */
void GameScene::moveDown() {
    if (cursorPosition / GRID_SIZE == GRID_SIZE - 1) {
        return;
    }

    moveCursorTo(cursorPosition + GRID_SIZE);
}

/**
 * Handle secondary button press.
 * Returns from the game back to the main menu.
 */
void GameScene::onSecondaryPress() {
    SceneManager.unloadScene();
    SceneManager.switchScene(new MainMenuScene());
}
