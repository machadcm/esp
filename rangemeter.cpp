#include "rangemeter.h"

//
rangemeter::rangemeter(uint8_t id):device(id) {
  _status = STATUS_SETUP;
  _sample = 1000;
  _value = 0;
}

// return type of device
uint16_t rangemeter::type(void) { return(SENSOR_DISTANCE); }

// {"pin":10, "sample":1002, "scheduler":1212, "timer":12}

// parse string and configure device
uint16_t rangemeter::setup(String json) {
  if (!jsonHasKey(json, "pin")) return(ERROR_MISSING_PARAMETER);
  // set pin
  _pin = jsonKey(json, "pin").toInt();
  // set name 
  if (jsonHasKey(json, "name")) jsonKey(json, "name").toCharArray(_name, NAME_LENGHT);
  // set sample time
  if (jsonHasKey(json, "sample")) _sample = jsonKey(json, "sample").toInt();
  // set scheduler
  if (jsonHasKey(json, "scheduler")) _scheduler = jsonKey(json, "scheduler").toInt();
  // set timer
  if (jsonHasKey(json, "timer")) _scheduler = jsonKey(json, "timer").toInt();
  _status = STATUS_SETUP;
  return(SUCCESS);
}

// configure from dev structure
uint16_t rangemeter::setup(device_info *dev) {
  _pin = dev->_ports[0];
  _mode = dev->_mode;
  _sample = dev->_sample;
  _scheduler = dev->_scheduler;
  _timer = dev->_timer;
  _status = dev->_status;
  strcpy(dev->_name, _name);
  return(SUCCESS);
}

// factory reset device, clean temporary variables
void rangemeter::factoryreset(void) {
  _status = STATUS_SETUP;
  _sample = 1000;
  _scheduler = _timer = _lastTime = 0;
  _name[0] = 0;
  if ( _status == STATUS_ENABLED) detachInterrupt(digitalPinToInterrupt(_pin));
} 

// reset device, clean temporary variables
void rangemeter::reset(void) {}; // do nothing 

// set device enabled
void rangemeter::enable(void) {
  // set pin as output
  pinMode(_pin, INPUT);
  _status = STATUS_ENABLED;
}    

// set device disabled
void rangemeter::disable(void) {
  pinMode(_pin, OUTPUT);
  _status = STATUS_DISABLED;
}

// set device on
uint16_t rangemeter::on(void) {
  return(ERROR_NOT_SUPPORTED);
}
    
// set device off
uint16_t rangemeter::off(void) {
  return(ERROR_NOT_SUPPORTED);
}

// read device
String rangemeter::read(void) {
  return(String(_value));
}
    
// read device
uint16_t rangemeter::digitalread(void) {
  return(_value ? 1 : 0);
}

// write device
uint16_t rangemeter::digitalwrite(void) {
  return(ERROR_NOT_SUPPORTED);
}

// read device
uint16_t rangemeter::analogread(void) {
  return((uint16_t)_value);
}
    
// write device
uint16_t rangemeter::analogwrite(void) {
  return(ERROR_NOT_SUPPORTED);
}

// execute scheduler and timer
String rangemeter::execute(void) {
  // wait for sample time
  delay (_sample);    
  // disable interrupts
  cli();            // Disable interrupts
  return(""); 
}

// set info config in device struct
void rangemeter::copy(device_info *dev) {
  dev->_type = SENSOR_DISTANCE;
  strcpy(_name, dev->_name);
  dev->_status =_status;
  dev->_scheduler = _scheduler;
  dev->_timer = _timer;
  dev->_mode = _mode;
  dev->_sample = _sample;  
  dev->_ports[0] = _pin;  
}

// get json info config
String rangemeter::jsoninfo(void) {
  String s = deviceinfo(SENSOR_DISTANCE) ;
  return("{" + s + "}");
}

// get json config
String rangemeter::jsonconfig(void) {
  String s = jsoninfo() + ",\"status\":" + String(_status) + ",\"pin\":" + String(_pin) + ",\"mode\":" + String(_mode) + 
             ",\"sample\":" + String(_sample) + ",\"value\":" + String(_value);
  if ( _scheduler ) s += ",\"scheduler\":" + String(_scheduler);
  if ( _timer ) s += ",\"timer\":" + String(_timer);

  return("{" + s + "}");
}

  
