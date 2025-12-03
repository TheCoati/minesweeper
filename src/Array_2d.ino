bool grid[9][9];

//This number is sent by the protocol on start up
uint8_t seedNumber = 2;

uint8_t columnNumber = 0;
uint8_t rowNumber = 0;

//Formule voor berekenen getal: (Seed * 13) + 7 % 10

void fillField() {

  for (uint8_t i = 0; i < 10; i++) {
    seedNumber = (seedNumber * 13) + 7;
    columnNumber = seedNumber % 10;

    seedNumber = (seedNumber * 13) + 7;
    rowNumber = seedNumber % 10;
    //Serial.println(i);
    if(grid[columnNumber][rowNumber] == true){
      i--;
    } else {
      grid[columnNumber][rowNumber] = true;
    }
    
  }
  
}
void setup() {
  Serial.begin(9600);

  fillField();
}

void loop() {
  // Print the grid
  for (uint8_t r = 0; r < 9; r++) {
    for (uint8_t c = 0; c < 9; c++) {
      Serial.print(grid[r][c]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}
