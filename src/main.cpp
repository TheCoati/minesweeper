#include <avr/delay.h>
#include <NunchuckControl.h>
#include <SPI.h>

// PD6 --> led
// PD2 --> sensor

int main(void)
{
  init();
  Wire.begin();
  Nunchuk.begin(NUNCHUK_ADDRESS);
  tft.begin();
  clearScreen();

  while(true)
  {
    nunchukControl();
  }
  return 0;
}