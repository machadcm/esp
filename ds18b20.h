#ifndef _DS18B20_
#define _DS18B20_


//  temperature sensor
//  https://cdn.sparkfun.com/datasheets/Sensors/Temp/DS18B20.pdf
//

#include "device.h"

#include <OneWire.h>
#include <DallasTemperature.h>

class ds18b20 : public device {
  private:
    uint8_t _pin;        // pin number
    float _temperature;  // last read value
    OneWire *_onewire = NULL;
    DallasTemperature *_ds18b20 = NULL;

  protected:
        
  public:
    ds18b20(uint8_t id);
    // return device type
    uint16_t type(void);
    // setup device
    uint16_t setup(String);
    // setup from dev structure
    uint16_t setup(device_info *dev);
    // factory reset device
    void factoryreset(void);
    // reset device
    void reset(void);
    // is readable
    boolean isreadable(void) { return(true); }
    // set device enabled
    void enable(void);    
    // set device disabled
    void disable(void);
    // set device on
    uint16_t on(void);
    // set device off
    uint16_t off(void);
    // read device
    String read(void);
    // read device
    uint16_t digitalread(void);
    // read device
    uint16_t analogread(void);
    // write 
    void write(String){};
    // write device
    void digitalwrite(uint16_t){};
    // write device
    void analogwrite(uint16_t){};
    // execute timer
    String execute(void);
    // get info config in device struct
    void copy(device_info*);

    // get json info config
    String jsoninfo(void);
    // get json detail config
    String jsonconfig(void);
};


#endif 
