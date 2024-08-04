#ifndef _ESPWEBSERVER_
#define _ESPWEBSERVER_

#include "define.h"

#ifdef DEVICE_TYPE
  #if ( DEVICE_TYPE == ESP8266 ) 
    #include <ESP8266WebServer.h>     
  #endif
  #if ( DEVICE_TYPE == ESP32 ) 
    #include <WebServer.h>     
  #endif
#endif

#include "devicemanagement.h"
#include "esp-wifi.h"

// define device port
#define HTTP_PORT   80


#ifdef DEVICE_TYPE
#if ( DEVICE_TYPE == ESP8266 ) 
class espwebserver : public ESP8266WebServer {
  private:
    uint8_t _status;
    devicemanagement *_devices;
    espwifi *_wifi;
    
    // register handle procedures
    void handlerregister(void);

    // handlers
    void jsonSystemStatus(void);
    void jsonWifiStatus(void);
    void jsonDeviceStatus(String, String);
    void jsonDeviceAdd(String);    
    void jsonDeviceDel(String, String);
    void jsonSystemConfig(String);
    void jsonSystemSave(void);
    void jsonSystemReset(void);
    void jsonSystemFactoryReset(void);
    void jsonWifiConfig(String);
    void jsonDeviceConfig(String, String, String);
    void jsonDeviceOperation(String, String, String);
    void jsonNotFound(void);
    // send json
    void sendjson(uint16_t code, String json);

    // http components
    String httpHeader(void);
    String httpFooter(void);
    String httpFieldInput(String label, String fieldname, String value, bool readonly);
    String httpFieldSelect(String label, String fieldname, uint8_t value, bool readonly);

  public:
    espwebserver(espwifi *w, devicemanagement *d, uint16_t port=HTTP_PORT);
    // initiate web server
    void start(uint8_t status);
    // loop 
    void loop(void);
    // handler management
    void handler();
    void handlerWifiConfig(boolean readonly);
    void handlerUpdateWifiConfig();
    void handlerNotFound();
    
};
#endif
#if ( DEVICE_TYPE == ESP32 ) 
class espwebserver : public WebServer {
  private:
    uint8_t _status;
    devicemanagement *_devices;
    espwifi *_wifi;
    
    // register handle procedures
    void handlerregister(void);

    // handlers
    void jsonSystemStatus(void);
    void jsonWifiStatus(void);
    void jsonDeviceStatus(String, String);
    void jsonDeviceAdd(String);    
    void jsonDeviceDel(String, String);
    void jsonSystemConfig(String);
    void jsonSystemSave(void);
    void jsonSystemReset(void);
    void jsonSystemFactoryReset(void);
    void jsonWifiConfig(String);
    void jsonDeviceConfig(String, String, String);
    void jsonDeviceOperation(String, String, String);
    void jsonNotFound(void);
    // send json
    void sendjson(uint16_t code, String json);

    // http components
    String httpHeader(void);
    String httpFooter(void);
    String httpFieldInput(String label, String fieldname, String value, bool readonly);
    String httpFieldSelect(String label, String fieldname, uint8_t value, bool readonly);

  public:
    espwebserver(espwifi *w, devicemanagement *d, uint16_t port=HTTP_PORT);
    // initiate web server
    void start(uint8_t status);
    // loop 
    void loop(void);
    // handler management
    void handler();
    void handlerWifiConfig(boolean readonly);
    void handlerUpdateWifiConfig();
    void handlerNotFound();
    
};
#endif
#endif


#endif
