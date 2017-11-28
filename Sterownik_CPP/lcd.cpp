#include "lcd.h"


#define clear				0x01	//Czyszczenie//
#define	cursor_home			0x02	//Kursor na pocz1tek//
#define mode_1				0x07	//Przesuwa sie ca3a zawartooa wyowietlacza w prawo//
#define mode_2				0x05	//Przesuwa sie ca3a zawartooa wyowietlacza w lewo//
#define mode_3				0x06	//Kursor przesuwa sie w prawo//
#define mode_4				0x04	//Kursor przesuwa sie w lewo//
#define ON					0x0C	//W31czenie wyswietlacza//
#define OFF					0x08	//Wy31czenie wyowietlacza//
#define cursor_on			0x0E	//W31czenie kursora//
#define	cursor_off			0x0C	//Wy31czenie kursora//
#define cursor_blink		0x0F	//Miganie kursora//
#define cursor_no_blink		0x0E	//Wy31czenie migania//
#define win_R				0x1C	//Przesowanie zawartooci okna w prawo//
#define win_L				0x18	//Przesowanie zawartooci okna w lewo//
#define cursor_R			0x14	//Przesuwanie kursora w prawo//
#define cursor_L			0x10	//Przesuwanie kursora w lewo//
#define _8_bit				0x38	//Interfejs 8bit, 2 wiersze, 5*7 punktów//
#define C					1
#define D					0

/************************************************************/
#define lcd_port_out	PORTA
#define lcd_port_in		PINA
#define lcd_port_conf	DDRA
#define ctrl_lcd_port	PORTC
#define ctrl_lcd_conf	DDRC
/************************************************************/
/************************************************************/
#define	RS	5	
#define	RW	6
#define	E	7	
/************************************************************/
/************************************************************/
#define lcd_enable		ctrl_lcd_port&=~_BV(E)
#define lcd_disable		ctrl_lcd_port|=_BV(E)
#define lcd_read		ctrl_lcd_port|=_BV(E)
#define lcd_write		ctrl_lcd_port&=~_BV(RW)
#define lcd_cmd			ctrl_lcd_port&=~_BV(RS)
#define lcd_data		ctrl_lcd_port|=_BV(RS)
#define lcd_flag		lcd_port_in&_BV(7)		
/************************************************************/
/************************************************************/
#define clear				0x01	//Czyszczenie//
#define	cursor_home			0x02	//Kursor na pocz1tek//
#define mode_1				0x07	//Przesuwa sie ca3a zawartooa wyowietlacza w prawo//
#define mode_2				0x05	//Przesuwa sie ca3a zawartooa wyowietlacza w lewo//
#define mode_3				0x06	//Kursor przesuwa sie w prawo//
#define mode_4				0x04	//Kursor przesuwa sie w lewo//
#define ON					0x0C	//W31czenie wyswietlacza//
#define OFF					0x08	//Wy31czenie wyowietlacza//
#define cursor_on			0x0E	//W31czenie kursora//
#define	cursor_off			0x0C	//Wy31czenie kursora//
#define cursor_blink		0x0F	//Miganie kursora//
#define cursor_no_blink		0x0E	//Wy31czenie migania//
#define win_R				0x1C	//Przesowanie zawartooci okna w prawo//
#define win_L				0x18	//Przesowanie zawartooci okna w lewo//
#define cursor_R			0x14	//Przesuwanie kursora w prawo//
#define cursor_L			0x10	//Przesuwanie kursora w lewo//
#define _8_bit				0x38	//Interfejs 8bit, 2 wiersze, 5*7 punktów//
/************************************************************/
/************************************************************/
#define C	1
#define D	0
#define CGRAM				0x40
#define DDRAM				0x80

//////////////////////////////////////////////////////////////////////////
//*************************KOD LCD v2***********************************//
//////////////////////////////////////////////////////////////////////////

#define u08	unsigned char

void write_to_lcd(u08 dat, u08 cmd)
{
	lcd_write;
	if(cmd)		lcd_cmd;
	if(!cmd)	lcd_data;
	_delay_ms(2);
	lcd_port_out=dat;
	lcd_enable;
	_delay_ms(2);
	lcd_disable;
}

void busy(void)
{
_delay_ms(5);
/*
	lcd_port_conf=0x00;
	lcd_read;
	lcd_enable;
	while(!lcd_flag);
	lcd_disable;
	lcd_port_conf=0xFF;*/
}

void lcd_init(void)
{
	lcd_port_conf=0xFF;
	lcd_port_out=0xFF;
	ctrl_lcd_conf=_BV(E) | _BV(RS) | _BV(RW);
	ctrl_lcd_port=_BV(E) | _BV(RS) | _BV(RW);
	write_to_lcd(0x3F,C);
	_delay_ms(50);
	write_to_lcd(0x3F,C);
	_delay_ms(50);
	write_to_lcd(0x3F,C);
	_delay_ms(50);
	write_to_lcd(_8_bit,C);
	write_to_lcd(ON,C);
	write_to_lcd(clear,C);
	busy();
	write_to_lcd(cursor_on,C);
	write_to_lcd(cursor_blink,C);
	write_to_lcd(mode_3,C);
}
	
void x_y(u08 x, u08 y)
{
	write_to_lcd((DDRAM | (x+(0x40*y))),C);
}
/*
void txt(char *t)
{	
	for(;;)
	{
		write_to_lcd(*t,D);
		t++;
		if(*t=='\0')	break;
	}
}
*/

void txt(const char *t)
{
	for(;;)
	{
		write_to_lcd(*t,D);
		t++;
		if(*t=='\0')	break;
	}
}
