#ifndef _PHSENSOR_
#define _PHSENSOR_

// Specification
//    Model:        -
//    Description:  PH sensor meter
//    URL:          -

#include "analogpowerinput.h"

#define ANALOG_POWER_DELAY  200

class phsensor : public analogpowerinput {
  public:
    phsensor(uint8_t id):analogpowerinput(id) {
      // set type on analog power
      _type = SENSOR_PH;
      _poweruptime = ANALOG_POWER_DELAY;
    }
};

#endif
