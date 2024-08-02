#ifndef _RANGEMETER_
#define _RANGEMETER_

// Specification
//    Model:        JSN-SR04T
//    Description:  Waterproof Ultrasonic Range Finder
//    URL:          https://protosupplies.com/product/jsn-sr04t-v3-0-waterproof-ultrasonic-range-finder/


#include "device.h"

class rangemeter : public device {
  private:
    uint8_t _pin;   // relay pin number
    uint8_t _mode;  // On 
    uint8_t _sample;  // sample time 
    double _value;  // last read value
    
  protected:
        
  public:
    rangemeter(uint8_t id);
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
    // write device
    uint16_t digitalwrite(void);
    // read device
    uint16_t analogread(void);
    // write device
    uint16_t analogwrite(void);
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
