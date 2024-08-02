#ifndef _FLOWMETER_
#define _FLOWMETER_

// Specification
//    Model:        YF-S201
//    Description:  Water Flow Measurement Sensor with 1-30Liter/min Flow Rate
//    URL:          https://robu.in/product/yf-s201-water-flow-measurement-sensor-with-1-30liter-min-flow-rate-2/


#include "device.h"

class flowmeter : public device {
  private:
    uint8_t _pin;   // relay pin number
    uint8_t _mode;  // On 
    uint16_t _sample;  // sample time 
    double _value;  // last read value
    
  protected:
        
  public:
    flowmeter(uint8_t id);
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
    // is device on
    boolean ison(void){ return(_status == STATUS_ENABLED);};
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
    // write device
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
