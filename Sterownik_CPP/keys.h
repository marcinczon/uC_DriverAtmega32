#ifndef keys_H
#define keys_H

//******************************************************//
//**********************BIBLIOTEKI**********************//
//******************************************************//
#include <avr/io.h>
#include "uart.h"
//*****************************************************//
//**********************DEFINICJE**********************//
//*****************************************************//

// Definicja przyciskow czy s¹ za³¹czone
#define BUTTON_DOWN_ON !(PIND & 0b00000100)
#define BUTTON_UP_ON !(PIND & 0b00010000)
#define BUTTON_ENTER_ON !(PIND & 0b00001000)

// Definicja przyciskow czy s¹ wylaczone
#define BUTTON_DOWN_OFF (PIND & 0b00000100)
#define BUTTON_UP_OFF (PIND & 0b00010000)
#define BUTTON_ENTER_OFF (PIND & 0b00001000)

//*****************************************************//
//**********************ZMIENNE************************//
//*****************************************************//
bool g_fButtonOnePress=false;

//*****************************************************//
//**********************FUNKCJE************************//
//*****************************************************//
int Button_Up_One_Press(void);
int Button_Down_One_Press(void);
int Button_Enter_One_Press(void);
int Button_Counter(int,int,int,int);

#endif
