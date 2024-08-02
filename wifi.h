#ifndef _WIFI_
#define _WIFI_

#include "define.h"

#ifdef DEVICE_TYPE
  #if ( DEVICE_TYPE == DEVICE_ESP8266 ) 
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
  #endif
  #if ( DEVICE_TYPE == DEVICE_ESP32 ) 
    #include <WiFi.h>     
    #include <HTTPClient.h>
  #endif
#endif


#define HTTP_PORT   80

#define DEFAULT_AP_SSID   "ESPMODULE"
#define DEFAULT_AP_PASSWORD  ""

#define WIFI_AP_MODE      0x01
#define WIFI_STATION_MODE 0x02

#define MAX_WIFI_WAIT_TIME 20 // times 500ms 

#define MAX_BUFFER 20

class wifi : public WiFiServer { 
  private:
    char _ssid[32];
    char _password[32];
    uint8_t _status;
    uint8_t _updated;
    String _buffer[MAX_BUFFER][2];
    uint8_t _bufferEntries = 0;
    
    boolean connectAsAP(void);
    boolean connectAsStation(void);

    boolean buffer_isempty(void);
    void buffer_remove(void);
    void buffer_add(String path, String json);
    // post data 
    int post(String path, String json);

  public:
    // constructor
    wifi(uint16_t port);
    // setup wifi config
    boolean setup(uint8_t mode);
    // return status
    uint16_t status(void) { return(_status); };
    // disconnect
    boolean disconnect(void) { return(WiFi.disconnect()); }
    // check status and restart if needed
    void check(void);
    // get SSID
    String ssid(void) { return(String(_ssid)); }
    // get ip address
    String ip(void);
    // get ip netmask
    String netmask(void);
     // get ip gateway
    String gateway(void);
    // set wifi credentials
    boolean setcredentials(char *ssid, char *password);
    boolean setcredentials(String ssid, String password);
    void getcredentials(char *ssid, char *password);
    // config from json
    uint16_t config(String);
    // true if default AP
    boolean isdefaultAp(void);
    // config updated
    boolean isupdated(void);
    // diagnostic
    void diag(void);
    // json config
    String jsonstatus(void);
    // send data 
    void send(String path, String json);

};

#endif
