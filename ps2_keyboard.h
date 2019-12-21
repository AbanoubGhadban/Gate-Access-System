#ifndef PS2_KEYBOARD_H_
#define PS2_KEYBOARD_H_

#include "tm4c123gh6pm.h"

#define BUFFER_SIZE 20
#define TIMEOUT 250
#define CLK_PIN (1U << 0)
#define CLK_DATA GPIO_PORTF_DATA_R
#define DATA_PIN (1U << 4)
#define DATA_DATA GPIO_PORTF_DATA_R

void KEYBOARD_init(void);
int KEYBOARD_availableChars(void);
char KEYBOARD_readChar(void);

#endif /* PS2_KEYBOARD_H_ */
