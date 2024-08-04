#ifndef _DEFINE_
#define _DEFINE_

// define version
#define VERSION           "v0.1.2402"

// define device type, do not forget to change board type
#define DEVICE_TYPE       ESP32
//#define DEVICE_TYPE     ESP8266


// set debug mode
#define DEBUG 1

// print debug info
#if ( DEBUG == 1 )
#define DBG_INIT(...)      Serial.begin(__VA_ARGS__)
#define DBG_PRINTLN(...)   Serial.println(__VA_ARGS__)
#define DBG_PRINT(...)     Serial.print(__VA_ARGS__)
#else
#define DBG_INIT(...)
#define DBG_PRINTLN(...)
#define DBG_PRINT(...)
#endif

//#define uint16_t  unsigned int 
//#define uint8 unsigned char

#define NAME_LENGHT         20
#define URL_LENGHT          100
#define MAX_DEVICES         16
#define MAX_PORTS           4

// supported device types
#define NO_DEVICE         0x00
#define DEVICE_ESP8266    0x01
#define DEVICE_ESP32      0x02

// device status
#define DEVICE_NOT_CONFIGURED 0x00


// port status
#define STATUS_DISABLED             0x00
#define STATUS_ENABLED              0x01
#define STATUS_ENABLED_AP_MODE      0x10
#define STATUS_ENABLED_STATION_MODE 0x12

#define STATUS_ERROR_BIT            0x80
#define STATUS_NOT_CONFIGURED       0xF0
#define STATUS_SETUP                0xF1
#define STATUS_ERROR                0xFF

#define PIN_NOT_CONFIGURED      0xFF

// error codes
//
#define SUCCESS                 0x00
#define ERROR_MISSING_PARAMETER 0x10
#define ERROR_DEVICE_STATUS     0x11
#define ERROR_NOT_SUPPORTED     0x12

#define HTTP_SUCCESS            200
#define HTTP_BAD_REQUEST        400
#define HTTP_FORBIDEN           403
#define HTTP_NOT_FOUND          404
#define HTTP_ALLOWED            405
#define HTTP_INTERNAL_ERROR     500
#define HTTP_NOT_IMPLEMENTED    501
#define HTTP_UNAVAILABLE        503

#define STATUS_LED_PIN          2

#endif