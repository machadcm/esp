#include "ds18b20.h"

ds18b20::ds18b20(uint8_t id):device(id) { factoryreset(); }

// return type of device
uint16_t ds18b20::type(void) { return(SENSOR_TEMPERATURE_DS18B20); }

// {"pin":10, "sample":1002, "scheduler":1212, "timer":12}

// parse string and configure device
uint16_t ds18b20::setup(String json) {
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
uint16_t ds18b20::setup(device_info *dev) {
  _pin = dev->_ports[0];
  strcpy(_name, dev->_name);
  _scheduler = dev->_scheduler;
  _timer = dev->_timer;
  reset();
  _status = dev->_status;
  if (_status == STATUS_ENABLED ) enable();
    else disable();
  return(SUCCESS);
}

// factory reset device, clean temporary variables
void ds18b20::factoryreset(void) {
  _status = STATUS_SETUP;
  _scheduler = _timer = _lastTime = 0;
  _name[0] = 0;
} 

// reset device, clean temporary variables
void ds18b20::reset(void) {}; // do nothing 

// set device enabled
void ds18b20::enable(void) {
  if ( _onewire == NULL ) {
    _onewire = new OneWire(_pin);
    _ds18b20 = new DallasTemperature(_onewire);
    _status = STATUS_ENABLED;
  }
}    

// set device disabled
void ds18b20::disable(void) {
  if ( _ds18b20 != NULL || _onewire != NULL ) {
    delete _ds18b20;
    delete _onewire;
    _status = STATUS_DISABLED;
  }
}

// set port on
uint16_t ds18b20::on(void) { return(ERROR_NOT_SUPPORTED); }
    
// set port off
uint16_t ds18b20::off(void) { return(ERROR_NOT_SUPPORTED); }

// read device
String ds18b20::read(void) { 
  _temperature = 0;
  if ( _status == STATUS_ENABLED ) {
    _ds18b20->requestTemperatures();
    _temperature = _ds18b20->getTempCByIndex(0);
  }
  return( String(_temperature) ); 
}
    
// read device
uint16_t ds18b20::digitalread(void) { return(0); }

// read device
uint16_t ds18b20::analogread(void) { return(0); }
    
// execute scheduler and timer
String ds18b20::execute(void) { return( checktime() ? json_execute(read()) : "" ); }

// set info config in device struct
void ds18b20::copy(device_info *dev) {
  dev->_type = SENSOR_TEMPERATURE_DS18B20;
  strcpy(_name, dev->_name);
  dev->_status =_status;
  dev->_scheduler = _scheduler;
  dev->_timer = _timer;
  dev->_mode = 0;
  dev->_sample = 0;
  dev->_ports[0] = _pin;  
}

// get json info config
String ds18b20::jsoninfo(void) { return(deviceinfo(SENSOR_TEMPERATURE_DS18B20)); }

// get json config
String ds18b20::jsonconfig(void) {
  String s = jsoninfo() + ",\"status\":" + String(_status) + ",\"pin\":" + String(_pin) +  ",\"value\":" + read() ;
  if ( _scheduler ) s += ",\"scheduler\":" + String(_scheduler);
  if ( _timer ) s += ",\"timer\":" + String(_timer);
  return("{" + s + "}");
}

  
