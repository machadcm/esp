#include "analoginput.h"

analoginput::analoginput(uint8_t id) : device(id) {
  _status = STATUS_SETUP;
  _value = 0;
}

// return if is on
boolean analoginput::ison(void) { return(false); }

// return type of device
uint16_t analoginput::type(void) { return(SENSOR_ANALOG_INPUT); }

// parse string and configure device
uint16_t analoginput::setup(String json) {
  if (!jsonHasKey(json, "pin")) return(ERROR_MISSING_PARAMETER);
  // set pin
  _pin = jsonKey(json, "pin").toInt();
  // set name 
  if (jsonHasKey(json, "name")) jsonKey(json, "name").toCharArray(_name, NAME_LENGHT);
  // set scheduler
  if (jsonHasKey(json, "scheduler")) _scheduler = jsonKey(json, "scheduler").toInt();
  _status = STATUS_SETUP;
  return(SUCCESS);
}

// configure from dev structure
uint16_t analoginput::setup(device_info *dev) {
  DBG_PRINTLN("load data from device info struct"); 
  _pin = dev->_ports[0];
  strcpy(_name, dev->_name);
  _scheduler = dev->_scheduler;
  _timer = 0;
  reset();
  _status = dev->_status;
  if (_status == STATUS_ENABLED ) enable();
    else disable();
  return(SUCCESS);
}

// set info config in device struct
void analoginput::copy(device_info *dev) {
  DBG_PRINTLN("copy data to device info struct"); 
  dev->_type = SENSOR_ANALOG_INPUT;
  strcpy(dev->_name, _name);
  dev->_status = _status;
  dev->_scheduler = _scheduler;
  dev->_timer = 0;
  dev->_ports[0] = _pin;
};

// factory reset device, clean temporary variables
void analoginput::factoryreset(void) {
  _status = STATUS_SETUP;
  _scheduler = _timer = _lastTime = 0;
  _value = 0;
  _name[0] = 0;
} 

// reset device
void analoginput::reset(void) { _value = 0; }

// set device enabled
void analoginput::enable(void) {
  // set pin as input
  pinMode(_pin, INPUT);
  // enabled
  _status = STATUS_ENABLED;
  _lastTime = now();
}    

// set device disabled
void analoginput::disable(void) {
  _status = STATUS_DISABLED;
  // set pin as input
  pinMode(_pin, INPUT);
}

// set device on
uint16_t analoginput::on(void) {return(SUCCESS);}
    
// set device off
uint16_t analoginput::off(void) {return(SUCCESS);}

// read device
String analoginput::read(void) { return(String(analogread())); }

// read device
uint16_t analoginput::digitalread(void) { 
  _value = digitalRead(_pin);
  return(_value);
}

// read device
uint16_t analoginput::analogread(void) {
  _value = analogRead(_pin);
  return(_value);
}
    
// execute scheduler and timer
String analoginput::execute(void) {
  if( checktime() ) return( json_execute(read()));
  return("");
}

// get json info config
String analoginput::jsoninfo(void) {
  String s = deviceinfo(SENSOR_ANALOG_INPUT) ;
  return("{" + s + "}");
}

// get json config
String analoginput::jsonconfig(void) {
  String s = deviceinfo(SENSOR_ANALOG_INPUT);
  if ( _status != STATUS_NOT_CONFIGURED && _status != STATUS_ERROR ) {
    s += ",\"pin\":" + String(_pin) + ( _status == STATUS_ENABLED ? ",\"value\":" + read() : "" );
    if ( _scheduler ) s += ",\"scheduler\":" + String(_scheduler);
  }
  return("{" + s + "}");
}
