#ifndef EEPROM_H
#define EEPROM_H
#include <stdbool.h> 


void eeprom_init() ;
unsigned long write(unsigned long block, unsigned long offset, unsigned long data);
unsigned long read(unsigned long block, unsigned long offset) ;
void SetPassword(char ch[32]);
bool CheckPassword(unsigned char ch[32]) ; 

#endif