/*
 * led_rgb.h
 *
 * Created: 2013-04-11 10:06:42
 *  Author: User
 */ 

#ifndef LED_RGB_H_
#define LED_RGB_H_

//******************************************************//
//**********************BIBLIOTEKI**********************//
//******************************************************//
#include "keys.h"
#include <avr/io.h>
//*****************************************************//
//**********************DEFINICJE**********************//
//*****************************************************//

//#define r_on PORTB|=_BV(3);
//#define r_off PORTB&=~(1<<3);
//#define g_on PORTD|=_BV(5);
//#define g_off PORTD&=~(1<<5);
//#define b_on PORTD|=_BV(7);
//#define b_off PORTD&=~(1<<7);
//#define init_rbgA DDRD|=_BV(5)|_BV(7);
//#define init_rbgB DDRB|=_BV(3);

//*****************************************************//
//**********************ZMIENNE************************//
//*****************************************************//

//signed int czas_rs=0,czas_bs=0,czas_gs=0;
//signed int dczas_r=0,dczas_b=0,dczas_g=0,dr=0,dg=0,db=0;
//signed int deltar=0,deltag=0,deltab=0;

//char tekst[20];

//*****************************************************//
//**********************FUNKCJE************************//
//*****************************************************//
/*
void PWM_ON(void);
void PWM_OFF(void);
void LED_Dzien(void);
void LED_Random(void);
void LED_Random_2(void);
*/


#endif /* LED_RGB_H_ */