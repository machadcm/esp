#include "status.h"
#include "define.h"

// constructor
status::status(uint8_t pin) {
  // set pin 
  _pin = pin;
}
 
// destructor
status::~status() {pinMode(_pin, INPUT);}

// begin
void status::begin(void) {
  // set pin status 0
  _pinstatus = LOW;
  // set por output
  pinMode(_pin, OUTPUT);
  // set pin to low
  digitalWrite(_pin, _pinstatus);
  // set timmer
  _timer = millis();
}

// change pin
void status::changePin(void) {
    // invert status 
  _pinstatus = ( _pinstatus == HIGH ? LOW : HIGH);
  // set pin 
  digitalWrite(_pin, _pinstatus);  
  // set timmer
  _timer = millis();
}

// blink led
void status::blink(systemStatus s) {
  if (s == Starting) changePin();
  else {
    // pin high blink every 100ms
    if ( _pinstatus == HIGH ) {
      if ( millis() >= (_timer + BLINK_TIME)  ) changePin();
    } else {
      if ( s == Connecting && millis() >= (_timer + BLINK_CONNECTING) ) changePin();
        else if (s == Connected && millis() >= (_timer + BLINK_CONNECTED) ) changePin();
    }
  }
}
