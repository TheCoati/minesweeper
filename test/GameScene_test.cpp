#define ALLOW_NO_SD_CARD
#define private public
#include "Game/Scenes/GameScene.h"
#undef private

#include <unity.h>

void test_IndexCoordsRoundtrip() {
    GameScene scene(0x42);

    for (uint8_t i = 0; i < TOTAL_FIELDS; ++i) {
        int8_t row = -1, col = -1;
        scene.indexToCoords(i, &row, &col);
        uint8_t back = scene.coordsToIndex(row, col);

        TEST_ASSERT_EQUAL_UINT8(i, back);
    }
}

void test_MutateSeedDeterministic() {
    GameScene scene(0);

    uint8_t s = 0x5A;
    uint8_t expected = static_cast<uint8_t>(s * 13 + 7);
    uint8_t mutated = scene.mutateSeed(s);

    TEST_ASSERT_EQUAL_UINT8(expected, mutated);
}

void test_FillPlacesCorrectNumberOfMines() {
    GameScene scene(0x11);

    scene.fillGrid();

    int mineCount = 0;

    for (uint8_t i = 0; i < TOTAL_FIELDS; ++i) {
        if (scene.getFieldValue(i) == 9) {
            ++mineCount;
        }
    }

    TEST_ASSERT_EQUAL_INT(static_cast<int>(scene.minesCount), mineCount);
}

void test_OpenMarksFieldOpenAndReturnsValue() {
    GameScene scene(0x99);

    scene.fillGrid();

    int target = -1;

    for (uint8_t i = 0; i < TOTAL_FIELDS; ++i) {
        if (scene.getFieldValue(i) != 9) {
            target = i;
            break;
        }
    }

    TEST_ASSERT_NOT_EQUAL(-1, target);

    uint8_t value = scene.openFieldAndGetValue(static_cast<uint8_t>(target));

    TEST_ASSERT_TRUE(scene.isFieldOpen(static_cast<uint8_t>(target)));
    TEST_ASSERT_EQUAL_UINT8(value, scene.getFieldValue(static_cast<uint8_t>(target)));
}

void setup() {
    UNITY_BEGIN();

    RUN_TEST(test_IndexCoordsRoundtrip);
    RUN_TEST(test_MutateSeedDeterministic);
    RUN_TEST(test_FillPlacesCorrectNumberOfMines);
    RUN_TEST(test_OpenMarksFieldOpenAndReturnsValue);

    UNITY_END();
}

void setUp() {
    // required by PlatformIO
}

void loop() {
    // required by PlatformIO
}

void tearDown() {
    // required by PlatformIO
}
