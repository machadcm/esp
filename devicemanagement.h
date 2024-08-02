#ifndef _DEVICEMANAGEMENT_
#define _DEVICEMANAGEMENT_

#include "define.h"
//#include "utils.h"
#include "wifi.h"
#include "device.h"
#include "phsensor.h"


#define PENDING_SAVE      0x01
#define PENDING_RESTART   0x02

#define MAX_BUFFER        16



#ifdef DEVICE_ESP8266 

/*
 *  GPIO0 - 0
 *  GRIO2 - 2 - ONBOARD LED - 
 */


const unsigned char boardSupportedPins[] = {0, 2, 4, 5, 14, 16};
#define MAX_AVAILABLE_PORTS 6

#endif



class devicemanagement : public utils {
  private:
    // define devices
    device *_devices[MAX_DEVICES];
    // name 
    char _name[NAME_LENGHT];
    // target URL
    char _target[URL_LENGHT];
    // is update
    uint8_t _updated;
    // is update
    uint8_t _control;
    wifi *_wifi;

    unsigned long _lasttime;    // last execution time 
    unsigned long _waittime;    // current wait time 

    String _buffer[MAX_BUFFER];

    // get chip id
    String getChipId(void);    
    // return buffer size
    int buffersize(void);
    // append data to buffer
    String pull(void);
    // send data to server
    void push(String t, String d);
    // register device
    void register_device(void);
    // register sensor
    void register_sensor(uint8_t id);
    // post data
    void post_data(String data);

  protected:
    
  public:
    devicemanagement(wifi *w);

    // setup configuration
    void setup(void);
    // reset
    void reset(void);
    // factory reset
    void factoryreset(void);
    // register devices 
    void registerdevice(void);
    // name
    const char *name(void);
    void name(const char *);
    void name(String);
    // target url
    const char *target(void);
    void target(const char *);
    void target(String);

    // add device
    int add(uint8_t type);
    // get device
    device *get(uint8_t id);
    // update device
    boolean update(uint8_t id, device_info *info);
    // delete device
    boolean del(uint8_t id);
    // get device configuration
    void getdevicesconfig(uint8_t id, device_info*);
    // set device configuration
    void setdevicesconfig(device_info*);
    // is valid port
    boolean isvalid(uint8_t id);
    // loop device execution
    void loop(void);
    // get device scheduler
    unsigned long scheduler(void);
    // config from json
    uint16_t config(String);
    // config updated
    boolean isupdated(void);
    // json device status
    String jsonstatus(void);
    // json device add
    uint16_t jsondeviceadd(String, String&);
    // json device config
    uint16_t jsondeviceconfig(uint8_t id, String s);
    // json device status
    String jsondevicestatus(void);
    // json device status
    String jsondevicestatus(uint8_t id);
    // json device operation
    uint16_t operation(uint8_t id, String op, String json, String &res);
    // system save
    void systemSave(void);
    // system reset
    void systemReset(void);
    // pending save
    boolean issave(void);  
    // pending restart
    boolean isrestart(void);  
    // send data to server   
    void send_data(void);

};

#endif
