#include "twi.h"


//=======================  TWI  ===========================================

void TWI_start(void)
{
	   TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	   while(!(TWCR&(1<<TWINT)));
   
	   if(((TWSR&0xF8)!=0x08)&&((TWSR&0xF8)!=0x10))
	   {
	   }

}

void  TWI_stop(void)
{
	   TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	   while((TWCR&(1<<TWSTO)));

	   if(TWSR!=0xF8)
	   {
	   }

}

void TWI_write(char dane)
{
   TWDR = dane;
   TWCR = (1<<TWINT)|(1<<TWEN);
   while(!(TWCR&(1<<TWINT)));

	if(((TWSR&0xF8)!=0x18)&&((TWSR&0xF8)!=0x28)&&((TWSR&0xf8)!=0x40))
	{
	}


}

unsigned char TWI_read(unsigned char ack)
{
   TWCR =(1<<TWINT)|(ack<<TWEA)|(1<<TWEN);
   while (!(TWCR&(1<<TWINT)));
   
   if(((TWSR & 0xF8)!=0x50)&&((TWSR & 0xF8)!=0x58))
   {
   }
   
   return TWDR;
}





