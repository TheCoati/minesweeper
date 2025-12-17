#include "GameScene.h"

#include "engine/Controller.h"
#include "engine/SceneManager.h"
#include "MainMenuScene.h"

void GameScene::onBegin() {
    tft.setCursor(0, 0);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);

    tft.println("Game");
    tft.println("[C] - Exit");
}

void GameScene::onTick() {
    if (Controller.available()) {
        ControllerAction action = Controller.read();

        switch (action) {
            case SECONDARY:
                this->onSecondaryPress();
                break;
            default:
                break;
        }
    }
}

void GameScene::onDestroy() {

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

    bool isHighNibble = (index % 2 == 0);

    return (isHighNibble) ? (gridRegister[index / 2] & 0xF0) >> 4 : gridRegister[index / 2] & 0x0F;
}

/**
 * Increments the adjacent mines value a field by 1.
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


void GameScene::onSecondaryPress() {
    SceneManager.switchScene(new MainMenuScene());
}
