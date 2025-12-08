#include <Game.h>
/*
uint8_t grid[9][9];

//This number is sent by the protocol on start up
uint8_t seedNumber = 185;

//This number cannot be higher than 30 when using a 9x9 grid
uint8_t mineNumber = 20;

//To save the cords of the selected space in the field temporarily
uint8_t columnNumber = 0;
uint8_t rowNumber = 0;

//Formula for generating the row and column number: (Seed * 13) + 7 % 9

void fillField() {

  //Fill the gridarray entirely with 0's
  memset(grid, 0, sizeof(grid));

  for (uint8_t i = 0; i < mineNumber; i++) {
    //The formula is chopped up to generate more semi-random numbers, as the standard formula has a possibility to loop before it has hit the mineNumber
    seedNumber = (seedNumber * 13) + 7;
    columnNumber = seedNumber % 9;

    seedNumber = (seedNumber * 13) + 7;
    rowNumber = seedNumber % 9;

    if (grid[columnNumber][rowNumber] == 9) {
      //If it tries to place a mine on a mine, it tries again with a new number
      i--;
    } else {
      //place a mine on that coordinate
      grid[columnNumber][rowNumber] = 9;
      //Send the coordinates in the appointValues function
      appointValues(columnNumber, rowNumber);
    }
  }
}

//Method to appoint the values around each mine, it checks the 3x3 grid around every mine thats placed
void appointValues(uint8_t column, uint8_t row) {
  //These statements loop nine times to check each spot in a 3x3 grid around a mine
  for (uint8_t r = 0; r < 3; r++) {
    for (uint8_t c = 0; c < 3; c++) {
  //Check statement to see if a selected spot is valid in the array, prevents memory leaks
      if ((column + (1 - c) >= 0 && column + (1 - c) <= 9) && (row + (1 - r) >= 0 && row + (1 - r) <= 9)) {
        //When the selected spot is the mine itself it skips past this statement
          if (grid[column + (1 - c)][row + (1 - r)] != 9) {
            grid[column + (1 - c)][row + (1 - r)]++;
          }
        }
    }
  }
}
*/


uint8_t grid[41];

uint8_t seedNumber = 98;
uint8_t mineNumber = 10;

uint8_t mutatePos(uint8_t position)
{
  position ^= position << 3;
  position ^= position >> 5;
  position ^= position << 1;
  position &= 0xFF;
  return position % 81;
}

void fillField(uint8_t seed)
{
  // veld vullen
  for (uint8_t i = 0; i < 41; i++)
  {
    grid[i] = 0;
  }
  
  // mines plaatsen gebaseerd op seed
  uint8_t pos = seed;
  uint8_t minesPlaced = 0;
    
  while (minesPlaced < mineNumber)
  {
    pos = mutatePos(pos);
        
    // plaats in array berekenen (welke byte)
    uint8_t byteIndex = pos / 2;
    // berekenen welk deel van byte
    bool isHighNibble = (pos % 2 == 0);
        
        
    uint8_t checkValue;

    if (isHighNibble) 
    {
      // controleren of er een mijn is
      checkValue = (grid[byteIndex] & 0xF0); 
            
      if (checkValue == 0)
      {
        // geen mijn --> mijn plaatsen
        grid[byteIndex] |= 0xF0; 
        minesPlaced++;
      }
    } 
    else 
    {
      // controleren of er een mijn is
      checkValue = (grid[byteIndex] & 0x0F); 
            
      if (checkValue == 0)
      {
        // geen mijn --> mijn plaatsen
        grid[byteIndex] |= 0x0F;
        minesPlaced++;
      }
    }
  }
}