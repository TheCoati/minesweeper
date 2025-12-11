#include "Game.h"

uint8_t grid[41];
uint8_t isVisibleGrid[11];

uint8_t seedNumber = 60; //98 (was ie mee aangeleverd. ik laat m hier ff staan voor de zekerheid, mvg mikaas)
uint8_t mineNumber = 10;

// muteert 8 bit unsigned integer
uint8_t mutateSeed(uint8_t seed)
{
  seed = (seed * 13) + 7;
  return seed;
}

// verandert 1d index in 2d coordinaten
void indexToCoords(uint8_t index, int8_t *row, int8_t *col) {
  *row = index / 9;
  *col = index % 9;
}

// verandert 2d coordinaten in 1d index
uint8_t coordsToIndex(int8_t row, int8_t col) {
  return (uint8_t)(row * 9 + col);
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

      // index omzetten in coordinaten
      int8_t row, col;
      indexToCoords(i, &row, &col);

      // loop door 3x3 grid om bom heen
      for (int8_t r = row - 1; r <= row + 1; r++)
      {
        for (int8_t c = col - 1; c <= col + 1; c++)
        {
          // controleren of we binnen bounds zijn van originele 9x9 grid
          if (r >= 0 && r < 9 && c >= 0 && c < 9)
          {
            // 2d positie omzetten naar 1d positie
            uint8_t neighborPos = coordsToIndex(r, c);
            
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

// controleert of een veld open is
bool isOpenField(uint8_t index)
{
  if (index > 80) return false;
  uint8_t byteIndex = index / 8;
  uint8_t bitIndex = index % 8;
  return (isVisibleGrid[byteIndex] & (0x80 >> bitIndex)) != 0;
}

// opent een veld
void openField(uint8_t index)
{
  if (index > 80) return;

  // controleren of veld al open is
  if (isOpenField(index)) return;

  // locatie van juiste bit berekenen
  uint8_t byteIndex = index / 8;
  uint8_t bitIndex = index % 8;

  // veld openen
  isVisibleGrid[byteIndex] |= (0x80 >> bitIndex);

  // waarde van veld ophalen
  uint8_t fieldValue = getFieldValue(index);

  // als het veld leeg (0) is open dan aanliggende lege velden
  if (fieldValue == 0)
  {
    openEmptyNeighbors(index);
  }

  // als het veld een bom is
  else if (fieldValue == 9)
  {
    // TODO: game over code
  }
}

// opent aanliggende lege velden (gecorrigeerd)
void openEmptyNeighbors(uint8_t index)
{
  // bound controleren
  if (index > 80) return;
  
  // imhoud van veld ophalen
  uint8_t value = getFieldValue(index);
  
  // als veld niet leeg is --> return
  if (value != 0) {
      return; 
  }
  
  // index omzetten in coordinaten
  int8_t row, col;
  indexToCoords(index, &row, &col);

  // door 3x3 veld loopen om huidige vakje
  for (int8_t r = row - 1; r <= row + 1; r++)
  {
    for (int8_t c = col - 1; c <= col + 1; c++)
    {
      // controleren of we binnen bounds van orginele grid zitten
      if (r >= 0 && r < 9 && c >= 0 && c < 9)
      {
        // coordinaten omzetten in index
        uint8_t neighborPos = coordsToIndex(r, c);
        
        // controleren of we niet in het huidige veld zitten
        if (neighborPos != index)
        {
          // veld openen
          openField(neighborPos);
        }
      }
    }
  }
}

