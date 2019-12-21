#include "lcd.h"
#include "delay.h"
#include "tm4c123gh6pm.h"

void LCD_init(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1 | SYSCTL_RCGCGPIO_R4;
    while ((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R1 == 0) || (SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R4 == 0));

    GPIO_PORTE_DEN_R |= (RS | RW | EN);
    GPIO_PORTE_DIR_R |= (RS | RW | EN);
    GPIO_PORTB_DEN_R |= 0xFF;
    GPIO_PORTB_DIR_R |= 0xFF;
}

void LCD_start(void) {
    delayMs(30);
    LCD_command(0x30);
    delayMs(100);
    LCD_command(0x30);
    delayMs(100);
    LCD_command(0x30);
    delayUs(100);

    LCD_command(0x38);
    delayUs(100);
    LCD_command(0x06);
    delayUs(100);
    LCD_command(0x01);
    delayUs(100);
    LCD_command(0x0F);
    delayUs(100);
}

void LCD_command(unsigned char command) {
    LCD_CTRL_DATA = 0;
    LCD_DATA_DATA = command;
    LCD_CTRL_DATA = EN;
    delayUs(1);
    LCD_CTRL_DATA = 0;

    if (command < 4)
        delayMs(2);
    else
        delayUs(40);
}

void LCD_data(unsigned char data) {
    LCD_CTRL_DATA = RS;
    LCD_DATA_DATA = data;
    LCD_CTRL_DATA = EN | RS;
    delayUs(1);
    LCD_CTRL_DATA = 0;
    delayUs(40);
}

void LCD_string(char *c) {
    int i = 0;
    while (c[i] != 0) {
        LCD_data((unsigned char)c[i++]);
    }
}

void LCD_int(int num) {
    int i, j;
    unsigned char str[10];

    if (num == 0) {
        LCD_data('0');
        return;
    }
    if (num < 0)
        LCD_data('-');

    i = 0;
    while (num > 0) {
        str[i] = (unsigned char) ((num % 10) + '0');
        num /= 10;
        ++i;
    }

    for (j = i - 1; j >= 0; --j)
        LCD_data(str[j]);
}

void LCD_clear(void) {
    LCD_command(0x1);
}
