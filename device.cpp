#include "device.h"

// return waittime in seconds
unsigned long utils::waittime(unsigned long inittime, unsigned long waittime) {
  unsigned long nowtime = now();

  // if not counter overflow return remaining seconds
  if ( nowtime >= inittime ) {
    if ( nowtime > (inittime + waittime)) return(0);
      else return( (inittime + waittime) - nowtime );
  }
  // return remaining seconds considering overflow
  return ( ( nowtime + 0x418937 - inittime ) - ( inittime + waittime));
}


// check if key exists in json
boolean utils::jsonHasKey(String json, String key) {
  return(json.indexOf(key) >= 0 ? true : false);
}

// return json 
String utils::jsonKey(String json, String key) {
  // find key
  int b = json.indexOf(key) + key.length() + 2;
  int e = json.indexOf(',', b);
  String o = json.substring(b, e > 0 ? e : 120);
  // process string
  o.trim();
  o.replace(": ",":");
  o.replace("\"","");
  o.replace("{","");
  o.replace("}","");
  return( o );
}

// class constructor
device::device(uint8_t id): utils()  {
  _id = id;
  _scheduler = _timer = _lastTime = 0;
  String name = "Device " + String(id);
  name.toCharArray(_name, NAME_LENGHT);
}

/*
// parse string and configure device
uint16_t device::setup(String json) {
  Serial.println("device::setup");

  // set name 
  if (jsonHasKey(json, "name")) jsonKey(json, "name").toCharArray(_name, NAME_LENGHT);
  // set scheduler
  if (jsonHasKey(json, "scheduler")) _scheduler = jsonKey(json, "scheduler").toInt();
  // set timer
  if (jsonHasKey(json, "timer")) _scheduler = jsonKey(json, "timer").toInt();
  _status = STATUS_SETUP;
  return(SUCCESS);
}
*/

// configure from dev structure
uint16_t device::setup(device_info *dev) {
  _scheduler = dev->_scheduler;
  _timer = dev->_timer;
  _status = dev->_status;
  strcpy(dev->_name, _name);
  return(SUCCESS);
}

// return waiting time for scheduling in seconds
unsigned long device::scheduler(void) {
  unsigned long wt = DEVICE_MAX_SLEEP_TIME+1;
   // if status not enable
  if (_status != STATUS_ENABLED ) return(wt);
  // if scheduler and timer not enable return max time
  if(!_scheduler && !_timer) return(wt);
  // return remaining time 
  if ( ison() ) {
    if ( _timer ) wt = waittime(_lastTime, _timer);
  } else {
    if ( _scheduler ) wt = waittime(_lastTime, _scheduler);
  }
  
  return(wt);
}

// json execution string
String device::json_execute(String value) {
  return ("{\"uptime\":" + String(now()) + ",\"id\":" + String(_id) + ",\"value\":\"" + value + "\"}");
}

// check if execution time
boolean device::checktime(void) {
  if (_status != STATUS_ENABLED ) return(false);
  if (!_scheduler && !_timer) return(false);

  if( ( !ison() && iswaittime(_lastTime, _scheduler) ) ||
      ( ison() && iswaittime(_lastTime, _timer) ) ) {
    _lastTime = now();
    return(true);
  }
  return(false);
}

// return device status
uint16_t device::status(void) { return(_status); }

// config updated
boolean device::isupdated(void) { 
  if ( _updated == 0 ) return(false); 
  _updated = 0;
  return(true); 
}

// get json info config
String device::deviceinfo(uint8_t  type) {
  String s = "\"id\":" + String(_id) + ",\"status\":" + String(_status) + ",\"name\":\"" + _name + "\""  + ",\"type\":\"" + sensorname(type) + "\"";
  return(s);
}

// get sensor name
String device::sensorname(uint8_t id) {
  switch(id) {
    case SENSOR_PORT_DRIVER:
      return("Port driver");
    case SENSOR_ANALOG_INPUT:
      return("Analog input");
    case SENSOR_DIGITAL_INPUT:
      return("Digital input");
    case SENSOR_TEMPERATURE_DHT22:
      return("Temperature and humidity sensor");
    case SENSOR_TEMPERATURE_DS18B20:
      return("Temperature sensor");
    case SENSOR_DISTANCE:
      return("Distance sensor");
    case SENSOR_FLOWMETER:
      return("Flowmeter sensor");
    case SENSOR_PH:
      return("PH sensor");
    case SENSOR_TDS:
      return("TDS sensor");
    case SENSOR_TURBIDITY:
      return("Turbidity sensor");
    case SENSOR_SOIL_MOISTURE:
      return("Soil moiture sensor");
    case MOTOR_DRIVER:
      return("Motor driver");
    default:
      return("Unknown sensor");
  }
}
