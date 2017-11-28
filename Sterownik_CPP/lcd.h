#ifndef lcd_H
#define lcd_H
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>


void write_to_lcd(unsigned char, unsigned char);
void busy(void);
void lcd_init(void);
void x_y(unsigned char, unsigned char );
void txt(const char *t);

#endif
