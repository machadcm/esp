#ifndef _DHT22_
#define _DHT22_

//  temperature and humidity sensor
//  https://components101.com/modules/soil-moisture-sensor-module
//

#include "device.h"

#include <DHT.h>

class dht22 : public device {
  private:
    uint8_t _pin;        // pin number
    float _humidity;     // sample time 
    float _temperature;  // last read value
    DHT *_dht = NULL;
  protected:
        
  public:
    dht22(uint8_t id);
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
