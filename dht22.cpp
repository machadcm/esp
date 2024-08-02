#include "dht22.h"

dht22::dht22(uint8_t id):device(id) { factoryreset(); }

// return type of device
uint16_t dht22::type(void) { return(SENSOR_TEMPERATURE_DHT22); }

// {"pin":10, "sample":1002, "scheduler":1212, "timer":12}

// parse string and configure device
uint16_t dht22::setup(String json) {
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
uint16_t dht22::setup(device_info *dev) {
  DEBUG_PRINTLN("load data from device info struct"); 
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
void dht22::factoryreset(void) {
  _status = STATUS_SETUP;
  _scheduler = _timer = _lastTime = 0;
  _name[0] = 0;
} 

// reset device, clean temporary variables
void dht22::reset(void) {}; // do nothing 

// set device enabled
void dht22::enable(void) {
  if ( _dht == NULL ) {
    _dht = new DHT(_pin, DHT22);
    _status = STATUS_ENABLED;
  }
}    

// set device disabled
void dht22::disable(void) {
  if ( _dht != NULL ) {
    delete _dht;
    _status = STATUS_DISABLED;
  }
}

// set port on
uint16_t dht22::on(void) { return(ERROR_NOT_SUPPORTED); }
    
// set port off
uint16_t dht22::off(void) { return(ERROR_NOT_SUPPORTED); }

// read device
String dht22::read(void) { 
  _temperature = 0;
  _humidity = 0;
  if ( _status == STATUS_ENABLED ) {
    _temperature = _dht->readTemperature();
    _humidity = _dht->readHumidity();
  }
  return( '[' + String(_temperature) + ',' + String(_humidity) + ']'); 
}
    
// read device
uint16_t dht22::digitalread(void) { return(0); }

// read device
uint16_t dht22::analogread(void) { return(0); }
    
// execute scheduler and timer
String dht22::execute(void) { return( checktime() ? json_execute(read()) : "" ); }

// set info config in device struct
void dht22::copy(device_info *dev) {
  dev->_type = SENSOR_FLOWMETER;
  strcpy(_name, dev->_name);
  dev->_status =_status;
  dev->_scheduler = _scheduler;
  dev->_timer = _timer;
  dev->_mode = 0;
  dev->_sample = 0;
  dev->_ports[0] = _pin;  
}

// get json info config
String dht22::jsoninfo(void) { return( deviceinfo(SENSOR_TEMPERATURE_DHT22) ); }

// get json config
String dht22::jsonconfig(void) {
  String s = jsoninfo() + ",\"status\":" + String(_status) + ",\"pin\":" + String(_pin) +  ",\"value\":" + read() ;
  if ( _scheduler ) s += ",\"scheduler\":" + String(_scheduler);
  if ( _timer ) s += ",\"timer\":" + String(_timer);
  return("{" + s + "}");
}

  
