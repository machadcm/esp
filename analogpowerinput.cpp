#include "analogpowerinput.h"

uint16_t analogpowerinput::averageRead(void) {
   // read 10 samples, one every 10ms
  uint8_t readbuf[10];
  for ( int i=0; i<10; i++) {
    readbuf[i] = analogRead(_pin);
    delay(10);
  }
  // sort result
  for( int i=0; i<9; i++)
    for( int k=i+1; k<10; k++) 
      if ( readbuf[i] > readbuf[k] ) {
        _value = readbuf[i];
        readbuf[i] = readbuf[k];
        readbuf[k] = _value;
      }
  // average 
  int average = 0;
  for( int i=2; i<8; i++) average += readbuf[i];
  return(average/6);
}

analogpowerinput::analogpowerinput(uint8_t id) : device(id) { 
  _status = STATUS_SETUP;
  factoryreset(); 
}

// return type of device
uint16_t analogpowerinput::type(void) { return(_type); }

// parse string and configure device
uint16_t analogpowerinput::setup(String json) {
  if (!jsonHasKey(json, "pin") || !jsonHasKey(json, "powerpin")) return(ERROR_MISSING_PARAMETER);
  // set pin
  _pin = jsonKey(json, "pin").toInt(); 
  // set powerpin
  _powerpin = jsonKey(json, "powerpin").toInt(); 
  // set name 
  if (jsonHasKey(json, "name")) jsonKey(json, "name").toCharArray(_name, NAME_LENGHT);
  // set scheduler
  if (jsonHasKey(json, "scheduler")) _scheduler = jsonKey(json, "scheduler").toInt();
  _status = STATUS_SETUP;
  return(SUCCESS);
}

// configure from dev structure
uint16_t analogpowerinput::setup(device_info *dev) {
  DBG_PRINTLN("load data from device info struct"); 
  _pin = dev->_ports[0];
  _powerpin = dev->_ports[1];
  strcpy(_name, dev->_name);
  _scheduler = dev->_scheduler;
  _timer = 0;
  _poweruptime = dev->_timer;
  reset();
  _status = dev->_status;
  if (_status == STATUS_ENABLED ) enable();
    else disable();
  return(SUCCESS);
}

// set info config in device struct
void analogpowerinput::copy(device_info *dev) {
  DBG_PRINTLN("copy data to device info struct"); 
  dev->_type = SENSOR_PORT_DRIVER;
  strcpy(dev->_name, _name);
  dev->_status = _status;
  dev->_scheduler = _scheduler;
  dev->_timer = _poweruptime;
  dev->_ports[0] = _pin;
  dev->_ports[1] = _powerpin;  
};

// factory reset device, clean temporary variables
void analogpowerinput::factoryreset(void) {
  if (_status == STATUS_ENABLED) disable();
  _status = STATUS_SETUP;
  _pin = _powerpin = PIN_NOT_CONFIGURED;
  _scheduler = _timer = _lastTime = 0;
  _value = 0;
  _name[0] = 0;
} 

// reset device
void analogpowerinput::reset(void) { _value = 0; }

// set device enabled
void analogpowerinput::enable(void) {
  // set powerpin as output
  pinMode(_powerpin, OUTPUT);
  // set power pin to LOW
  digitalWrite(_powerpin, LOW);
  // set pin as output
  pinMode(_pin, INPUT);
  // enabled
  _status = STATUS_ENABLED;
  _lastTime = now();
}    

// set device disabled
void analogpowerinput::disable(void) {
  _status = STATUS_DISABLED;
  // set power pin to LOW
  digitalWrite(_powerpin, LOW);
  // set powerpin as input
  pinMode(_powerpin, INPUT);
  // set pin as input
  pinMode(_pin, INPUT);
}

// set device on
uint16_t analogpowerinput::on(void) { 
  // enable sensor 
  digitalWrite(_powerpin, HIGH);
  // delay to power up
  delay(_poweruptime);
  return(SUCCESS);
}
    
// set device off
uint16_t analogpowerinput::off(void) { 
  // disable sensor 
  digitalWrite(_powerpin, LOW);
  return(SUCCESS); 
}

// read device
String analogpowerinput::read(void) { return(String(analogread())); }

// read device
uint16_t analogpowerinput::digitalread(void) {return(ERROR_DEVICE_STATUS);}

// write device
void analogpowerinput::digitalwrite(uint16_t) {};

// read device
uint16_t analogpowerinput::analogread(void) {
  // enable sensor 
  digitalWrite(_powerpin, HIGH);
  // delay to power up
  delay(_poweruptime);
  // read device
  _value = averageRead();
  // disable sensor 
  digitalWrite(_powerpin, LOW);
  return(_value);
}
    
// write device
void analogpowerinput::analogwrite(uint16_t) { };

// execute scheduler and timer
String analogpowerinput::execute(void) {
  if( checktime() ) return( json_execute(read()));
  return("");
}

// get json info config
String analogpowerinput::jsoninfo(void) {
  String s = deviceinfo(_type) ;
  return("{" + s + "}");
}

// get json config
String analogpowerinput::jsonconfig(void) {
  String s = deviceinfo(_type);
  if ( _status != STATUS_NOT_CONFIGURED && _status != STATUS_ERROR ) {
    s += ",\"pin\":[" + String(_pin) + "," + String(_powerpin) + "]" + ( _status == STATUS_ENABLED ? ",\"value\":" + read() : "" );
    if ( _scheduler ) s += ",\"scheduler\":" + String(_scheduler);
  }
  return("{" + s + "}");
}
