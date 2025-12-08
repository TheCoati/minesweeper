#include <Game.h>

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