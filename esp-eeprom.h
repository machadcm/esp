#ifndef _EEPROM_
#define _EEPROM_

//#include "define.h"
#include <Arduino.h>
#include <EEPROM.h>


#define EEPROM_SIZE 1024

#define EEPROM_OK               0
#define EEPROM_CHECKSUM_ERROR   1

class eeprom {
  private:
    char *_buffer;
    int _length;
    
    uint8_t checksum(void);
  public:
    eeprom(char *buffer, int length);
    ~eeprom();
    void setup(char *buffer, int length);
    int read(void);
    int write(void);
};


#endif