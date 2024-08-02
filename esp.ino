#include <Arduino.h>
#include "define.h"

#include "wifi.h"
#include "controller.h"


// wifi needs to be declared as global instance
wifi _globalWifi(HTTP_PORT);

// declare controller
controller _controller(&_globalWifi);

// setup function
void setup() {
  DEBUG_INIT(115200);
  delay(1000);
  // print debug 
  DEBUG_PRINTLN("\n************************\n");
  // put your setup code here, to run once:
  DEBUG_PRINTLN("Starting system");
  _controller.setup();
}

// loop function
void loop() {
  // put your main code here, to run repeatedly:
  _controller.loop();
}
