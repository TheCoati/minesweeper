//#include "Arduino.h"
//
//#define GRID_ROWS 9
//#define GRID_COLS 9
//#define GRID_SIZE 81   // 9 * 9
//#define MINE_COUNT 10
//
//uint8_t grid[GRID_ROWS][GRID_COLS];
//
//void generateFromSeed(uint8_t seed) {
//    // 1. Fast clear the board
//    memset(grid, 0, sizeof(grid));
//
//    // 2. Initialize Random State
//    // If seed is 0, force a default to prevent "stuck" math
//    uint16_t rng_state = (seed == 0) ? 123 : seed;
//
//    // 3. Place Mines
//    uint8_t minesPlaced = 0;
//
//    while (minesPlaced < MINE_COUNT) {
//
//        // --- LINEAR CONGRUENTIAL GENERATOR (LCG) ---
//        // A = 2053 (Prime), C = 13849 (Prime)
//        rng_state = (rng_state * 2053) + 13849;
//
//        // Extract "Random" Index (0 to 80)
//        // We use the top 8 bits (>> 8) because they are more random than bottom bits
//        uint8_t index = (rng_state >> 8) % GRID_SIZE;
//
//        // Convert Index -> Coordinates
//        uint8_t r = index / GRID_COLS;
//        uint8_t c = index % GRID_COLS;
//
//        // --- DUPLICATE CHECK ---
//        // Only place if the spot is currently empty
//        if (grid[r][c] == 0) {
//            grid[r][c] = 1;
//            minesPlaced++;
//        }
//    }
//}
//
//bool isMine(uint8_t row, uint8_t col) {
//    if (row >= GRID_ROWS || col >= GRID_COLS) return false;
//    return (grid[row][col] == 1);
//}
//
//uint8_t generateSeed() {
//    // Generate random noise from the timer
//    uint16_t noise = TCNT1;
//
//    return (noise ^ 0xA5) & 0xFF;
//}
//
//
//void test() {
//    uint8_t seed = generateSeed();
//    generateFromSeed(seed);
//}
