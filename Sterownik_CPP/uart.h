/*
 * uart.h
 *
 * Created: 2013-04-11 10:14:55
 *  Author: User
 */ 

#ifndef UART_H_
#define UART_H_

//******************************************************//
//**********************BIBLIOTEKI**********************//
//******************************************************//
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "bufor_cykliczny.cpp"
//*****************************************************//
//**********************DEFINICJE**********************//
//*****************************************************//
#define USART_BAUDRATE 19200
//#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
#define BAUD_PRESCALE (((F_CPU+USART_BAUDRATE*8UL)/(16UL*USART_BAUDRATE))-1)

//*****************************************************//
//**********************ZMIENNE************************//
//*****************************************************//

//Potrzebne do przerwania
//volatile int _iCounter=0,_iHelpCounter=0;
//volatile bool _fInstrukcja=false,_fError=false,_fStart=false, _fStop=true;
//char USARTData[20];
 
//int _iFuncStart=0, _iFuncStop=0;
//int _iDataStart=0, _iDataStop=0;

struct USARTDataStruct  
{	
	char funkcja[11];
	char dane[11];
		
}USARTDataStruct1;

//*****************************************************//
//**********************OBIEKTY************************//
//*****************************************************//
static BuforCykliczny  Bufor_Tx;
static BuforCykliczny  Bufor_Rx;

//*****************************************************//
//**********************FUNKCJE************************//
//*****************************************************//

void USARTInit(void);
char USART_Read_Char(void);
void USART_Write_Char(char);
void USART_Text(const char *);
void USART_Text_ISR(const char *);
void USART_Instruction_Process(void);



#endif /* UART_H_ */