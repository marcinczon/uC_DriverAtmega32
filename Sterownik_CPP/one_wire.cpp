#include "one_wire.h"

///////////////////////////////////////////////////////////////////////////////
//*********************************1Wire*************************************//
///////////////////////////////////////////////////////////////////////////////

unsigned char ONE_WIRE_RESET(void) 
{ 
 	  DDRC|=(DS);
     PORTC&=~(DS);
     _delay_us(250);
     _delay_us(250);
     PORTC|=DS;
	 DDRC&=~(DS);
     _delay_us(30);
     if(PINC&(DS))  return 0;
     else
	 {
	 _delay_us(250);
     _delay_us(250);              
	 return 1;
	 }
 } 
void ONE_WIRE_SEND_BIT(unsigned char bit) 
{ 

	 PORTC &= ~DS; 
	 DDRC |= DS;       // pin jako wyjœcie 
	 _delay_us(2); 
	 if(bit) 
	 { 
		 PORTC |= DS;      
	 } 
	 _delay_us(60); 
	 DDRC &= ~DS;       // pin jako wejscie 

} 
unsigned char ONE_WIRE_READ_BIT(void) 
{ 

   unsigned char tmp; 
   PORTC &= ~DS; 
   DDRC |= DS;       // pin jako wyjœcie 
   _delay_us(3); 
    DDRC &= ~DS;       // pin jako wejscie 
   _delay_us(3); 
	if(PINC & DS)	tmp=1;
	else	tmp=0;
   _delay_us(55); 
   return tmp;

} 
void ONE_WIRE_SEND_BYTE(unsigned char byte) 
{ 
  unsigned char i; 
  
  for( i = 0; i<8; i++) 
  { 
     ONE_WIRE_SEND_BIT(byte & 1<<i);
     _delay_us(2); 
  } 
} 
unsigned char ONE_WIRE_READ_BYTE(void) 
{ 
   unsigned char i,k=0; 
    
   for(i=0; i<8; i++) 
   { 
      if(ONE_WIRE_READ_BIT()) k|= 1<<i; 
   } 
  return k; 
}





