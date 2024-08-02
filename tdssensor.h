#ifndef _TDSSENSOR_
#define _TDSSENSOR_

// Specification
//    Model:        SEN0244
//    Description:  TDS (total disonved solids) sensor 
//    URL:          https://wiki.dfrobot.com/Gravity__Analog_TDS_Sensor___Meter_For_Arduino_SKU__SEN0244

#include "analogpowerinput.h"

#define ANALOG_POWER_DELAY  200

class tdssensor : public analogpowerinput {
  public:
    tdssensor(uint8_t id):analogpowerinput(id) {  
      // set type on analog power
      _type = SENSOR_TDS;
      _poweruptime = ANALOG_POWER_DELAY;
    };
};

#endif
