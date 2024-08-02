#include "driver.h"

driver::driver(uint8_t id) : device(id) {
	_mode = DEVICE_MODE_ON_UP;
	_status = STATUS_SETUP;
  _value = LOW;
}

// return if is on
boolean driver::ison(void) {
  return( (_value == HIGH && _mode == DEVICE_MODE_ON_UP ) || (_value == LOW && _mode == DEVICE_MODE_ON_DOWN ) );
}

// return type of device
uint16_t driver::type(void) { return(SENSOR_PORT_DRIVER); }

// {"pin":10, "mode":["onup"|"ondown"], "scheduler":1212, "timer":12}

// parse string and configure device
uint16_t driver::setup(String json) {
	if (!jsonHasKey(json, "pin")) return(ERROR_MISSING_PARAMETER);
	// set pin
	_pin = jsonKey(json, "pin").toInt();
  // set name 
  if (jsonHasKey(json, "name")) jsonKey(json, "name").toCharArray(_name, NAME_LENGHT);
	// set mode
	if (jsonHasKey(json, "mode")) 
		_mode = jsonKey(json, "mode") == "offup" ? DEVICE_MODE_ON_DOWN : DEVICE_MODE_ON_UP;
	// set scheduler
	if (jsonHasKey(json, "scheduler")) _scheduler = jsonKey(json, "scheduler").toInt();
	// set timer
	if (jsonHasKey(json, "timer")) _timer = jsonKey(json, "timer").toInt();
	_status = STATUS_SETUP;
  return(SUCCESS);
}

// configure from dev structure
uint16_t driver::setup(device_info *dev) {
  DEBUG_PRINTLN("load data from device info struct"); 
  _pin = dev->_ports[0];
  _mode = dev->_mode;
  strcpy(_name, dev->_name);
  _scheduler = dev->_scheduler;
  _timer = dev->_timer;
  reset();
  _status = dev->_status;
  if (_status == STATUS_ENABLED ) enable();
    else disable();
  return(SUCCESS);
}

// set info config in device struct
void driver::copy(device_info *dev) {
  DEBUG_PRINTLN("copy data to device info struct"); 
  dev->_type = SENSOR_PORT_DRIVER;
  strcpy(dev->_name, _name);
  dev->_status = _status;
  dev->_scheduler = _scheduler;
  dev->_timer = _timer;
  dev->_mode = _mode;
  dev->_ports[0] = _pin;
};

// factory reset device, clean temporary variables
void driver::factoryreset(void) {
  _status = STATUS_SETUP;
  _scheduler = _timer = _lastTime = 0;
  _value = 0;
  _name[0] = 0;
} 

// reset device
void driver::reset(void) { 
	// set output off
	off();
}

// set device enabled
void driver::enable(void) {
  DEBUG_PRINTLN("Enable pin " + String(_pin) ); 
  // set pin as output
  pinMode(_pin, OUTPUT);
  // enabled
  _status = STATUS_ENABLED;
  // set driver off
  off();
  _lastTime = now();
}    

// set device disabled
void driver::disable(void) { 
  ("Disable pin " + String(_pin) ); 
  // disable
	_status = STATUS_DISABLED;
  // set pin as input
  pinMode(_pin, INPUT);
}

// set device on
uint16_t driver::on(void) {  
	if (_status == STATUS_ENABLED) {
		_value = ( _mode == DEVICE_MODE_ON_UP ? HIGH : LOW);
		digitalWrite(_pin, _value);
    DEBUG_PRINTLN("Set ON pin " + String(_pin) + ", mode " + String(_mode) + ", id " + String(_id) ); 
    _lastTime = now();
		return(SUCCESS); 
	}
	return(ERROR_DEVICE_STATUS);
}
    
// set device off
uint16_t driver::off(void) {

	if (_status == STATUS_ENABLED) {
		_value = ( _mode == DEVICE_MODE_ON_UP ? LOW : HIGH);
		digitalWrite(_pin, _value);
    DEBUG_PRINTLN("Set OFF pin " + String(_pin) + ", mode " + String(_mode) + ", id " + String(_id) ); 
    _lastTime = now();
		return(SUCCESS); 
	}
	return(ERROR_DEVICE_STATUS);
}

// read device
String driver::read(void) { return(String(_value)); }


// read device
uint16_t driver::digitalread(void) { return(_value); }

// write device
void driver::digitalwrite(uint16_t) { };

// read device
uint16_t driver::analogread(void) { return(_value); }
    
// write device
void driver::analogwrite(uint16_t) {};

// execute scheduler and timer
String driver::execute(void) {
  if( checktime() ) {
    if ( ison() ) off();
      else on(); 
    return( json_execute(ison() ? "on" : "off"));
  }
  return("");
}

// get json info config
String driver::jsoninfo(void) {
	String s = deviceinfo(SENSOR_PORT_DRIVER) ;
	return("{" + s + "}");
}

// get json config
String driver::jsonconfig(void) {
  String s = deviceinfo(SENSOR_PORT_DRIVER);
  if ( _status != STATUS_NOT_CONFIGURED && _status != STATUS_ERROR ) {
    s += ",\"pin\":" + String(_pin) + ",\"mode\":" + String(_mode) + ",\"value\":" + (ison() ? "\"on\"" : "\"off\"");
    if ( _scheduler ) s += ",\"scheduler\":" + String(_scheduler);
    if ( _timer ) s += ",\"timer\":" + String(_timer);
  }
  return("{" + s + "}");
}
