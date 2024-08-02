#ifndef _STATUS_
#define _STATUS_

#include <Arduino.h>

/*
  Starting will remain on
  Connecting will blink every 300ms 
  Connected will blink every 2 seconds
*/

enum systemStatus { Starting, Connecting, Connected };

#define BLINK_STARTING    0
#define BLINK_CONNECTING  500
#define BLINK_CONNECTED   3000
#define BLINK_TIME        100

class status {
  private:
    uint8_t _pin; 
    uint8_t _pinstatus; 
    unsigned long _timer;

    void changePin(void);
  public:
    // constructor
    status(uint8_t pin);
    // destructor
    ~status();
    // init
    void begin(void);
    // blink led
    void blink(systemStatus status);
};

#endif

