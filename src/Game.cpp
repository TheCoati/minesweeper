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

uint8_t mutateSeed(uint8_t seed)
{
  seed = (seed * 13) + 7;
  return seed;
}

// waarde van veld ophalen (index begint bij 0)
// returned 255 als de index out of bound is
uint8_t getFieldValue(uint8_t index)
{
  if (index > 80) return 255;
  bool isHighNibble = (index % 2 == 0);
  return (isHighNibble) ? (grid[index/2] & 0xF0) >> 4 : grid[index/2] & 0x0F;
}

void incrementFieldValue(uint8_t index)
{
  // bounds controleren
  if (index > 80) return;

  // huidige waarde van veld ophalen
  uint8_t fieldValue = getFieldValue(index);

  // 8 of hoger --> kan niet verhogen (bom of max waarde)
  if (fieldValue >= 8) return;

  // berekenen welk deel van byte (high/low gedeelte)
  bool isHighNibble = (index % 2 == 0);

  // lokale waarde van veld verhogen met 1
  fieldValue++;

  // huidige waarde van byte ophalen
  uint8_t byteValue = grid[index/2];

  // lokaal opgeslagen waarde eroverheen maskeren
  byteValue &= (isHighNibble) ? 0x0F : 0xF0;
  byteValue |= (isHighNibble) ? fieldValue << 4 : fieldValue;

  // lokale waarde naar grid array schrijven
  grid[index/2] = byteValue;
}

void fillField(uint8_t seed)
{
  // veld vullen
  for (uint8_t i = 0; i < 41; i++)
  {
    grid[i] = 0;
  }
  
  // mines plaatsen gebaseerd op seed
  uint8_t minesPlaced = 0;
  
  while (minesPlaced < mineNumber)
  {
    seed = mutateSeed(seed);
    uint8_t pos = seed % 81;
    
    // plaats in array berekenen (welke byte)
    uint8_t byteIndex = pos / 2;
    // berekenen welk deel van byte (high/low gedeelte)
    bool isHighNibble = (pos % 2 == 0);
    
    uint8_t checkValue;

    if (isHighNibble) 
    {
      // controleren of er een mijn is
      checkValue = (grid[byteIndex] & 0x90); 
      
      if (checkValue == 0)
      {
        // geen mijn --> mijn plaatsen
        grid[byteIndex] |= 0x90; 
        minesPlaced++;
      }
    } 
    else 
    {
      // controleren of er een mijn is
      checkValue = (grid[byteIndex] & 0x09); 
      
      if (checkValue == 0)
      {
        // geen mijn --> mijn plaatsen 
        grid[byteIndex] |= 0x09;
        minesPlaced++;
      }
    }
  }

  // velden rondom bommen ophogen
  for (uint8_t i = 0; i < 81; i++)
  {
    // controleren of bom aanwezig is in veld
    if (getFieldValue(i) == 9)
    {
      // 1d positie omzetten in 2d positie
      int8_t row = i / 9;
      int8_t col = i % 9;

      // loop door 3x3 grid om bom heen
      for (int8_t r = row - 1; r <= row + 1; r++)
      {
        for (int8_t c = col - 1; c <= col + 1; c++)
        {
          // controleren of we binnen bounds zijn van originele 9x9 grid
          if (r >= 0 && r < 9 && c >= 0 && c < 9)
          {
            // 2d positie omzetten naar 1d positie
            uint8_t neighborPos = r * 9 + c;
            
            // controleren of we niet op de bom zelf zitten
            if (neighborPos != i)
            {
              incrementFieldValue(neighborPos);
            }
          }
        }
      }
    }
  }

}