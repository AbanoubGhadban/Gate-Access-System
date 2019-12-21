#ifndef CAPTURE_TIMER_LCD_H
#define CAPTURE_TIMER_LCD_H

#define LCD_CTRL_DATA GPIO_PORTE_DATA_R
#define LCD_DATA_DATA GPIO_PORTB_DATA_R
#define RS (1U << 0)
#define RW (1U << 1)
#define EN (1U << 2)

void LCD_init(void);
void LCD_start(void);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_string(char *c);
void LCD_int(int num);
void LCD_clear(void);


#endif //CAPTURE_TIMER_LCD_H
