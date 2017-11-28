#include "Sterownik_CPP.h"

int main(void)
{    	
	Inicjalizacje();	
	USART_Text_ISR("\r\nUART OK");


	//wdt_enable( WDTO_2S );
	while(1)
	{						
		;Main_Loop();	

	    if(g_fMenu_1==0)
		{
			//wdt_disable();
		}
		else
		{
			//wdt_enable( WDTO_2S );
		}
		//wdt_reset();							//Watchdog Reset
		
	}
}





