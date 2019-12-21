#include "tm4c123gh6pm.h"
#include "ps2_keyboard.h"
#include "lcd.h"

int main(void)
{
    int i;
    KEYBOARD_init();
    LCD_init();
    LCD_start();

    i = 0;
    while (1) {
        if (KEYBOARD_availableChars()) {
            char c = KEYBOARD_readChar();
            if (c == 0x66) { // User Pressed Backspace
                LCD_command(0x10);
                --i;
            } else if (c == '\n') { // User Pressed Enter
                if (i <= 16) {
                    LCD_command(0xC0); // Make LCD cursor move to the second line
                    i = 16;
                } else {
                    LCD_clear();
                    i = 0;
                }
            } else {
                if (i == 16) {
                    LCD_command(0xC0); // Make LCD cursor move to the second line
                } else if (i == 32) {
                    LCD_clear();
                    i = 0;
                }
                LCD_data(c);
                ++i;
            }


        }
    }
}
