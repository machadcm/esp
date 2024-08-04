#ifndef _CONTROLLER_
#define _CONTROLLER_

#include "define.h"
#include "esp-eeprom.h"
#include "device.h"
#include "esp-wifi.h"
#include "esp-webserver.h"
#include "devicemanagement.h"
#include "status.h"


#define MAX_SLEEP_TIME    100 
#define PENDING_LOOP      10


struct info {
  char _ssid[NAME_LENGHT];
  char _password[NAME_LENGHT];
  char _name[NAME_LENGHT];
  char _url[URL_LENGHT];  
  device_info _devices[MAX_DEVICES];
};



class controller : public utils {
  private:
    uint8_t _status;
    // led status 
    status _led{STATUS_LED_PIN};
    // define devices
    devicemanagement *_devices;
    // define wifi module
    espwifi *_wifi;    
    // define web server
    espwebserver *_webserver;
    // wifi setup function
    uint8_t wifisetup(void);

  protected:
    // load data from eeprom
    boolean load(void);
    // save data to eeprom 
    boolean save(void);
    // factory reset
    void factoryreset(void);
    
  public:
    // constructor
    controller(espwifi *w);
    // destructor
    ~controller();
    // setup configuration
    void setup(void);
    // reset
    void reset(void);
    // loop 
    void loop(void);
};

#endif
