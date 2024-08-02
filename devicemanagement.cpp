#include "devicemanagement.h"

#include "driver.h"
#include "flowmeter.h"
#include "analoginput.h"
#include "digitalinput.h"
#include "dht22.h"
#include "ds18b20.h"
#include "tdssensor.h"
#include "turbiditysensor.h"

// get chip id
String devicemanagement::getChipId(void){
  #if ( DEVICE_TYPE == DEVICE_ESP8266 ) 
    return(String(ESP.getChipId()));
  #endif
  #if ( DEVICE_TYPE == DEVICE_ESP32 ) 
    return(String(ESP.getEfuseMac() & 0xFF));
  #endif
}    

int devicemanagement::buffersize(void) {
  uint8_t i=0;
  for (; _buffer[i] != ""; i++);
  return(i);
}

// append data to buffer
String devicemanagement::pull(void) {
  // if buffer empty 
  if ( !buffersize() ) return("");
  String s = _buffer[0];
  // remove first entry
  for (int i = 0; i<MAX_BUFFER-1; i++) _buffer[i] = _buffer[i+1];
  _buffer[MAX_BUFFER-1] = ""; 
  return(s);
}

// append data to buffer
void devicemanagement::push(String t, String d) {
   if (_target[0]) {
    // if buffer full remove first entry
    if ( buffersize() == MAX_BUFFER ) pull();
    // append as last entry
    _buffer[buffersize()] = t + d;
   }
}

// send data to server
void devicemanagement::send_data(void) {
  // loop array to send data
  for( String s = pull(); s != ""; s = pull() ) {
    String target = s.substring(0, s.indexOf('{'));
    String data = s.substring(s.indexOf('{'));
    _wifi->send(target, data); 
  }
}


devicemanagement::devicemanagement(wifi *w) {
  _wifi = w; 
  // initialize device structure
  for( int i=0; i< MAX_DEVICES; i++) _devices[i] = NULL;
  // initialize tx buffer
  for (int i=0; i<MAX_BUFFER; i++) _buffer[i] = "";
  factoryreset();
}

// register device
void devicemanagement::register_device(void) {
  push(String(_target) + "/api/register/device",
        "{\"uptime\":" + String(now()) + ",\"serial\":\"" + getChipId() + "\",\"name\":\"" + String(_name) + "\",\"ip\":\"" +  _wifi->ip() + "\"}"); 
  reset();
}

// register sensor
void devicemanagement::register_sensor(uint8_t id)  {
    push(String(_target) + "/api/register/sensor", 
        "{\"uptime\":" + String(now()) + ",\"serial\":\"" + getChipId() + 
        "\",\"sensorid\":" + String(id) + ",\"sensortype\":" + String(_devices[id]->type()) + ",\"name\":\"" + _devices[id]->name() + "\"}"); 
}

// post data
void devicemanagement::post_data(String data)  {
  push(String(_target) + "/api/data", 
        "{\"uptime\":" + String(now()) + ",\"serial\":\"" + getChipId() + "\",\"data\":[" + data + "]}");
}

// setup controller
void devicemanagement::setup(void) {
}

// reset device
void devicemanagement::reset(void) {
  _control = _updated = _lasttime = 0;
  _waittime = (DEVICE_MAX_SLEEP_TIME + 1) * 1000; 
  // force all pins to input
  //for( int i=0; i < MAX_AVAILABLE_PORTS; i++) pinMode(boardSupportedPins[i], INPUT);

}

// reset device
void devicemanagement::factoryreset(void) {
  // delete and initialize devices
  for( int i=0; i< MAX_DEVICES; i++) del(i);
  // initialize state variables  
  _name[0] = 0;
  _target[0] = 0;
  reset();
}

// register device
void devicemanagement::registerdevice(void) {
  register_device();
  for( int i=0; i< MAX_DEVICES; i++ )  
    if(_devices[i] != NULL) register_sensor(i);
}

// name
const char* devicemanagement::name(void) { return(_name); }
    
void devicemanagement::name(const char *name) {
  strcpy(_name, name);
  _updated++;
}

void devicemanagement::name(String name) {
   name.trim();
   name.toCharArray(_name, URL_LENGHT);
  _updated++;
}

// target url
const char* devicemanagement::target(void) { return(_target); }

void devicemanagement::target(const char *target) {
  strcpy(_target, target);
  _updated++;
}

void devicemanagement::target(String target) {
  target.trim();
  target.toCharArray(_target, URL_LENGHT);
  _updated++;
}

// add device
int devicemanagement::add(uint8_t type) {
  // find first free driver
  int port = 0;
  for(; port != NULL && port < MAX_DEVICES; port++ );
  // if no port left return error
  if (port == MAX_DEVICES) return(-1);
  // create device
  switch(type) {
    case SENSOR_PORT_DRIVER:
      DEBUG_SERIAL("devicemanagement::add driver");
      _devices[port] = new driver(port);
      break;
    case SENSOR_ANALOG_INPUT:
      DEBUG_SERIAL("devicemanagement::add analog input");
      _devices[port] = new analoginput(port);
      break;
    case SENSOR_DIGITAL_INPUT:
      DEBUG_SERIAL("devicemanagement::add digital input");
      _devices[port] = new digitalinput(port);
      break;
    case SENSOR_TEMPERATURE_DHT22:
      DEBUG_SERIAL("devicemanagement::add dht22");
      _devices[port] = new dht22(port);
      break;
    case SENSOR_TEMPERATURE_DS18B20:
      DEBUG_SERIAL("devicemanagement::add ds18b20");
      _devices[port] = new ds18b20(port);
      break;
    case SENSOR_DISTANCE:
      DEBUG_SERIAL("devicemanagement::add distance sensor");
      break;
    case SENSOR_FLOWMETER:
      DEBUG_SERIAL("devicemanagement::add flowmeter");
      _devices[port] = new flowmeter(port);
      break;
    case SENSOR_PH:
      DEBUG_SERIAL("devicemanagement::add phsensor");
      _devices[port] = new phsensor(port);
      break;
    case SENSOR_TDS:
      DEBUG_SERIAL("devicemanagement::add: tdssensor");
      _devices[port] = new tdssensor(port);
      break;
    case SENSOR_TURBIDITY:
      DEBUG_SERIAL("devicemanagement::add turbiditysensor");
      _devices[port] = new turbiditysensor(port);
      break;
    case MOTOR_DRIVER:
      DEBUG_SERIAL("devicemanagement::add motor driver");
      break;  
    default:
      DEBUG_SERIAL("devicemanagement::add unknown");
      return(-1);
  }
  register_sensor(port);
  return(port);
}

// get device
device* devicemanagement::get(uint8_t id) {
  return(_devices[id]);
}

// update device
boolean devicemanagement::update(uint8_t id, device_info *info) {
  // if id not valid
  if ( id >= MAX_DEVICES || _devices[id] == NULL ) return(false);
  return(_devices[id]->setup(info) == SUCCESS ? true : false);
}

// delete device
boolean devicemanagement::del(uint8_t id) {
  if(_devices[id] == NULL) return(false);
  delete _devices[id];
  _devices[id] = NULL;
  return(true);
}

// get device information structure
void devicemanagement::getdevicesconfig(uint8_t id, device_info *dev) {
  // if not configured return no device else copy
  if (_devices[id] == NULL) dev->_type = NO_DEVICE;
  else _devices[id]->copy(dev);
}

// set device information structure
void devicemanagement::setdevicesconfig(device_info *dev) {
  // add device 
  int id = add(dev->_type);
  // configure device
  if ( id >= 0 ) _devices[id]->setup(dev);
}

// check if valid port
boolean devicemanagement::isvalid(uint8_t id) { return( id < MAX_DEVICES ? true : false); }

// loop for device execution
void devicemanagement::loop(void) {
  String json = "";
  // check if time elapsed 
  if ( _waittime <= DEVICE_MAX_SLEEP_TIME && iswaittime(_lasttime, _waittime) ) {
    // execute periodic actions action 
    for( int i=0; i< MAX_DEVICES; i++ ) if(_devices[i] != NULL) {
      String jsonexec = _devices[i]->execute();
      if ( jsonexec != "" )
        json += ( json != "" ? "," : "" ) + jsonexec; 
    }
    if ( json != "" ) post_data(json);
  }
}

// get device scheduler in miliseconds
unsigned long devicemanagement::scheduler(void) {
  unsigned long timer = (DEVICE_MAX_SLEEP_TIME + 1) ;
  for( int i=0; i< MAX_DEVICES; i++ ) 
    if(_devices[i] != NULL) {
      unsigned long t = _devices[i]->scheduler();
//      DEBUG_SERIAL("Scheduler (devicemanagement) t: " + String(t) + "  (" + i + ")");
      if ( t < timer ) timer = t;
    }
  _waittime = timer;
//  DEBUG_SERIAL("Scheduler (devicemanagement): " + String(_waittime) );
  return(_waittime * 1000);
}

// config 
uint16_t devicemanagement::config(String json) {
  // set device name
  if (jsonHasKey(json, "name") ) name(jsonKey(json, "name"));
  // set target url
  if (jsonHasKey(json, "target") ) target(jsonKey(json, "target"));
  //Serial.println("#" + jsonKey(json, "name") + "#" + jsonKey(json, "target") + "#");
  return(HTTP_SUCCESS);
}

// config updated
boolean devicemanagement::isupdated(void) { 
  // still pending for device control...
  return(false);
  boolean out = false;
  if ( _updated > 0 ) out = true;
  _updated = 0;
  for( int i=0; i< MAX_DEVICES; i++ ) if( _devices[i]->isupdated() ) out = true;
  return(out); 
}

// json device add
uint16_t devicemanagement::jsondeviceadd(String json, String &res) {
  // if key not exist return error
  if (!jsonHasKey(json, "type")) return(HTTP_BAD_REQUEST);
  // get key value
  uint8_t key = jsonKey(json, "type").toInt();
  // get id 
  uint8_t id = add(key);
  // return error id error on id allocation
  if ( id < 0 ) return(HTTP_UNAVAILABLE);
  // return success
  res = "{\"id\":" + String(id) + "}"; 
  return(HTTP_SUCCESS);
}

// json device config
uint16_t devicemanagement::jsondeviceconfig(uint8_t id, String s)
{
  if( id >= MAX_DEVICES || _devices[id] == NULL ) return(HTTP_UNAVAILABLE);
  if ( _devices[id]->setup(s) == SUCCESS ) return(HTTP_SUCCESS);
  return(HTTP_BAD_REQUEST);
}

// return json status
String devicemanagement::jsonstatus(void) {
  int activesensors=0;
  // get configured devices
  for(int i=0; i< MAX_DEVICES; i++ ) if(_devices[i] != NULL) activesensors++;
  // create config json
  String s = "{\"serial\":\"" + getChipId() + "\",\"type\":\"" + DEVICE_TYPE +
              "\",\"version\":\"" + VERSION + "\",\"uptime\":" + String(millis()/1000) + 
              ",\"name\":\"" + String(_name) + "\",\"target\":\"" + String(_target) +
              //( _name[0] != 0 ? ",\"name\":\"" + String(_name) + "\"" : "" ) + ( _target[0] != 0? ",\"target\":\"" + String(_target) + "\"" : "" ) +
              "\",\"activesensors\":" + String(activesensors) + ",\"supportedsensors\":" + MAX_DEVICES + "}" ;
  return(s);
}

// json device status
String devicemanagement::jsondevicestatus(void) {
  String s = "";

  for(int i=0; i< MAX_DEVICES; i++ ) s += (s != "" ? "," : "") + jsondevicestatus(i);
  return( "[" + s + "]");
}

// json device status
String devicemanagement::jsondevicestatus(uint8_t id) {
  String s = "";
  if (_devices[id] == NULL) return( "{\"id\": " + String(id) + ",\"status\":" + String(STATUS_NOT_CONFIGURED) + "}");
  return( _devices[id]->jsonconfig() );
}

// json device operation
uint16_t devicemanagement::operation(uint8_t id, String op, String json, String &res) {

  //DEBUG_SERIAL("devicemanagement (operation): " + String(id) + " (" + op + ") - " + json + " : " + json);

  // check if valid id
  if(!isvalid(id) ) return(HTTP_UNAVAILABLE);
  // call operation  
  if ( op == "enabled") _devices[id]->enable();
    else if ( op == "disable" ) _devices[id]->disable();
    else if ( op == "on" ) _devices[id]->on();
    else if ( op == "off" ) _devices[id]->off();
    else if ( op == "del" ) del(id);
    else if ( op == "read" ) res = "{\"value\":" + _devices[id]->read() + "}";
    else if ( op == "write" && jsonHasKey(json, "value")) _devices[id]->write(jsonKey(json, "value"));
    else return(HTTP_NOT_FOUND);
  // return success
  return(HTTP_SUCCESS);
}

// system save
void devicemanagement::systemSave(void) { _control |= PENDING_SAVE; }

// system reset
void devicemanagement::systemReset(void) { _control |= PENDING_RESTART; };

// pending save
boolean devicemanagement::issave(void) {
  boolean s = _control & PENDING_SAVE;
  _control &= ~PENDING_SAVE;
  return(s);  
}

// pending restart
boolean devicemanagement::isrestart(void) {
  boolean s = _control & PENDING_RESTART;
  _control &= ~PENDING_RESTART;
  return(s);  
}
