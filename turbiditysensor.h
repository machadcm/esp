#ifndef _TURBIDITYSENSOR_
#define _TURBIDITYSENSOR_

// Specification
//    Model:        TS-300B
//    Description:  Turbidity sensor 
//    URL:          https://innovatorsguru.com/ts-300b-arduino-turbidity-sensor/

#include "analogpowerinput.h"

#define ANALOG_POWER_DELAY  200

class turbiditysensor : public analogpowerinput {
  public:
    turbiditysensor(uint8_t id):analogpowerinput(id) {
      // set type on analog power
      _type = SENSOR_TURBIDITY;
      _poweruptime = ANALOG_POWER_DELAY;
    }
};

#endif
