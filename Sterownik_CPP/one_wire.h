#ifndef termometr_H
#define termometr_H


// http://www.elenota.pl/datasheet_download/155914/AN187 funkcje wykorzystane w programie

//******************************************************//
//**********************BIBLIOTEKI**********************//
//******************************************************//

#include <avr/io.h>                // dostêp do rejestrów
#include <util/delay.h>
#include <string.h>
//#include <stdio.h>

//*****************************************************//
//**********************DEFINICJE**********************//
//*****************************************************//

#define DS (1<<4)
#define TRUE 1
#define FALSE 0

//*****************************************************//
//**********************ZMIENNE************************//
//*****************************************************//


//*****************************************************//
//**********************FUNKCJE************************//
//*****************************************************//

void float_to_str_convert(float , char*);
unsigned char ONE_WIRE_RESET(void);
void ONE_WIRE_SEND_BIT(unsigned char);
unsigned char ONE_WIRE_READ_BIT(void);
void ONE_WIRE_SEND_BYTE(unsigned char);
unsigned char ONE_WIRE_READ_BYTE(void);

#endif
