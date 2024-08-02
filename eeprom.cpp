#include <Arduino.h>
#include "eeprom.h"

// constructor
eeprom::eeprom(char *buffer, int length) { 
  _buffer = buffer;
  _length = length; 
  // initialize eeprom
  EEPROM.begin(EEPROM_SIZE);
}

// destructor
eeprom::~eeprom() { EEPROM.end(); }

// constructor
void eeprom::setup(char *buffer, int length) { 
  _buffer = buffer;
  _length = length;
  if (_length >= (EEPROM_SIZE - 2) ) _length = 0; 
}


// read bytes from eeprom
int eeprom::read(void) {
  // read bytes from eeprom
  int i=0;
  for (; i < _length; i++) { _buffer[i] = EEPROM.read(i); }
  uint8_t cs = EEPROM.read(i);
  // calculate check sum
  if (cs != checksum()) return(EEPROM_CHECKSUM_ERROR);
  return(EEPROM_OK);
}

// write eeprom
int eeprom::write(void) {
  int i=0;
  for (; i < _length; i++) { EEPROM.write(i,_buffer[i]); }
  if( i ) {
    // write checksum  
    uint8_t cs = checksum();
    EEPROM.write(i, cs);
    // commit changes
    EEPROM.commit();
  }
  return(i);
}

// calculate checksum
//
uint8_t eeprom::checksum(void) {
  uint8_t csum = 0;
  for(int i=0; i<_length; i++) csum += _buffer[i];
  return (csum & 0x0FF);  
}
