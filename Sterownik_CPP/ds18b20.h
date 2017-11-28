#ifndef ds18b20_H
#define ds18b20_H

//******************************************************//
//**********************BIBLIOTEKI**********************//
//******************************************************//

#include "convert.h"
#include "one_wire.h"

//*****************************************************//
//**********************DEFINICJE**********************//
//*****************************************************//


//*****************************************************//
//**********************ZMIENNE************************//
//*****************************************************//

//Numery ROM czujnikow DS18B20

unsigned char 	ROM_NO[8],DS_ID[4][8];
volatile int 	LastDiscrepancy=0,LastDeviceFlag=FALSE,LastFamilyDiscrepancy=0;

//char			DS_1_ROM[8];										//Do starych funkcji szukania czujnikow
//char			DS_2_ROM[8];
//float  		Temperatura_Bufor_Float=0,MSB_float=0,LSB_float=0;
//unsigned int 	LSB=0,MSB=0;
//unsigned char test,cd,act=0;
//char 			Temperatura_Bufor_Char[20], bufor_tekstu[20];
//float			Temperatura_Ds_1, Temperatura_Ds_2;

//*****************************************************//
//**********************FUNKCJE************************//
//*****************************************************//

int OWSearch(void);
void STR_CPY(unsigned char);
int OWFirst(void);
int OWNext(void);

//void Automatyczny_odczyt_temperatury(void);
//void Czujnik_1(void);
//void Czujnik_2(void);
//void Ds18B20_Polecenie_Konwersji(void);


#endif
