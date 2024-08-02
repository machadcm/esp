#ifndef _DEFINE_
#define _DEFINE_

// set debug mode
#define DEBUG 1

// print debug info
#ifdef DEBUG
#define DEBUG_INIT(...)      Serial.begin(__VA_ARGS__)
#define DEBUG_PRINTLN(...)   Serial.println(__VA_ARGS__)
#define DEBUG_PRINT(...)     Serial.print(__VA_ARGS__)
#define DEBUG_SERIAL(...)    Serial.println(__VA_ARGS__)
#else
#define DEBUG_INIT(...)
#define DEBUG_PRINTLN(...)
#define DEBUG_PRINT(...)
#define DEBUG_SERIAL(...)
#endif

//#define uint16_t  unsigned int 

#define NAME_LENGHT         20
#define URL_LENGHT          100
#define MAX_DEVICES         16
#define MAX_PORTS           4

// supported device types
#define NO_DEVICE         0x00
#define DEVICE_ESP8266    0x01
#define DEVICE_ESP32      0x02

// define device type
#define DEVICE_TYPE       DEVICE_ESP32
//define DEVICE_TYPE      DEVICE_ESP8266

#define VERSION           "v0.1.2402"

// status
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
