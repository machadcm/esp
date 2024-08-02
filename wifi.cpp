#include "wifi.h"


//#include <ESP8266Ping.impl.h>

// constructor
wifi::wifi(uint16_t port):WiFiServer(port) {
  _status = STATUS_SETUP;
  strcpy(_ssid, DEFAULT_AP_SSID);
  strcpy(_password, DEFAULT_AP_PASSWORD);
  _updated = 0;
}

// setup wifi module
boolean wifi::setup(uint8_t mode) {
  // connect as AP
  if ( mode == WIFI_AP_MODE) return(connectAsAP());
  return(connectAsStation()); 
}

// check
void wifi::check(void) {
  //Serial.println("wifi check, status: " + String(_status) );
    // if not connected try to connect
  if ( _status == STATUS_ENABLED_STATION_MODE && WiFi.status() != WL_CONNECTED ) connectAsStation();
}

// get IP address
String wifi::ip(void) {
  String ip = IPAddress(( _status == STATUS_ENABLED_AP_MODE ? (uint32_t)WiFi.softAPIP() : (uint32_t)WiFi.localIP())).toString();
  return(ip);
}

// get IP netmask 
String wifi::netmask(void) {
  String n = IPAddress((uint32_t)WiFi.subnetMask()).toString();
  return(n);
}

// get IP gateway 
String wifi::gateway(void) {
  String n = IPAddress((uint32_t)WiFi.gatewayIP()).toString();
  return(n);
}


// update credentials
boolean wifi::setcredentials(char *ssid, char *password) {
  strcpy(_ssid, ssid);
  strcpy(_password, password);
  return(true);
}

// update credentials
boolean wifi::setcredentials(String ssid, String password) {
  ssid.toCharArray(_ssid, 32);
  password.toCharArray(_password, 32);
  if( _status == STATUS_ENABLED_AP_MODE || _status == STATUS_ENABLED_STATION_MODE) _updated = 1;
  return(true);
}


// return credentials
void wifi::getcredentials(char *ssid, char *password) {
  // update credentials
  strcpy(ssid, _ssid);
  strcpy(password, _password);
}


// start connection in AP mode
boolean wifi::connectAsAP(void) {
  if(_status == STATUS_ENABLED_AP_MODE || _status == STATUS_ENABLED_STATION_MODE) return(false);
  DEBUG_SERIAL("Connect as AP (" + String(_ssid) + "/" + String(_password) + ")");
  // set AP mode
  WiFi.mode(WIFI_AP);
  // start AP
  if (WiFi.softAP(_ssid, _password)) {
    _status = STATUS_ENABLED_AP_MODE;
    return(true);
  }
  _status = STATUS_ERROR;
  return(true);
}

// start connection in AP mode
boolean wifi::connectAsStation(void) {
//  if(_status == STATUS_ENABLED_AP_MODE || _status == STATUS_ENABLED_STATION_MODE) return(false);
  DEBUG_SERIAL("Connect as STATION (" + String(_ssid) + "/" + String(_password) + ")");
  // force disconnect
  disconnect();
  // set mode to station
  WiFi.mode(WIFI_STA); 
  delay(100);
  WiFi.begin(_ssid, _password);
  for(int i=0; i < MAX_WIFI_WAIT_TIME; i++) {
    delay(500);
    if ( WiFi.status() ) {
       _status = STATUS_ENABLED_STATION_MODE;
       DEBUG_SERIAL("CONNECTED!");
       delay(2000);
       return(true);
    }
    _status = STATUS_ERROR;
    DEBUG_SERIAL("ERROR!: " + String(WiFi.status()));
    return(false);
  }
  return(true);
}


uint16_t wifi::config(String json) {
  DEBUG_PRINTLN("Wifi config!!!");
  return(0);
}

boolean wifi::isdefaultAp(void) {
  return(strcmp(_ssid, DEFAULT_AP_SSID) == 0 ? true : false);
}

// config updated
boolean wifi::isupdated(void) { 
  if ( _updated == 0 ) return(false); 
  _updated = 0;
  return(true); 
}

// print diagnostics
void wifi::diag(void) {
  WiFi.printDiag(Serial);
}


// return json status
String wifi::jsonstatus(void) {
  String s = "{\"status\":" + String(WiFi.status()) + ",\"ip\":\"" + ip() + + "\",\"netmask\":\"" + netmask() + + "\",\"gateway\":\"" + gateway() + 
             "\",\"ssid\":\"" + WiFi.SSID() + "\",\"rssi\":" + WiFi.RSSI() + "}";
  return(s);
}

// if buffer is empty
boolean wifi::buffer_isempty(void) { return( _bufferEntries == 0 ? true : false ); }

// remove first entry from buffer
void wifi::buffer_remove(void) {
  if ( _bufferEntries > 0 ) {
    // remove first entry
    for( int i=1; i < _bufferEntries; i++) {
      _buffer[i-1][0] = _buffer[i][0];
      _buffer[i-1][1] = _buffer[i][1];
    }
    // decrease entries
    _bufferEntries--;
    // set to null 
    _buffer[_bufferEntries][0] = "";
    _buffer[_bufferEntries][1] = "";
  }
}

// add to end of buffer
void wifi::buffer_add(String path, String json) {
  // if buffer full remove oldest
  if ( _bufferEntries == MAX_BUFFER ) buffer_remove();
  _buffer[_bufferEntries][0] = path;
  _buffer[_bufferEntries][1] = json;
  // increase number of entries
  _bufferEntries ++;
}

// send data
void wifi::send(String path, String json) {
  if ( post(path, json) == -1 ) {
    // not reachable, add to buffer
    buffer_add(path, json);
  } else {
    // connection ok send old data
    while ( !buffer_isempty() )
      if ( post(_buffer[0][0], _buffer[0][1]) == -1) break;
        else buffer_remove();
  }
}


// post data 
int wifi::post(String path, String json){
  if ( WiFi.status() != WL_CONNECTED ) return(-1);
  // connected send data  
  WiFiClient client;
  HTTPClient http;
  // connect to server
  http.begin(client, path);
  // set content type as Json
  http.addHeader("Content-Type", "application/json");
  // send data and get response code
  int httpResponseCode = http.POST(json);
  http.end();

  DEBUG_PRINTLN("Sending post: " + path + " - " + json + " - code:" + String(httpResponseCode));
  return(httpResponseCode); 
}
