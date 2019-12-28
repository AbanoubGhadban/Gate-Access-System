#include "tm4c123gh6pm.h"
#include "EEPROM.h"
#include "delay.h"

inline void eeprom_waitToFinish() {
    while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);
}

char* hashPassword(char* password) {
    int len = 0, i = 0;
    bool endReached = false;
    char* hashedPass;

    hashedPass = (char*)malloc(32*sizeof(char));
    for (i = 0; i < 32; ++i) {
        if (!endReached && !password[i])
            endReached = true;

        if (endReached)
            hashedPass[i] = 0;
        else
            hashedPass[i] = password[i];
    }
    return hashedPass;
}

void eeprom_init() {
    SYSCTL_RCGCEEPROM_R |= SYSCTL_RCGCEEPROM_R0;
    while (!(SYSCTL_PREEPROM_R & SYSCTL_PREEPROM_R0));
    delayUs(10);
    eeprom_waitToFinish();

    if (EEPROM_EESUPP_R & (EEPROM_EESUPP_PRETRY | EEPROM_EESUPP_ERETRY)) {
        // Error
        return;
    }

    SYSCTL_SREEPROM_R |= SYSCTL_SREEPROM_R0;
    delayUs(100);
    SYSCTL_SREEPROM_R &= ~SYSCTL_SREEPROM_R0;
    while (!(SYSCTL_PREEPROM_R & SYSCTL_PREEPROM_R0));
    eeprom_waitToFinish();
    if (EEPROM_EESUPP_R & (EEPROM_EESUPP_PRETRY | EEPROM_EESUPP_ERETRY)) {
        // Error
        return;
    }
}

unsigned long eeprom_write(unsigned long block, unsigned long offset, unsigned long data) {
    eeprom_waitToFinish();
    EEPROM_EEBLOCK_R = block;
    EEPROM_EEOFFSET_R = offset;

    EEPROM_EERDWR_R = data;
    eeprom_waitToFinish();
    return EEPROM_EERDWR_R;
}

unsigned long eeprom_read(unsigned long block, unsigned long offset) {
    EEPROM_EEBLOCK_R = block;
    EEPROM_EEOFFSET_R = offset;
    return EEPROM_EERDWR_R;
}

inline unsigned long concatFourChars(char* str, int i) {
    return (str[i] | (str[i+1] << 8) | (str[i+2] << 16) | (str[i+3] << 24));
}

void setPassword(char* password){
    int i = 0;
    char* hashedPass = hashPassword(password);
    for (i = 0; i < 8; ++i) {
        eeprom_write(0 , i , concatFourChars(hashedPass, i*4));
    }
    free(hashedPass);
}

bool checkPassword(char* password)
{
    int i = 0;
    char* hashedPass = hashPassword(password);
    for (i = 0; i < 8; ++i) {
        if (concatFourChars(hashedPass, i*4) != eeprom_read(0, i)) {
            free(hashedPass);
            return false;
        }
    }
    free(hashedPass);
    return true;
}
