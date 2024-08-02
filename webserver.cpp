#include "webserver.h"

/************************************************

  1. add device - POST /config/device/add
  2. config device - POST /config/device/<device id>
  3. enable device - POST /device/<device id>/enabled
  4. action device (on, off, read, etc.) - POST /device/<device id>/[on|off|read]
  5. disable device - POST /device/<device id>/disabled
  6. delete device - DEL /device/<device id>

************************************************/


// pointer to WebWerver class
webserver *webHandle;

#if ( DEVICE_TYPE == DEVICE_ESP8266 ) 
webserver::webserver(wifi *w, devicemanagement *d, uint16_t port):ESP8266WebServer(port) {
  _wifi = w; 
  _devices = d;
  webHandle = this;
}
#endif
#if ( DEVICE_TYPE == DEVICE_ESP32 ) 
webserver::webserver(wifi *w, devicemanagement *d, uint16_t port):WebServer(port) {
  _wifi = w; 
  _devices = d;
  webHandle = this;
}
#endif

// start web server
void webserver::start(uint8_t status) {
  // update status
  _status = status;
  //here the list of headers to be recorded
  const char * headerkeys[] = {"Content-Type"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  //ask server to track these headers
  collectHeaders(headerkeys, headerkeyssize );
  // register handlers
  handlerregister();
  // start web server
  begin();
}

// register web handler
void webserver::handlerregister(void) {
  on("/", HTTP_GET, [](){ webHandle->handler(); });
  onNotFound([](){ webHandle->handler(); });
}

// loop 
void webserver::loop() { handleClient(); }

// general handler 
void webserver::handler() {
  String uri = _currentUri;
  
  DEBUG_SERIAL("WebServer Handler: " + uri + " method (" + method() + "), body:" + arg("plain"));

  switch(method()) {
    case 1: // GET
      if ( _status == STATUS_ENABLED_STATION_MODE ) {
        if ( header("Content-Type") == "application/json" ) {    
          if (uri.startsWith("/status/system") ) jsonSystemStatus();
          else if (uri.startsWith("/status/wifi") ) jsonWifiStatus();
          else if (uri.startsWith("/status/devices") ) jsonDeviceStatus(uri, "/status/devices");
          else if (uri.startsWith("/status/device/") ) jsonDeviceStatus(uri, "/status/device/");
          else if (uri.startsWith("/device/") ) jsonDeviceOperation(uri, "/device/", "");
          else jsonNotFound();
        } else handlerNotFound();
      } else handlerWifiConfig(false); // AP mode
      break;
    case 3: // POST
      if ( _status == STATUS_ENABLED_STATION_MODE ) 
        if ( header("Content-Type") == "application/json" ) {    
          if (uri.startsWith("/config/system") ) jsonSystemConfig(arg("plain"));
          else if (uri.startsWith("/config/wifi") ) jsonWifiConfig(arg("plain"));
          else if (uri.startsWith("/config/device/add") ) jsonDeviceAdd(arg("plain"));          
          else if (uri.startsWith("/config/device/") ) jsonDeviceConfig(uri, "/config/device/", arg("plain"));
          else if (uri.startsWith("/system/save") ) jsonSystemSave();
          else if (uri.startsWith("/system/reset") ) jsonSystemReset();
          else if (uri.startsWith("/system/factoryreset") ) jsonSystemFactoryReset();
          else if (uri.startsWith("/device/") ) jsonDeviceOperation(uri, "/device/", arg("plain"));
          else jsonNotFound();
        } else handlerNotFound();
      else // AP mode 
        if (uri.startsWith("/config/wifi") ) handlerUpdateWifiConfig();
        else handlerNotFound();
      break;
    case 0: // DELETE
      if ( _status == STATUS_ENABLED_STATION_MODE ) 
        if ( header("Content-Type") == "application/json" ) {    
          if (uri.startsWith("/device/") ) jsonDeviceDel(uri, "/device/");
          else jsonNotFound();
        } else handlerNotFound();

    default:
      break;  
  }
}

// return json system status
void webserver::jsonSystemStatus(void) { sendjson(HTTP_SUCCESS, _devices->jsonstatus()); }

// return json wifi statue
void webserver::jsonWifiStatus(void) { sendjson(HTTP_SUCCESS, _wifi->jsonstatus()); }

// return json status port 
void webserver::jsonDeviceStatus(String uri, String path) {
  // remove base path
  uri.replace(path, "");
  // if empty return all device status
  if( uri == "" ) sendjson(HTTP_SUCCESS, _devices->jsondevicestatus());
  // get port from uri
  uint8_t id = uri.substring(0, ( uri.indexOf('/') ? uri.indexOf('/') : 4 )).toInt();
  // if valid port return port configuration
  if( _devices->isvalid(id) ) sendjson(HTTP_SUCCESS, _devices->jsondevicestatus(id));
  else {
    // else return error
    DEBUG_SERIAL("webserver::jsonDeviceStatus: HTTP_BAD_REQUEST");
    sendjson(HTTP_BAD_REQUEST, "");
  }
}

// config system with json
void webserver::jsonSystemConfig(String json) { sendjson(_devices->config(json), ""); }

// save system configuration
void webserver::jsonSystemSave(void) { 
  _devices->systemSave();
  sendjson(200, ""); 
}

// reset system 
void webserver::jsonSystemReset(void) {
  _devices->systemReset();
  sendjson(200, ""); 
}

// reset system 
void webserver::jsonSystemFactoryReset(void) {
  _devices->factoryreset();
  sendjson(200, ""); 
}

// config wifi with json
void webserver::jsonWifiConfig(String json) { sendjson(_wifi->config(json), ""); }

// add device
void webserver::jsonDeviceAdd(String json) { 
  String s;
  sendjson(_devices->jsondeviceadd(json, s), s); 
}

// add device
void webserver::jsonDeviceDel(String uri, String path) { 
  String s;
  // remove base path
  uri.replace(path, "");
  // get port from uri
  uint8_t id = uri.substring(0, uri.indexOf('/')).toInt();
  if( !_devices->isvalid(id) ) {
    sendjson(HTTP_BAD_REQUEST, "");
    DEBUG_SERIAL("webserver::jsonDeviceDel " + uri + ", " + path );
  } 
  sendjson(_devices->operation(id, "del", "", s), s);
}

// config device
void webserver::jsonDeviceConfig(String uri, String path, String json) { 
  // remove base path
  uri.replace(path, "");
  // get port from uri
  uint8_t id = uri.substring(0, uri.indexOf('/')).toInt();
  if( !_devices->isvalid(id) || json == "" ) {
    sendjson(HTTP_BAD_REQUEST, "");
    DEBUG_SERIAL("webserver::jsonDeviceConfig " + uri + ", " + path );
  } 
  sendjson(_devices->jsondeviceconfig(id, json), "");
}

// return json device opeation
void webserver::jsonDeviceOperation(String uri, String path, String json) {
  DEBUG_SERIAL("webserver::jsonDeviceOperation " + uri + ", " + path  + ", " + json );

  String res = "";
  // remove base path
  uri.replace(path, "");
  // get operation from uri
  String op = uri.substring(0, uri.indexOf('/'));
  // get port from uri
  uint8_t id = uri.substring(uri.indexOf('/'), 10).toInt();

  sendjson(_devices->operation(id, op, json, res), res);
}


// return json not found
void webserver::jsonNotFound(void) { 
  DEBUG_SERIAL("webserver (jsonNotFound): HTTP_NOT_FOUND");
  sendjson(HTTP_NOT_FOUND, ""); 
}

void webserver::sendjson(uint16_t code, String json) {
  String description;
  switch(code) {
    case HTTP_SUCCESS:
      description = "Success";
      break;
    case HTTP_BAD_REQUEST:
      description = "Bad request";
      break;
    case HTTP_FORBIDEN:
      description = "Forbiden";
      break;
    case HTTP_NOT_FOUND:
      description = "Not found";
      break;
    case HTTP_ALLOWED:
      description = "Not allowed";
      break;
    case HTTP_INTERNAL_ERROR:
      description = "Internal error";
      break;
    case HTTP_NOT_IMPLEMENTED:
      description = "Not Implemented";
      break;
    case HTTP_UNAVAILABLE:    
      description = "Unavailable";
      break;
    default:
      description = "Unknown";
      break;
  }
  String message = "{\"return\":" + String(code) + ",\"description\":\"" + description + "\"" +
              (json != "" ? ",\"data\":" + json : "") + "}";
  
  DEBUG_SERIAL("webserver (sendjson): code:" + String(code) + ", " + message);
  send(code, "application/json", message);
}


// print wifi configuration when in AP mode
void webserver::handlerWifiConfig(boolean readonly) {
  String s = httpHeader();
  s += "<ul>" + httpFieldInput("Configured SSID:","", _wifi->ssid(), true ) + "</ul>";
  s += "<form action=\"config/wifi\" method=\"post\"><ul>\r\n";
  s += "<li><label>Available networks:</label><select name=\"ssid\">\n\r";
  
  int networks = WiFi.scanNetworks(); 
  DEBUG_SERIAL("Networks found: " + String(networks));
  for (int i = 0; i < networks; i++) {
    s+= "<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>\r\n"; 
    DEBUG_SERIAL(i + ": " + WiFi.SSID(i) + ", " + String(WiFi.RSSI(i)) + ", " + String(WiFi.encryptionType(i)));
  }
  s += "</select></li>\r\n";
  s += httpFieldInput("Password:", "pass", "", (readonly ? true : false));
  if (!readonly) s += "<li><button type=\"submit\">Save</button></li>\r\n";
  s += "</ul></form>";
  s += httpFooter();
  // Send the response to the client
  send(200, "text/html", s);
}

// wifi configuration updated
void webserver::handlerUpdateWifiConfig() {
  DEBUG_SERIAL("handlerUpdateWifiConfig : " + arg("ssid") + ", " + arg("pass"));
  if ( _wifi->setcredentials(arg("ssid"), arg("pass")))  send(200, "text/html", "<h1>WiFi configuration updated, device will restart soon.</h1>");
  DEBUG_SERIAL("webserver::handlerUpdateWifiConfig: HTTP_BAD_REQUEST");
  send(HTTP_BAD_REQUEST, "text/html", "<h1>Fail to update WiFi configuration.</h1>");
};

// handler not found
void webserver::handlerNotFound() {
    DEBUG_SERIAL("WebServer (NotFound): " + _currentUri);

  send(200, "text/html", "<h1>Page not found</h1>");
}


// html header
String webserver::httpHeader(void) {
  String s = "<!DOCTYPE HTML>\r\n";
  s += "<head><link rel=\"icon\" href=\"data:,\"></head>";
  s += "<style>";
  s += "html,body {font:20px Calabri,Arial; margin-left: 10px;}\r\n";
  s += "label {width:200px;display: inline-block;}\r\n";
  s += "ul {padding:0}\r\n";
  s += "li {list-style-type:none; margin: 20px 0}\r\n";
  s += "input,select {width: 200px;padding:4px 6px}\r\n";
  s += "</style>";
  s += "<html><body>\r\n<h1><a href=\"http://" + _wifi->ip() + "\">ESP module</a> </h1>";
  return(s);  
}

// html footer
String webserver::httpFooter(void) {
  return("</body></html>");  
}

// html input field
String webserver::httpFieldInput(String label, String fieldname, String value, bool readonly) {
  String s = "<li><label>" + label + "</label>";
  s += "<input type=\"text\"" + (fieldname == "" ? "" : " name=\"" + fieldname + "\" ") + 
                                (value == "" ? "" : " value=\"" + value + "\" ") +
                                (readonly?" readonly":"") + "></li>\r\n"; 
  return(s);
}

// html select field
String webserver::httpFieldSelect(String label, String fieldname, uint8_t value, bool readonly) {
  String s = "<li><label>" + label + "</label>";
  s += "<select name=\"" + fieldname + "\">\r\n";
  s += "</select></li>\r\n";
  return(s);
}
