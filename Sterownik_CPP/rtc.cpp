#include "rtc.h"
//*****************************************************//
//**********************ZMIENNE************************//
//*****************************************************//

void ustawianie_zegarka(void)
{
	
	if (BUTTON_ENTER_OFF)
	{
			czas_flaga_1=0;
	}
	if(BUTTON_ENTER_ON && czas_flaga_1==0)
	{
	write_to_lcd(clear,C);
	czas_flaga_1=1;
	while(z!=4)
	{
		if(BUTTON_ENTER_OFF)
		{
			sprawdz=0;
		}		
		if(BUTTON_ENTER_ON&&sprawdz==0)
		{
			z++;
			sprawdz=1;
			write_to_lcd(clear,C);
			k=0;
		}
		switch(z)
		{
				case 1:	
					x_y(0,0);
					txt("Ustaw zegar ");			
					godziny=keys();
					if(godziny>23) godziny=k=0;
					if(godziny<0) godziny=k=23;
					//godziny=k;					
					//dtostrf(godziny,0,0,buf);
					sprintf(buf,"Godzina: %d",godziny);
					x_y(0,1);
					txt(buf);
					
				break;
				case 2:
					x_y(0,0);
					txt("Ustaw zegar");
					minuty=keys();
					if(minuty>59) minuty=k=0;
					if(minuty<0) minuty=k=59;
					//minuty=k;
					sprintf(buf,"Minuta: %d",minuty);
					x_y(0,1);
					txt(buf);
				break;
				case 3:
					x_y(0,0);
					txt("Ustaw zegar");
					sekundy=keys();
					if(sekundy>59) sekundy=k=0;
					if(sekundy<0) sekundy=k=59;
					//sekundy=k;
					sprintf(buf,"Sekundy: %d",sekundy);
					x_y(0,1);
					txt(buf);
				break;
				
			}
		}
		z=1;
		write_to_lcd(clear,C);	
		godziny=decToBcd(godziny);
		EEPROM_write(4,godziny);
		minuty=decToBcd(minuty);
		EEPROM_write(3,minuty);
		sekundy=decToBcd(sekundy);
		EEPROM_write(2,sekundy);
	
		}			
}