#include "tm4c123gh6pm.h"
#include "EEPROM.h"
#include "delay.h"
#include <stdbool.h> 

void eeprom_init() {
    SYSCTL_RCGCEEPROM_R |= SYSCTL_RCGCEEPROM_R0;
    while (!(SYSCTL_PREEPROM_R & SYSCTL_PREEPROM_R0));
    delayUs(10);
    while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);

    if (EEPROM_EESUPP_R & (EEPROM_EESUPP_PRETRY | EEPROM_EESUPP_ERETRY)) {
        // Error
        return;
    }

    SYSCTL_SREEPROM_R |= SYSCTL_SREEPROM_R0;
    delayUs(100);
    SYSCTL_SREEPROM_R &= ~SYSCTL_SREEPROM_R0;
    while (!(SYSCTL_PREEPROM_R & SYSCTL_PREEPROM_R0));
    while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);
    if (EEPROM_EESUPP_R & (EEPROM_EESUPP_PRETRY | EEPROM_EESUPP_ERETRY)) {
        // Error
        return;
    }
}

unsigned long write(unsigned long block, unsigned long offset, unsigned long data) {
    EEPROM_EEBLOCK_R = block;
    EEPROM_EEOFFSET_R = offset;

    EEPROM_EERDWR_R = data;
    delayMs(2);
    return EEPROM_EERDWR_R;
}

unsigned long read(unsigned long block, unsigned long offset) {
    EEPROM_EEBLOCK_R = block;
    EEPROM_EEOFFSET_R = offset;

    return EEPROM_EERDWR_R;
}

void SetPassword(char ch[32]){
    int index = 0 ; 
    for(int i =0 ; i< 4 ; i ++)
    {
      for(int j = 0 ; j < 8 ; j++)
      {
        write(i , j , ch[index]);
        index++ ; 
      }
    }
}

bool CheckPassword(unsigned char ch[32])
{
  bool val = true;
  int index = 0 ; 
  for(int i =0 ; i< 4 ; i ++)
    {
      for(int j = 0 ; j < 8 ; j++)
      {
        if(ch[index] != read(i , j))
        {
          val = false ;
          break;
        }
        index++ ; 
      }
    }
  return val ;
}