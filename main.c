#include "tm4c123gh6pm.h"
#include "ps2_keyboard.h"
#include "lcd.h"

#define MAX_PASSWORD_LENGTH 32

/* ------------- UNUSED ------------- */
int do_handle_keyboard_input_on_lcd(char c) {
    static int lcd_cursor_pos = 0;

    if (c == 0x66) {
        // User Pressed Backspace
        LCD_command(0x10);
        lcd_cursor_pos--;
    } else if (c == '\n') {
        // User Pressed Enter
        if (lcd_cursor_pos <= 16) {
            LCD_command(0xC0); // Make LCD cursor move to the second line
            lcd_cursor_pos = 16;
        } else {
            LCD_clear();
            lcd_cursor_pos = 0;
        }
    } else {
        if (lcd_cursor_pos == 16) {
            LCD_command(0xC0); // Make LCD cursor move to the second line
        } else if (lcd_cursor_pos == 32) {
            LCD_clear();
            lcd_cursor_pos = 0;
        }
        LCD_data(c);
        lcd_cursor_pos++;
        return 1;
    }
    return 0;
}

char read_keyboard_input_sync() {
    while (1) {
        if (KEYBOARD_availableChars())
            return KEYBOARD_readChar();
    }
}

char* open_enter_password_dialogue() {
    LCD_clear();
    LCD_string("Press [Enter] to type password");
    while (read_keyboard_input_sync() != '\n');
    LCD_clear();

    int i = 0;
    static char password[33];
    while (1) {
        char c = read_keyboard_input_sync();
        if (c == '\n')
            break;

        if (i == MAX_PASSWORD_LENGTH)
            continue;

        if (c >= ' ' && c <= '~') {
            LCD_clear();

            int k = 0;
            while (k != i) {
                LCD_data('*');
                k++;
            }

            LCD_data(c);

            password[i] = c;
            i++;
        }
    }
    password[i] = '\0';
    return password;
}

void show_incorrect_passwd_dialogue() {
    LCD_string("Incorrect password!");
    LCD_command(0xC0);
    LCD_string("Press [Enter] to try again.");

    while (read_keyboard_input_sync() != '\n');
}

int open_on_door_closed_menu() {
    while (1) {
        LCD_clear();
        LCD_string("1. Change password");
        LCD_command(0xC0);
        LCD_string("2. Open door");

        char c = read_keyboard_input_sync();
        if (c == '\n') {
            LCD_clear();
            LCD_string("3. Logout");
        }
        c = read_keyboard_input_sync();
        if (c == '\n')
            continue;

        if (c == '1') {
            char* passwd = open_enter_password_dialogue();
            // TODO: save the password to the EEPROM
        } else if (c == '2') {
            return 0;
        } else if (c == '3') {
            return 1;
        }
    }
}

void open_main_menu() {
    while (1) {
        LCD_clear();
        LCD_string("1. Change password");
        LCD_command(0xC0);
        LCD_string("2. Close door");

        char c = read_keyboard_input_sync();
        if (c == '1') {
            char* passwd = open_enter_password_dialogue();
            // TODO: save the password to the EEPROM
        } else if (c == '2') {
            int return_code = open_on_door_closed_menu();
            if (return_code == 0) {
                continue;
            } else {
                break;
            }
        }
    }
}

int main(void)
{
    KEYBOARD_init();
    LCD_init();
    LCD_start();

    while (1) {
        char* password = open_enter_password_dialogue();

        int valid_password = 1; // TODO: change with return of call to check_password()
        if (valid_password) {
            open_main_menu();
        } else {
            // TODO: maybe restrict the number of trials here.
            show_incorrect_passwd_dialogue();
        }
    }
}
