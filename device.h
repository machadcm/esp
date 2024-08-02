#ifndef _DEVICE_
#define _DEVICE_

#include <Arduino.h>
#include "define.h"

#define DEVICE_MAX_SLEEP_TIME    10000

#define SENSOR_PORT_DRIVER        10
#define SENSOR_ANALOG_INPUT       11
#define SENSOR_DIGITAL_INPUT      12

#define SENSOR_TEMPERATURE_DHT22  13
#define SENSOR_TEMPERATURE_DS18B20  14
#define SENSOR_DISTANCE           15
#define SENSOR_FLOWMETER          16

#define SENSOR_PH                 20
#define SENSOR_TDS                21   
#define SENSOR_TURBIDITY          22   // SEN0189
#define SENSOR_SOIL_MOISTURE      23

#define MOTOR_DRIVER              30




struct device_info {
    uint8_t _pin;
    uint8_t _type;
    char _name[NAME_LENGHT];
    uint8_t _status;
    uint16_t _scheduler;
    uint16_t _timer;
    uint8_t _mode;
    uint16_t _sample;  
    uint8_t _ports[MAX_PORTS];
};


class utils {

  public:
    utils() {};
    // get time in seconds
    unsigned long now(void) {return (millis()/1000);}
    // check if waittime is reached
    boolean iswaittime(unsigned long inittime, unsigned long waittime) { return (now() >= (inittime + waittime) ? true : false);}
    // get waittime
    unsigned long waittime(unsigned long inittime, unsigned long waittime);

    String jsonInfo(String name);
    // check if key exists
    boolean jsonHasKey(String json, String key);
    // return key value
    String jsonKey(String json, String key);
};

// 
class device: public utils  {
  protected:
    uint8_t _id;
    char _name[NAME_LENGHT];
    uint8_t _status;
    uint8_t _updated;
    uint16_t _scheduler;      // if periodic timer is enabled
    uint16_t _timer;          // up time  
    unsigned long _lastTime;  // last activation time

    // get json info config
    String deviceinfo(uint8_t);
    // json execution string
    String json_execute(String value);
    // check time
    boolean checktime(void);
    
  public:
    device(uint8_t id);
    // return device status 
    uint16_t status(void);
		// scheduler calculator
    unsigned long scheduler(void);
    // config updated
    boolean isupdated(void);

    // return device type
    virtual uint16_t type(void) = 0;
    // return device name 
    String name(void) { return(String(_name)); }
		// setup device
    virtual uint16_t setup(String)=0;
    // setup from dev structure
    uint16_t setup(device_info *dev);
    // get info config in device struct
    virtual void copy(device_info*) = 0;    
    // factory reset device
    virtual void factoryreset(void) = 0;
    // reset device
    virtual void reset(void) = 0;
    // is on
    virtual boolean ison(void) { return(_status == STATUS_ENABLED); };
    // is readable
    virtual boolean isreadable(void) = 0;
    // enable device
    virtual void enable(void) = 0;    
    // disable disabled
    virtual void disable(void) = 0;
    // set device on
    virtual uint16_t on(void) = 0;
    // set device off
    virtual uint16_t off(void) = 0;
    // read device
    virtual String read(void) = 0;
    // digital read device
    virtual uint16_t digitalread(void) = 0;
    // analog read device
    virtual uint16_t analogread(void) = 0;
    // write 
    virtual void write(String) = 0;
    // digital write device
    virtual void digitalwrite(uint16_t) = 0;
    // analog write device
    virtual void analogwrite(uint16_t) = 0;
    // execute timer
    virtual String execute(void) = 0;    
    // get json info config
    virtual String jsoninfo(void) = 0;
    // get json detail config
    virtual String jsonconfig(void) = 0;
    // get sensor type name
    String sensorname(uint8_t id);
};


#endif 
