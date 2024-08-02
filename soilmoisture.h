#ifndef _SOILMOISTURE_
#define _SOILMOISTURE_

#include "analogpowerinput.h"

//  soil moisture specification
//  https://components101.com/modules/soil-moisture-sensor-module
//

#define ANALOG_POWER_DELAY  200

class soilmoisture : public analogpowerinput {
  public:
    soilmoisture(uint8_t id):analogpowerinput(id) {
      // set type on analog power
      _type = SENSOR_SOIL_MOISTURE;
      _poweruptime = ANALOG_POWER_DELAY;
    }
};

#endif 
