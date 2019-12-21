#include "delay.h"
#include "tm4c123gh6pm.h"

void delayUs(unsigned int microseconds) {
    int us;
    for (us = 0; us < microseconds; ++us) {
        NVIC_ST_RELOAD_R = 16;
        NVIC_ST_CTRL_R = 5;
        while ((NVIC_ST_CTRL_R & 0x10000) == 0);
        NVIC_ST_CTRL_R = 0;
    }
}

void delayMs(unsigned int milliseconds) {
    int ms;
    for (ms = 0; ms < milliseconds; ++ms) {
        NVIC_ST_RELOAD_R = 16000;
        NVIC_ST_CTRL_R = 5;
        while ((NVIC_ST_CTRL_R & 0x10000) == 0);
        NVIC_ST_CTRL_R = 0;
    }
}
