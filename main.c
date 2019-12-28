#include "tm4c123gh6pm.h"
#include "ps2_keyboard.h"
#include "lcd.h"

#define MAX_PASSWORD_LENGTH 32

char read_keyboard_char_sync() {
    while (1) {
        if (KEYBOARD_availableChars())
            return KEYBOARD_readChar();
    }
}

char* start_enter_password_dialogue() {
    LCD_clear();
    LCD_string("Press [Enter] to");
    LCD_command(0xC0);
    LCD_string("type password");

    while (read_keyboard_char_sync() != '\n');
    LCD_clear();

    int i = 0;
    static char password[33];
    while (1) {
        char c = read_keyboard_char_sync();
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
                if (k == 16)
                    LCD_command(0xC0);
            }

            LCD_data(c);

            password[i] = c;
            i++;
        }
    }
    password[i] = '\0';
    return password;
}

void open_incorrect_passwd_dialogue() {
    LCD_string("Incorrect password!");
    LCD_command(0xC0);
    LCD_string("Press [Enter] to");
    LCD_command(0xC0);
    LCD_string("try again.");

    while (read_keyboard_char_sync() != '\n');
}

int open_on_door_closed_menu() {
    while (1) {
        LCD_clear();
        LCD_string("1. Change password");
        LCD_command(0xC0);
        LCD_string("2. Open door");

        char c = read_keyboard_char_sync();
        if (c == '\n') {
            LCD_clear();
            LCD_string("3. Logout");

            c = read_keyboard_char_sync();
            if (c == '\n')
                continue;
        }

        if (c == '1') {
            char* passwd = start_enter_password_dialogue();
            setPassword(passwd);
        } else if (c == '2') {
            LCD_string("Opening door..");
            door_open();
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

        char c = read_keyboard_char_sync();
        if (c == '1') {
            char* passwd = start_enter_password_dialogue();
            setPassword(passwd);
        } else if (c == '2') {
            LCD_string("Closing door..")
            door_close();
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
    motor_init();
    eeprom_init();

    while (1) {
        char* password = start_enter_password_dialogue();

        int valid_password = checkPassword(password);
        if (valid_password) {
            open_main_menu();
        } else {
            open_incorrect_passwd_dialogue();
        }
    }
}
