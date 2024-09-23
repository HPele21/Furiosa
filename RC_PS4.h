#include <Adafruit_NeoPixel.h>
#include <PS4Controller.h>
#include <DRV8833.h>
#include "Pixel.h"


extern DRV8833 motor;
extern Adafruit_NeoPixel pixels;


void rc_ps4() {

  if(!PS4.isConnected()) {

    motor.stop();
    px_fill(100, 100, 0);
    pixels.clear();

  }else if(PS4.isConnected()) {

    PS4.setLed(0, 255, 0);
    PS4.sendToController();
    px_fill(0, 100, 0);

    int RS = map(PS4.RStickX(), -127, 127, -950, 950);

    int R2 = map(PS4.R2Value(), 0, 255, 0, 1023);
    int L2 = map(PS4.L2Value(), 0, 255, 0, 1023);

    int move = constrain((R2 +L2), -1023, 1023);

    int vel_esq = constrain(RS + (R2 - L2), -1023, 1023);
    int vel_dir = constrain(-RS + (R2 - L2), -1023, 1023);

    motor.move(vel_esq, vel_dir);
  }
}

