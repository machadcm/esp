#ifndef _ANALOG_INPUT_
#define _ANALOG_INPUT_

#include "define.h"
#include "device.h"

#define DEVICE_MODE_ON_UP   0x00
#define DEVICE_MODE_ON_DOWN 0x01

class analoginput : public device {
  private:
    uint8_t _pin;     // relay pin number
    uint16_t _value;   // pin value 

  public:
    analoginput(uint8_t id);
    // return device type
    uint16_t type(void);
    // setup device
    uint16_t setup(String);
    // setup from dev structure
    uint16_t setup(device_info *dev);
    // get info config in device struct
    void copy(device_info*);
    // factory reset device
    void factoryreset(void);
    // reset device
    void reset(void);
    // is on
    boolean ison(void);
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
    // get json info config
    String jsoninfo(void);
    // get json config
    String jsonconfig(void);
};

#endif 
