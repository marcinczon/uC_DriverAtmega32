#ifndef TWI_H
#define TWI_H
//******************************************************//
//**********************BIBLIOTEKI**********************//
//******************************************************//
#include <avr/io.h>                // dostep do rejestr�w
#include <util/delay.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>
//*****************************************************//
//**********************DEFINICJE**********************//
//*****************************************************//

//*****************************************************//
//**********************ZMIENNE************************//
//*****************************************************//

//*****************************************************//
//**********************FUNKCJE************************//
//*****************************************************//
void TWI_start(void);
void TWI_stop(void);
void TWI_write(char);
unsigned char TWI_read(unsigned char);

#endif
