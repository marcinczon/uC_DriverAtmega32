#include "eeprom.h"

void EEPROM_write(unsigned char adres,unsigned char dane)
{	
		TWI_start();
		TWI_write(0xA0);
		TWI_write(adres);
		TWI_write(dane);
		TWI_stop();
		_delay_ms(15);
}

unsigned char EEPROM_read(unsigned char adres)
{

		unsigned char odczyt;
		TWI_start();
		TWI_write(0xA0);
		TWI_write(adres);
		TWI_start();
		TWI_write(0xA1);
		odczyt=TWI_read(0);
		TWI_stop();
		return odczyt;

}

void EEPROM_write_page(unsigned char adres, int *dane,int ilosc)
{
		int i;
		TWI_start();
		TWI_write(0xA0);
		TWI_write(adres);
		for(i=0;i<ilosc;i++)
		TWI_write(dane[i]);
		TWI_stop();
		_delay_ms(15);
}

void  EEPROM_read_page(unsigned char adres, int *dane, int ilosc)
{
		int i;
		TWI_start();
		TWI_write(0xA0);
		TWI_write(adres);
		TWI_start();
		TWI_write(0xA1);
		for(i=0;i<ilosc;i++)
		dane[i]=TWI_read(1);
		dane[ilosc]=TWI_read(0);
}
