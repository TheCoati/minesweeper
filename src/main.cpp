#include <avr/delay.h>
#include <NunchuckControl.h>
#include <SPI.h>

// PD6 --> led
// PD2 --> sensor

void InitializeIO()
{
  initializeIRIO();

  // INT0 falling edge
  EICRA |= (1 << ISC01);
  EICRA &= ~(1 << ISC00);
  EIMSK |= (1 << INT0);

  sei();
}

int main(void)
{
  InitializeIO();
  init();
  Wire.begin();
  Nunchuk.begin(NUNCHUK_ADDRESS);
  tft.begin();
  clearScreen();
  
  InitializeIO();

  firstRender();

  while(true)
  {
    nunchukControl();
  }
  return 0;
}