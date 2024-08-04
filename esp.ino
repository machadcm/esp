#include <Arduino.h>
#include "define.h"

#include "esp-wifi.h"
#include "controller.h"


// wifi needs to be declared as global instance
espwifi _globalWifi(HTTP_PORT);

// declare controller
controller _controller(&_globalWifi);

// setup function
void setup() {
  DBG_INIT(115200);
  delay(1000);
  // print debug 
  DBG_PRINTLN("\n************************\n");
  // put your setup code here, to run once:
  DBG_PRINTLN("Starting system");
  _controller.setup();
}

// loop function
void loop() {
  // put your main code here, to run repeatedly:
  _controller.loop();
}
