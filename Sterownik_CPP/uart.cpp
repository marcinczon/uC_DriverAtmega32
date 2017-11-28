#include "uart.h"

void USARTInit(void)
{	
    UBRRH = (BAUD_PRESCALE >> 8);        //wpisanie starszego bajtu
    UBRRL = BAUD_PRESCALE;             //wpisanie mlodszego bajtu
	UCSRC=(1<<URSEL)|(3<<UCSZ0);  
    UCSRB=(1<<RXEN)|(1<<TXEN);
}
char USART_Read_Char()
{
   while(!(UCSRA & (1<<RXC)));
   return UDR;
}

void USART_Write_Char(char data)
{
   while(!(UCSRA & (1<<UDRE)));
   UDR=data;
}

void USART_Text(const char *dana)
{
	int dlugosc;
	int k=0;
		dlugosc=strlen(dana);

		for(k=0;k<dlugosc;k++)
		{
			USART_Write_Char(dana[k]);
		}
}

void USART_Text_ISR(const char *dana)
{
	int dlugosc;
	int k=0;
	dlugosc=strlen(dana);

	for(k=0;k<dlugosc;k++)
	{
		Bufor_Tx.Push(dana[k]);
	}
	UCSRB|=_BV(UDRIE);
}

ISR(USART_UDRE_vect)
{
	if(!Bufor_Tx.Empty())
	Bufor_Tx.Pop_UDR();
	else
	UCSRB &= ~(1<< UDRIE);
}

ISR(USART_RXC_vect)
{
	Bufor_Rx.Push(UDR);
}


/*
void USART_Instruction_Process(void)
{
	
	for(int _i=0;_i<20;_i++)
	{
		if(USARTData[_i]=='[')
		_iFuncStart=_i;
		
		if(USARTData[_i]==']')
		_iFuncStop=_i;

		if(USARTData[_i]=='(')
		_iDataStart=_i;
		
		if(USARTData[_i]==')')
		_iDataStop=_i;
	}
	
	//Czyszczenie struktory
	for(int _i=0;_i<10;_i++)
	{
		USARTDataStruct1.funkcja[_i]=0x00;
	}	
	for(int _i=0;_i<10;_i++)
	{
		USARTDataStruct1.dane[_i]=0x00;
	}	
	
	//Przepisywanie danych do struktory
	for(int _i=_iFuncStart;_i<=_iFuncStop;_i++)
	{
		USARTDataStruct1.funkcja[_i]=USARTData[_i];
	}
	for(int _i=_iDataStart;_i<=_iDataStop;_i++)
	{
		USARTDataStruct1.dane[_i-_iDataStart]=USARTData[_i];
	}	
}
*/




/*
//Dorobic funkcje parzystosci i sprawdzanie ramki
ISR(USART_RXC_vect)
{
	
	USARTData[_iCounter]=UDR;
	if((USARTData[_iCounter]=='[') && _fStop==true)//&&(RamkaUSART[_iCounter+1]=='*'))
	{		
		for(int _i=0;_i<20;_i++)
		USARTData[_i]=0x00;
		
		for(int _i=0;_i<10;_i++)
		USARTDataStruct1.funkcja[_i]=0x00;
		
		for(int _i=0;_i<10;_i++)
		USARTDataStruct1.dane[_i]=0x00;
		
		USARTData[0]='[';
		_fStart=true;
		_fStop=false;
		_iCounter=0;
		_fInstrukcja=true;
	}
	

	// Jezeli beda znaki konca CR albo LF
	//0x0D  0x0A
	if((USARTData[_iCounter]==')') && (_fStart==true))
	{		
		_iHelpCounter=_iCounter;
		_fStop=true;
		_fStart=false;			
	}	    

	if((_fStart==false) && (_fStop==true) && (_fInstrukcja==false))
	{
		USARTData[_iCounter]=0x00;
		_iCounter=0;
	}
			
	if((_fStop==true)&&(_iCounter>_iHelpCounter))
	{
		USARTData[_iCounter]=0x00;
    }
	
	if(UCSRA & (1<<FE))
	{
		USART_Text("\r\nBład !!");
	}
	_iCounter++;
	if(_iCounter>20)
	_iCounter=0;
	
}

*/











