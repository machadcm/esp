#include "flowmeter.h"

// global variable
volatile int pulseCount;

// interrupt routine
void pulseCounter() { pulseCount++; } 


flowmeter::flowmeter(uint8_t id):device(id) {
  _status = STATUS_SETUP;
  _sample = 1000;
  _value = 0;
}

// return type of device
uint16_t flowmeter::type(void) { return(SENSOR_FLOWMETER); }

// {"pin":10, "sample":1002, "scheduler":1212, "timer":12}

// parse string and configure device
uint16_t flowmeter::setup(String json) {
  if (!jsonHasKey(json, "pin")) return(ERROR_MISSING_PARAMETER);
  // set pin
  _pin = jsonKey(json, "pin").toInt();
  // set name 
  if (jsonHasKey(json, "name")) jsonKey(json, "name").toCharArray(_name, NAME_LENGHT);
  // set sample time
  if (jsonHasKey(json, "sample")) _sample = jsonKey(json, "sample").toInt();
  // set scheduler
  if (jsonHasKey(json, "scheduler")) _scheduler = jsonKey(json, "scheduler").toInt();
  _status = STATUS_SETUP;
  return(SUCCESS);
}

// configure from dev structure
uint16_t flowmeter::setup(device_info *dev) {
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
void flowmeter::factoryreset(void) {
  _status = STATUS_SETUP;
  _sample = 1000;
  _scheduler = _timer = _lastTime = 0;
  _name[0] = 0;
  //if ( _status == STATUS_ENABLED) detachInterrupt(digitalPinToInterrupt(_pin));
} 

// reset device, clean temporary variables
void flowmeter::reset(void) {}; // do nothing 

// set device enabled
void flowmeter::enable(void) {
  // set pin as output
  pinMode(_pin, INPUT);
  _status = STATUS_ENABLED;
  // attach interrupt
  //attachInterrupt(digitalPinToInterrupt(_pin), pulseCounter, RISING);
}    

// set device disabled
void flowmeter::disable(void) {
  pinMode(_pin, OUTPUT);
  _status = STATUS_DISABLED;
  //detachInterrupt(digitalPinToInterrupt(_pin));
}

// set device on
uint16_t flowmeter::on(void) {
  return(ERROR_NOT_SUPPORTED);
}
    
// set device off
uint16_t flowmeter::off(void) {
  return(ERROR_NOT_SUPPORTED);
}

// read device
String flowmeter::read(void) {
  execute();
  return(String(_value));
}
    
// read device
uint16_t flowmeter::digitalread(void) {
  return(_value ? 1 : 0);
}

// read device
uint16_t flowmeter::analogread(void) {
  return((uint16_t)_value);
}
    

// execute scheduler and timer
String flowmeter::execute(void) {
  // set pulse counters to zero
  pulseCount = _value = 0;   
  if (_status == STATUS_ENABLED && _sample > 0 ) {
    // attach interrupt to pin
    attachInterrupt(digitalPinToInterrupt(_pin), pulseCounter, RISING);
    //sei();            // Enable interrupts
    // wait for sample time
    delay (_sample);    
    // detach interrupt from pin
    detachInterrupt(digitalPinToInterrupt(_pin));
    //cli();            // Disable interrupts
    // calculate flow,  (Pulse frequency x 60) / 7.5Q, = flow rate in L/hour
    _value = ( ( pulseCount * 1000 ) / _sample ) * 60 / 7.5; 
  }
  return "";
}

// set info config in device struct
void flowmeter::copy(device_info *dev) {
  dev->_type = SENSOR_FLOWMETER;
  strcpy(_name, dev->_name);
  dev->_status =_status;
  dev->_scheduler = _scheduler;
  dev->_timer = _timer;
  dev->_mode = _mode;
  dev->_sample = _sample;  
  dev->_ports[0] = _pin;  
}


// get json info config
String flowmeter::jsoninfo(void) {
  return( deviceinfo(SENSOR_FLOWMETER) );
}

// get json config
String flowmeter::jsonconfig(void) {
  String s = jsoninfo() + ",\"pin\":" + String(_pin) + ",\"sample\":" + String(_sample) + ",\"value\":" + String(_value);
  if ( _scheduler ) s += ",\"scheduler\":" + String(_scheduler);
  return("{" + s + "}");
}

  
