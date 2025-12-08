#ifndef NUNCHUCKCONTROL_H_
#define NUNCHUCKCONTROL_H_

#include <LED.h>
#include <Screen.h>
#include <Nunchuk.h>

#define NUNCHUK_ADDRESS 0x52

extern bool zPressed;

void nunchukControl();

#endif
