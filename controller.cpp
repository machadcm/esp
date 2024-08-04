#include "controller.h"
#include "status.h"

// constructor
controller::controller(espwifi *w) {
  _wifi = w;
  _devices = new devicemanagement(w);
  _webserver = new espwebserver(_wifi, _devices);
  _status = STATUS_SETUP;
  _led.blink(Starting);
}

// constructor
controller::~controller() {
  delete _webserver;
  delete _devices;
}

// setup controller
void controller::setup(void) {
  DBG_PRINTLN("Setup devices");
  // setup led
  _led.begin();
  // setup devices  
  _devices->setup();  
  // load stored configuration
  if ( !load() ) factoryreset();
  // setup wifi
  _status = wifisetup();
  // start webserver
  _webserver->start(_status);
}

uint8_t controller::wifisetup(void) {
  uint8_t status;
  // setup wifi
  if (_wifi->isdefaultAp() ) {
    status = ( _wifi->setup(WIFI_AP_MODE) ? STATUS_ENABLED_AP_MODE : STATUS_ERROR );
  } else { 
    status = ( _wifi->setup(WIFI_STATION_MODE) ? STATUS_ENABLED_STATION_MODE : STATUS_ERROR );
  }
  // print wifi diagnostics
  _wifi->diag();
  // start webserver
  return(status);
}

// load configuration
boolean controller::load(void) {
  info _info;
  eeprom _eeprom((char*)&_info, sizeof(info));;
  DBG_PRINTLN("Load data from EEPROM");
  // read from eeprom
  if (_eeprom.read() == EEPROM_OK) {
    _wifi->setcredentials(_info._ssid, _info._password);
    // setup devices
    for( int i=0; i< MAX_DEVICES; i++)
      if( _info._devices[i]._type != NO_DEVICE ) _devices->setdevicesconfig(&_info._devices[i]);
    // load name and target url
    _devices->name(_info._name);
    _devices->target(_info._url);
    // reset isupate flag
    _devices->isupdated();
    _wifi->isupdated(); 
    return(true);  
  }
  DBG_PRINTLN("Checksum error on EEPROM");
  return(false);
}

// save configuration
boolean controller::save(void) {
  info _info;

  // get wifi credentials
  _wifi->getcredentials(_info._ssid, _info._password);
  // get devices configuration
  for( int i=0; i< MAX_DEVICES; i++) _devices->getdevicesconfig(i, &_info._devices[i]);
  // set url and name
  strcpy(_info._name, _devices->name());
  strcpy(_info._url, _devices->target());

  DBG_PRINTLN("Write data on EEPROM: " + String(sizeof(info)));
  eeprom _eeprom((char*)&_info, sizeof(info));
  int wres = _eeprom.write();
  DBG_PRINTLN("Data Writen: " + String(wres));
  return(wres > 0 ? true : false);
}

// reset device
void controller::factoryreset(void) {
  DBG_PRINTLN("Factory reset!");
  // set wifi default
  _wifi->setcredentials(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD);
  _devices->factoryreset();
}

// reset device
void controller::reset(void) {};

// loop 
void controller::loop(void) {
  uint8_t pendingrequest = PENDING_LOOP;
  for(int i = 0; i<100; i++) { 
    _led.blink(Connecting);
    delay(100);
  }
  // print ip
  DBG_PRINTLN("IP address: " + _wifi->ip());
  // register device
  _devices->registerdevice();
  // loop forever
  for(;;) {
    // loop devices for execution
    _devices->loop();
    // check web server
    _webserver->loop();
    // process pendig requests every PENDING_LOOP
    if ( pendingrequest ) pendingrequest--;
    else {
      // is wifi changes or save operation 
      // FALTA reset do device
      if ( _wifi->isupdated() || _devices->issave() ) save();
      if ( _devices->isrestart() ) reset();
      // transmit data if pending
      _devices->send_data();
      // check wifi connectivity
      _wifi->check();
      // restart pending counter
      pendingrequest = PENDING_LOOP;
    }
    // get new time from devices scheduler
    unsigned long sleep = _devices->scheduler();
    if ( sleep > MAX_SLEEP_TIME) sleep = MAX_SLEEP_TIME;
    // blink led status
    _led.blink(Connected);
    // sleep 
    delay(sleep);
  }
}
