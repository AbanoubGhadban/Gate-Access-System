#ifndef EEPROM_H
#define EEPROM_H
#include <stdbool.h> 


void eeprom_init() ;
unsigned long eeprom_write(unsigned long block, unsigned long offset, unsigned long data);
unsigned long eeprom_read(unsigned long block, unsigned long offset) ;
void setPassword(char* password);
bool checkPassword(char* password);

#endif
