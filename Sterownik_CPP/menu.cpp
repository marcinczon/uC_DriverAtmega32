/*
 * menu.cpp
 *
 * Created: 2012-07-13 11:46:06
 *  Author: Marcin
 *
 */ 
#include "menu.h"
#include <avr/eeprom.h>
//*****************************************************//
//**********************TABLICE************************//
//*****************************************************//

//Zmienne zapisane w EEPROM
//*******Komendy do USART********
char EEPROM_USART_Command_Relay[14][10] __attribute__((section(".eeprom")))=
{
	// 0 Funkcja
	"[Relay]",
	// 1 Dane
	"(1-On)",
	// 2 Dane
	"(2-On)",
	// 3 Dane
	"(3-On)",
	// 4 Dane
	"(4-On)",
	// 5 Dane
	"(1-Off)",
	// 6 Dane
	"(2-Off)",
	// 7 Dane
	"(3-Off)",
	// 8 Dane
	"(4-Off)",
	// 9 Dane
	"(ModFull)",
	// 10 Dane
	"(ModSemi)",
	// 11 Dane
	"(ModManu)",
	// 12 Dane
	"(All-Off)",
	// 13 Dane
	"(All-On)",
};
char EEPROM_USART_Command_Time[2][10] __attribute__((section(".eeprom")))=
{
	// 0 Funkcja
	"[TimeSet]",
	// 1 Funkcja
	"[Time]",
};
char EEPROM_USART_Command_PWM[2][10] __attribute__((section(".eeprom")))=
{
	// 0 Funkcja
	"[PWM]",
	// 1 Funkcja
	
};


//*******Text Menu Zapisuwany do Pamieci EEPROM (max 1024 byte)*********
//Glowne menu
//Pamietaj o przecinkach!!!
//Tablica opcji, pierwsza liczba = poziomy opcji podstawowych+1

char EEPROM_Menu_Opcje[6][16] __attribute__((section(".eeprom"))) =
{
	"1 Time       :",
	"2 Relay      :",
	"3 Led        :",
	"4 Temp.sensor:",
	"5 Uart       :",
	"6 Exit       :",
};
//Ustawienie Czasu
char EEPROM_Pod_Menu_Opcje_1[2][16] __attribute__((section(".eeprom"))) =
{
	"1 Set       :",
	"2 Exit      :",

};
//Ustawianie przekaznikow
char EEPROM_Pod_Menu_Opcje_2[6][16] __attribute__((section(".eeprom"))) =
{
	"1 Set Time  :",
	"2 Set State :",
	"3 Set Mode  :",
	"4 Inf. Time :",
	"5 Reset     :",
	"6 Exit      :",
};
//PWM/LED
char EEPROM_Pod_Menu_Opcje_3[8][16] __attribute__((section(".eeprom"))) =
{
	"1 Led On    :",
	"2 Led Off   :",
	"3 Led D.Time:",
	"4 Led N.Time:",
	"5 Led Color :",
	"6 Led A/M   :",
	"7 Led Ran.  :",
	"8 Exit      :",
};
//Ustawianie temperatury
char EEPROM_Pod_Menu_Opcje_4[6][16] __attribute__((section(".eeprom"))) =
{
	"1 Test DS_1 :",
	"2 Test DS_2 :",
	"3 Find DS_1 :",
	"4 Find DS_2 :",
	"5 Min Max \xdf""C:",
	"6 Exit      :",
};
//Ustawienie UART
char EEPROM_Pod_Menu_Opcje_5[6][16] __attribute__((section(".eeprom"))) =
{
	"1 Uart On   :",
	"2 Uart Off  :",
	"3 Set Speed :",
	"4 Test Rxd  :",
	"5 Test Txd  :",
	"6 Exit      :",
};

//Text Menu Zapisywany w pamieci FLASH
/*
char Menu_Opcje[6][16] =
{
	"1 Time       :",
	"2 Relay      :",
	"3 Led        :",
	"4 Temp.sensor:",
	"5 Uart       :",
	"6 Exit       :",
};								
//Ustawienie Czasu
char Pod_Menu_Opcje_1[2][16] = 
{
	"1 Set       :",
	"2 Exit      :",

};
//Ustawianie przekaznikow
char Pod_Menu_Opcje_2[6][16] = 
{
	"1 Set Time  :",
	"2 Set State :",
	"3 Set Mode  :",
	"4 Inf. Time :",
	"5 Reset     :",
	"6 Exit      :",
};
//PWM/LED
char Pod_Menu_Opcje_3[8][16] =
{
	"1 Led On    :",
	"2 Led Off   :",
	"3 Led D.Time:",
	"4 Led N.Time:",
	"5 Led Color :",
	"6 Led A/M   :",
	"7 Led Ran.  :",
	"8 Exit      :",
};
//Ustawianie temperatury
char Pod_Menu_Opcje_4[6][16] = 
{
	"1 Test DS_1 :",
	"2 Test DS_2 :",
	"3 Find DS_1 :",
	"4 Find DS_2 :",
	"5 Min Max \xdf""C:",
	"6 Exit      :",
};
//Ustawienie UART
char Pod_Menu_Opcje_5[6][16] =
{
	"1 Uart On   :",
	"2 Uart Off  :",
	"3 Set Speed :",
	"4 Test Rxd  :",
	"5 Test Txd  :",
	"6 Exit      :",
};
*/

//*****************************************************//
//**********************OBIEKTY************************//
//*****************************************************//

static Relay Relay_1(4),Relay_2(2),Relay_3(1),Relay_4(0);
static rtc_pcf8583 Zegar;
static Sensor_Ds18b20 Sensor_Ds18b20_No_1(ROM_NO,1,0);
static Sensor_Ds18b20 Sensor_Ds18b20_No_2(ROM_NO,1,9);
static Led Led_RBG;

//static Button_Up ButtonUp;
//static Button_Down ButtonDown;
//static Button_Enter ButtonEnter;
//static Button_Counter ButtonCounter;

//*****************************************************//
//**********************FUNCTION***********************//
//*****************************************************//

//Relay
void Relay_Set_Mode(void)
{	
	int _iRelayMode; //0 - manual, 1 - semi auto, 2 - full auto	
	
	_iRelayMode=Relay_1.GetMode();
	
	write_to_lcd(clear,C);
	while(!Button_Enter_One_Press())
	{
		_iRelayMode=Button_Counter(0,3,_iRelayMode,1);
		x_y(0,0);
		txt("Set Mode:");
		if(_iRelayMode==2)
		{
			x_y(0,1);
			txt("<- Full  Auto ->");			
		}
		if(_iRelayMode==1)
		{
			x_y(0,1);
			txt("<- Semi  Auto ->");	
		}
		if(_iRelayMode==0)
		{
			x_y(0,1);
			txt("<-   Manual   ->");
		}	
		Relay_1.SetMode(_iRelayMode);
		Relay_2.SetMode(_iRelayMode);
		Relay_3.SetMode(_iRelayMode);
		Relay_4.SetMode(_iRelayMode);
		
	}	
	write_to_lcd(clear,C);	
}
/*
void Relay_K1_Set_Time(void)
{
	
	int _iSwich=0;
	bool _fFlaga1=false;
	int _iK1HourOn, _iK1MinOn, _iK1HourOff, _iK1MinOff;
	_iK1HourOn=Relay_1.GetHourOn();
	_iK1MinOn=Relay_1.GetMinOn();
	_iK1HourOff=Relay_1.GetHourOff();
	_iK1MinOff=Relay_1.GetMinOff();
		while(_iSwich!=5)
		{
			_fFlaga1=Button_Enter_One_Press();
			if(_fFlaga1==true)
			{
				_iSwich++;
				write_to_lcd(clear,C);
			}
			switch(_iSwich)
			{
				case 1:
						x_y(0,0);
						txt("K1 Hour On");
						_iK1HourOn=Button_Counter(0,23,_iK1HourOn);
						sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK1HourOn,_iK1MinOn,_iK1HourOff,_iK1MinOff);
						x_y(0,1);
						txt(g_cTextTab);				
				break;
				case 2:
						x_y(0,0);
						txt("K1 Min On");
						_iK1MinOn=Button_Counter(0,59,_iK1MinOn);
						sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK1HourOn,_iK1MinOn,_iK1HourOff,_iK1MinOff);
						x_y(0,1);
						txt(g_cTextTab);
				break;
				case 3:
						x_y(0,0);
						txt("K1 Hour Off");
						_iK1HourOff=Button_Counter(0,23,_iK1HourOff);
						sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK1HourOn,_iK1MinOn,_iK1HourOff,_iK1MinOff);
						x_y(0,1);
						txt(g_cTextTab);				
				break;
				case 4:
						x_y(0,0);
						txt("K1 Min Off");
						_iK1MinOff=Button_Counter(0,59,_iK1MinOff);
						sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK1HourOn,_iK1MinOn,_iK1HourOff,_iK1MinOff);
						x_y(0,1);
						txt(g_cTextTab);
				break;				
			}
		}
		_iSwich=0;
		
		write_to_lcd(clear,C);
		sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK1HourOn,_iK1MinOn,_iK1HourOff,_iK1MinOff);
		Relay_1.SetTime(_iK1HourOn,_iK1MinOn,_iK1HourOff,_iK1MinOff);
		x_y(0,0);
		txt(g_cTextTab);
		_delay_ms(3000);
		write_to_lcd(clear,C);
}
void Relay_K2_Set_Time(void)
{
	
	int _iSwich=0;
	bool _fFlaga1=false;
	int _iK2HourOn, _iK2MinOn, _iK2HourOff, _iK2MinOff;
	_iK2HourOn=Relay_2.GetHourOn();
	_iK2MinOn=Relay_2.GetMinOn();
	_iK2HourOff=Relay_2.GetHourOff();
	_iK2MinOff=Relay_2.GetMinOff();
	while(_iSwich!=5)
	{
		_fFlaga1=Button_Enter_One_Press();
		if(_fFlaga1==true)
		{
			_iSwich++;
			write_to_lcd(clear,C);
		}
		switch(_iSwich)
		{
			case 1:
					x_y(0,0);
					txt("K2 Hour On");
					_iK2HourOn=Button_Counter(0,23,_iK2HourOn);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK2HourOn,_iK2MinOn,_iK2HourOff,_iK2MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
			case 2:
					x_y(0,0);
					txt("K2 Min On");
					_iK2MinOn=Button_Counter(0,59,_iK2MinOn);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK2HourOn,_iK2MinOn,_iK2HourOff,_iK2MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
			case 3:
					x_y(0,0);
					txt("K2 Hour Off");
					_iK2HourOff=Button_Counter(0,23,_iK2HourOff);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK2HourOn,_iK2MinOn,_iK2HourOff,_iK2MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
			case 4:
					x_y(0,0);
					txt("K2 Min Off");
					_iK2MinOff=Button_Counter(0,59,_iK2MinOff);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK2HourOn,_iK2MinOn,_iK2HourOff,_iK2MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
		}
	}
	_iSwich=0;
	
	write_to_lcd(clear,C);
	sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK2HourOn,_iK2MinOn,_iK2HourOff,_iK2MinOff);
	Relay_2.SetTime(_iK2HourOn,_iK2MinOn,_iK2HourOff,_iK2MinOff);
	x_y(0,0);
	txt(g_cTextTab);
	_delay_ms(3000);
	write_to_lcd(clear,C);
}
void Relay_K3_Set_Time(void)
{
	
	int _iSwich=0;
	bool _fFlaga1=false;
	int _iK3HourOn, _iK3MinOn, _iK3HourOff, _iK3MinOff;
	_iK3HourOn=Relay_3.GetHourOn();
	_iK3MinOn=Relay_3.GetMinOn();
	_iK3HourOff=Relay_3.GetHourOff();
	_iK3MinOff=Relay_3.GetMinOff();
	while(_iSwich!=5)
	{
		_fFlaga1=Button_Enter_One_Press();
		if(_fFlaga1==true)
		{
			_iSwich++;
			write_to_lcd(clear,C);
		}
		switch(_iSwich)
		{
			case 1:
					x_y(0,0);
					txt("K3 Hour On");
					_iK3HourOn=Button_Counter(0,23,_iK3HourOn);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK3HourOn,_iK3MinOn,_iK3HourOff,_iK3MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
			case 2:
					x_y(0,0);
					txt("K3 Min On");
					_iK3MinOn=Button_Counter(0,59,_iK3MinOn);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK3HourOn,_iK3MinOn,_iK3HourOff,_iK3MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
			case 3:
					x_y(0,0);
					txt("K3 Hour Off");
					_iK3HourOff=Button_Counter(0,23,_iK3HourOff);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK3HourOn,_iK3MinOn,_iK3HourOff,_iK3MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
			case 4:
					x_y(0,0);
					txt("K3 Min Off");
					_iK3MinOff=Button_Counter(0,59,_iK3MinOff);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK3HourOn,_iK3MinOn,_iK3HourOff,_iK3MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
		}
	}
	_iSwich=0;
	
	write_to_lcd(clear,C);
	sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK3HourOn,_iK3MinOn,_iK3HourOff,_iK3MinOff);
	Relay_3.SetTime(_iK3HourOn,_iK3MinOn,_iK3HourOff,_iK3MinOff);
	x_y(0,0);
	txt(g_cTextTab);
	_delay_ms(3000);
	write_to_lcd(clear,C);
}
void Relay_K4_Set_Time(void)
{
	
	int _iSwich=0;
	bool _fFlaga1=false;
	int _iK4HourOn, _iK4MinOn, _iK4HourOff, _iK4MinOff;
	_iK4HourOn=Relay_4.GetHourOn();
	_iK4MinOn=Relay_4.GetMinOn();
	_iK4HourOff=Relay_4.GetHourOff();
	_iK4MinOff=Relay_4.GetMinOff();
	while(_iSwich!=5)
	{
		_fFlaga1=Button_Enter_One_Press();
		if(_fFlaga1==true)
		{
			_iSwich++;
			write_to_lcd(clear,C);
		}
		switch(_iSwich)
		{
			case 1:
					x_y(0,0);
					txt("K4 Hour On");
					_iK4HourOn=Button_Counter(0,23,_iK4HourOn);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK4HourOn,_iK4MinOn,_iK4HourOff,_iK4MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
			case 2:
					x_y(0,0);
					txt("K4 Min On");
					_iK4MinOn=Button_Counter(0,59,_iK4MinOn);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK4HourOn,_iK4MinOn,_iK4HourOff,_iK4MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
			case 3:
					x_y(0,0);
					txt("K4 Hour Off");
					_iK4HourOff=Button_Counter(0,23,_iK4HourOff);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK4HourOn,_iK4MinOn,_iK4HourOff,_iK4MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
			case 4:
					x_y(0,0);
					txt("K4 Min Off");
					_iK4MinOff=Button_Counter(0,59,_iK4MinOff);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK4HourOn,_iK4MinOn,_iK4HourOff,_iK4MinOff);
					x_y(0,1);
					txt(g_cTextTab);
			break;
		}
	}
	_iSwich=0;
	
	write_to_lcd(clear,C);
	sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",_iK4HourOn,_iK4MinOn,_iK4HourOff,_iK4MinOff);
	Relay_4.SetTime(_iK4HourOn,_iK4MinOn,_iK4HourOff,_iK4MinOff);
	x_y(0,0);
	txt(g_cTextTab);
	_delay_ms(3000);
	write_to_lcd(clear,C);
}
*/
void Relay_General_Set_Time(void)
{
	bool _fRelay=true;
	int _iRelayChoose=1;
	char _text[16];	
	
	while (_fRelay==true)
	{
		_iRelayChoose=Button_Counter(0,5,_iRelayChoose,1);
		//_iRelayChoose=ButtonCounter.Count(1,5,_iRelayChoose,1);
		if(_iRelayChoose<1 || _iRelayChoose>4)// Wyjscie z menu ustawiana przekaznikow jezeli wartosc _iRelayChoose czyli podmenu bedize wieksze od 4 albo mniejsze od 1
		{
			_fRelay=false;
		}
		switch(_iRelayChoose)
		{
			case  1:
					x_y(0,0);
					txt("Ex < K1? > K2");
					x_y(0,1);
					sprintf(_text,"%2d:%2d-%2d:%2d",Relay_1.GetHourOn(),Relay_1.GetMinOn(),Relay_1.GetHourOff(),Relay_1.GetMinOff());
					txt(_text);				
					if(Button_Enter_One_Press())
					{
						x_y(0,1);
						Relay_1.ChangeTime("K1");
						
					}
			break;
			
			case  2:
					x_y(0,0);
					txt("K1 < K2? > K3");
					x_y(0,1);
					sprintf(_text,"%2d:%2d-%2d:%2d",Relay_2.GetHourOn(),Relay_2.GetMinOn(),Relay_2.GetHourOff(),Relay_2.GetMinOff());
					txt(_text);
					if(Button_Enter_One_Press())
					{
						Relay_2.ChangeTime("K2");
						
					}			
			break;
			
			case  3:
					x_y(0,0);
					txt("K2 < K3? > K4");
					x_y(0,1);
					sprintf(_text,"%2d:%2d-%2d:%2d",Relay_3.GetHourOn(),Relay_3.GetMinOn(),Relay_3.GetHourOff(),Relay_3.GetMinOff());
					txt(_text);
					if(Button_Enter_One_Press())
					{
						Relay_3.ChangeTime("K3");
					}
			break;
			
			case  4:
					x_y(0,0);
					txt("K3 < K4? > Ex");
					x_y(0,1);
					sprintf(_text,"%2d:%2d-%2d:%2d",Relay_4.GetHourOn(),Relay_4.GetMinOn(),Relay_4.GetHourOff(),Relay_4.GetMinOff());
					txt(_text);
					if(Button_Enter_One_Press())
					{
						Relay_4.ChangeTime("K4");						
					}
			break;
		}
	}
}
void Relay_Set_State(void)
{
	bool _fK1State,_fK2State,_fK3State,_fK4State;
	bool _fRelayCheck=true;
	int _iSwitch=1;
	
	_fK1State=Relay_1.GetState();
	_fK2State=Relay_2.GetState();
	_fK3State=Relay_3.GetState();
	_fK4State=Relay_4.GetState();
	
	while(_fRelayCheck==true)
	{
		_iSwitch=Button_Counter(0,5,_iSwitch,1);
		x_y(0,0);
		txt("Select: ");
		if(_iSwitch<1||_iSwitch>4)
		{
			_fRelayCheck=false;
		}
		switch(_iSwitch)
		{
			case 1:
					x_y(0,1);
					txt("K1");
					
					if(Relay_1.GetState()){x_y(5,1); txt("On ");}
									   else{x_y(5,1); txt("Off");} 
					if(Button_Enter_One_Press())
					{
						//sprawdz_przekazniki_flaga_1=true;
						_fK1State=!_fK1State;
						if(_fK1State==false)
						{
							Relay_1.RelayOff();
							//K1_OFF
							x_y(5,1);
							txt("Off");
						}
						if(_fK1State==true)
						{
							//K1_ON
							Relay_1.RelayOn();
							x_y(5,1);
							txt("On ");
						}
					}
			break;
			
			case 2:
					x_y(0,1);
					txt("K2");
					if(Relay_2.GetState()){x_y(5,1); txt("On ");}
								       else{x_y(5,1); txt("Off");}
					if(Button_Enter_One_Press())
					{
						//sprawdz_przekazniki_flaga_1=true;
						_fK2State=!_fK2State;
						if(_fK2State==false)
						{
							//K2_OFF
							Relay_2.RelayOff();
							x_y(5,1);
							txt("Off");
						}
						if(_fK2State==true)
						{
							//K2_ON
							Relay_2.RelayOn();
							x_y(5,1);
							txt("On ");
						}
					}
			break;
			
			case 3:
					x_y(0,1);
					txt("K3");
					if(Relay_3.GetState()){x_y(5,1); txt("On ");}
									   else{x_y(5,1); txt("Off");}
					if(Button_Enter_One_Press())
					{
						//sprawdz_przekazniki_flaga_1=true;
						_fK3State=!_fK3State;
						if(_fK3State==false)
						{
							Relay_3.RelayOff();
							x_y(5,1);
							txt("Off");
						}
						if(_fK3State==true)
						{
							Relay_3.RelayOn();
							x_y(5,1);
							txt("On ");
						}
					}
			break;
			
			case 4:
					x_y(0,1);
					txt("K4");
					if(Relay_4.GetState()){x_y(5,1); txt("On ");}
									   else{x_y(5,1); txt("Off");}
					if(Button_Enter_One_Press())
					{
						//sprawdz_przekazniki_flaga_1=true;
						_fK4State=!_fK4State;
						if(_fK4State==false)
						{
							Relay_4.RelayOff();
							x_y(5,1);
							txt("Off");
						}
						if(_fK4State==true)
						{
							Relay_4.RelayOn();
							x_y(5,1);
							txt("On ");
						}
					}
			break;
		}
		
	}
	
	
}
void Relay_Time_Info(void)
{
	int _fRelayCheck=1;
	int _iSwitch=1;
	char _text[16];
	
	while(_fRelayCheck==1)
	{
		_iSwitch=Button_Counter(0,5,_iSwitch,1);
		x_y(0,0);
		if(_iSwitch<1||_iSwitch>4)
		{
			_fRelayCheck=0;
		}
		switch(_iSwitch)
		{
			case 1:
					x_y(0,0);
					sprintf(_text,"K1 %2d:%2d-%2d:%2d",Relay_1.GetHourOn(),Relay_1.GetMinOn(),Relay_1.GetHourOff(),Relay_1.GetMinOff());
					txt(_text);

			break;
			case 2:
					x_y(0,0);
					sprintf(_text,"K2 %2d:%2d-%2d:%2d",Relay_2.GetHourOn(),Relay_2.GetMinOn(),Relay_2.GetHourOff(),Relay_2.GetMinOff());
					txt(_text);

			break;
			case 3:
					x_y(0,0);
					sprintf(_text,"K3 %2d:%2d-%2d:%2d",Relay_3.GetHourOn(),Relay_3.GetMinOn(),Relay_3.GetHourOff(),Relay_3.GetMinOff());
					txt(_text);

			break;
			case 4:
					x_y(0,0);
					sprintf(_text,"K4 %2d:%2d-%2d:%2d",Relay_4.GetHourOn(),Relay_4.GetMinOn(),Relay_4.GetHourOff(),Relay_4.GetMinOff());
					txt(_text);

			break;
		}
		
	}
	
	
}
void Relay_Time_Reset(void)
{		
		Relay_1.SetTimes(10,0,20,0);
		Relay_2.SetTimes(10,30,19,30);
		Relay_3.SetTimes(8,0,22,0);
		Relay_4.SetTimes(2,0,23,0);
				
		x_y(0,0);
		txt("Reset Time");
		_delay_ms(255);
		write_to_lcd(clear,C);
}
void Relay_Show_Status(void)
{
	//wdt_reset();
	
	if(Relay_1.GetState())
	{
		x_y(0,0);
		txt("K1 On    ");
	}
	else
	{
		x_y(0,0);
		txt("K1 Off   ");
	}
	
	if(Relay_2.GetState())
	{
		x_y(9,0);
		txt("K2 On    ");
	}
	else
	{
		x_y(9,0);
		txt("K2 Off   ");
	}
	
	
	if(Relay_3.GetState())
	{
		x_y(0,1);
		txt("K3 On    ");
	}
	else
	{
		x_y(0,1);
		txt("K3 Off   ");
	}
		
	if(Relay_4.GetState())
	{
		x_y(9,1);
		txt("K4 On    ");
	}
	else
	{
		x_y(9,1);
		txt("K4 Off   ");
	}
	
}

//Led
void inline Led_Change_Mod(void)
{
	if(!Button_Enter_One_Press())
	{
		g_fISR_INT1=false;
		while(g_fISR_INT1==false)
		{
			Led_RBG.Change_Mode();
			if(g_fISR_INT1==true)
			continue;
		}
	}
}
void inline Led_Test_Function(void)
{
	if(!Button_Enter_One_Press())
	{
		x_y(0,0);
		txt("Random Color");
		_delay_ms(255);
		g_fISR_INT1=false;
		while(g_fISR_INT1==false)
		{
			Led_RBG.Rand_1(false);
			if(g_fISR_INT1==true)
			continue;
		}
	}
	write_to_lcd(clear,C);
}

//Thermometr
void inline Temperature_Sensor_Test_1(void)
{
		char _text[16];
	
		if(!Button_Enter_One_Press())
		{
			sprintf(_text,"%d:%d-%d:%d",Sensor_Ds18b20_No_1.GetRomNo(0),Sensor_Ds18b20_No_1.GetRomNo(1),Sensor_Ds18b20_No_1.GetRomNo(6),Sensor_Ds18b20_No_1.GetRomNo(7));
			x_y(0,0);
			txt(_text);
	
			g_fISR_INT1=false;
	
			while(g_fISR_INT1==false)
			{
				Sensor_Ds18b20_No_1.ConvertTemperature();
				_delay_ms(750);
				Sensor_Ds18b20_No_1.ReadTemperature();
				Sensor_Ds18b20_No_1.SetX(1);
				Sensor_Ds18b20_No_1.SetY(3);
				Sensor_Ds18b20_No_1.WriteTemperatureOnLcd();
				if(g_fISR_INT1==true) continue;
			}
		}
		write_to_lcd(clear,C);	
}
void inline Temperature_Sensor_Test_2(void)
{	
		char _text[16];
	
		if(!Button_Enter_One_Press())
		{
			sprintf(_text,"%d:%d-%d:%d",Sensor_Ds18b20_No_2.GetRomNo(0),Sensor_Ds18b20_No_2.GetRomNo(1),Sensor_Ds18b20_No_2.GetRomNo(6),Sensor_Ds18b20_No_2.GetRomNo(7));
			x_y(0,0);
			txt(_text);
	
			g_fISR_INT1=false;
	
			while(g_fISR_INT1==false)
			{
				Sensor_Ds18b20_No_2.ConvertTemperature();
				_delay_ms(750);
				Sensor_Ds18b20_No_2.ReadTemperature();
				Sensor_Ds18b20_No_2.SetX(1);
				Sensor_Ds18b20_No_2.SetY(3);
				Sensor_Ds18b20_No_2.WriteTemperatureOnLcd();
		
				if(g_fISR_INT1==true) continue;
			}
}
write_to_lcd(clear,C);
}
void inline Temperature_Sensor_Find_1(void)
{
		int _fSensorIsFound;
		char _text[16];
		
		x_y(0,0);
		txt("Searching Ds1:");
		_fSensorIsFound=OWSearch();
		_delay_ms(250);
		if(_fSensorIsFound==1)
		{
			Sensor_Ds18b20_No_1.SetRomNo(ROM_NO);
			Sensor_Ds18b20_No_1.SetExist(true);
			write_to_lcd(clear,C);
			x_y(0,0);
			txt("Found T1");
			sprintf(_text,"%d:%d-%d:%d",Sensor_Ds18b20_No_1.GetRomNo(0),Sensor_Ds18b20_No_1.GetRomNo(1),Sensor_Ds18b20_No_1.GetRomNo(6),Sensor_Ds18b20_No_1.GetRomNo(7));
			x_y(0,1);
			txt(_text);
		}
		else
		{
			Sensor_Ds18b20_No_1.SetRomNo(ROM_NO);
			Sensor_Ds18b20_No_1.SetExist(false);
			write_to_lcd(clear,C);
			x_y(0,0);
			txt("No found");
		}
		_delay_ms(1000);
}
void inline Temperature_Sensor_Find_2(void)
{
		int _fSensorIsFound;
		char _text[16];
		
		x_y(0,0);
		txt("Searching Ds2:");
		_fSensorIsFound=OWSearch();
		_delay_ms(250);
		if(_fSensorIsFound==1)
		{
			Sensor_Ds18b20_No_2.SetRomNo(ROM_NO);
			Sensor_Ds18b20_No_2.SetExist(true);
			write_to_lcd(clear,C);
			x_y(0,0);
			txt("Found T2");
			sprintf(_text,"%d:%d-%d:%d",Sensor_Ds18b20_No_2.GetRomNo(0),Sensor_Ds18b20_No_2.GetRomNo(1),Sensor_Ds18b20_No_2.GetRomNo(6),Sensor_Ds18b20_No_2.GetRomNo(7));
			x_y(0,1);
			txt(_text);
		}
		else
		{
			Sensor_Ds18b20_No_1.SetRomNo(ROM_NO);
			Sensor_Ds18b20_No_2.SetExist(false);
			write_to_lcd(clear,C);
			x_y(0,0);
			txt("No found");
		}
		_delay_ms(1000);
}
void inline Temperature_Sensor_No_Definied_Function(void)
{
	if(!Button_Enter_One_Press())
	{
		g_fISR_INT1=false;
		while(g_fISR_INT1==false)
		{
			// KOD //
			Sensor_Ds18b20_No_1.SetXY(0,0);
			Sensor_Ds18b20_No_2.SetXY(1,0);
			Sensor_Ds18b20_No_1.MinMaxTemperature();
			Sensor_Ds18b20_No_2.MinMaxTemperature();
			_delay_ms(255);
			if(g_fISR_INT1==true) continue;
		}
	}
}


void Clear_Tab(char buff[])
{
	unsigned int i;
	for(i=0;i<sizeof(buff);i++)
	{
		buff[i]=0x00;
	}
}
//USART
void USART_Instruction_Process(void)
{
	char _znak=0x00;
	
	if(!Bufor_Rx.Empty())
	{
		if(!_start_dane && !_start_funkcja)
		{
			_znak=Bufor_Rx.Pop();
		}
		if(_znak=='[')
		{
			_start_funkcja=true;
			_start_dane=false;
			USARTDataStruct1.funkcja[0]='[';
			_zakres=0;
		}
		if(_znak=='(')
		{
			_start_funkcja=false;
			_start_dane=true;
			USARTDataStruct1.dane[0]='(';
			_zakres=0;
		}
		
		if(_start_funkcja && !_start_dane && !Bufor_Rx.Empty())
		{
			_zakres++;
			if(_zakres>=sizeof(USARTDataStruct1.funkcja)-1)
			{
				USART_Text_ISR("\r\nPrzepelnienie Funkcji");
				Clear_Tab(USARTDataStruct1.funkcja);
				_zakres=0;
				_start_funkcja=false;
			}
			else
			{
				_znak=Bufor_Rx.Pop();
				if(_znak==']')
				{
					_start_funkcja=false;
					USARTDataStruct1.funkcja[_zakres]=']';
					for(unsigned int _j=_zakres+1;_j<sizeof(USARTDataStruct1.funkcja);_j++)
					{
						USARTDataStruct1.funkcja[_j]=0x00;
					}
					_zakres=0;
				}
				else
				{
					USARTDataStruct1.funkcja[_zakres]=_znak;
				}
			}
			
		}
		
		if(_start_dane && !_start_funkcja && !Bufor_Rx.Empty())
		{
			_zakres++;
			if(_zakres>=sizeof(USARTDataStruct1.dane)-1)
			{
				USART_Text_ISR("\r\nPrzepelnienie Danych");
				Clear_Tab(USARTDataStruct1.dane);
				_zakres=0;
				_start_dane=false;
			}
			else
			{
				_znak=Bufor_Rx.Pop();
				if(_znak==')')
				{
					_fCommandExist=true;
					_start_dane=false;
					USARTDataStruct1.dane[_zakres]=')';
					for(unsigned int _j=_zakres+1;_j<sizeof(USARTDataStruct1.dane);_j++)
					{
						USARTDataStruct1.dane[_j]=0x00;
					}
					_zakres=0;
				}
				else
				{
					USARTDataStruct1.dane[_zakres]=_znak;
				}
			}
		}
	}
}
void USART_Command(void)
{
	            char _command[12];
				char _echo[25];
				
				Clear_Tab(_echo);

				
				_fCommandExist=false;		

//Funkcje dotyczace sterowania przekaznikami
//Zamienic na Switch Case?
				eeprom_read_block(_command,EEPROM_USART_Command_Relay[0],10); //Command = [Relay]
				if(!strcmp(USARTDataStruct1.funkcja,_command))
				{
					
				// ***** Zmiana stanu przekaznikow***********
				
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[1],10); //Command = (1-On)
					if(!strcmp(USARTDataStruct1.dane,_command)) 										
					{
						Relay_1.RelayOn();
						if(Relay_1.GetState())
						{					
							sprintf(_echo,"%s%s\r\n",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
							USART_Text_ISR(_echo);
						}
					}
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[2],10); //Command = (2-On)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{
						Relay_2.RelayOn();
						if(Relay_2.GetState())
						{
							sprintf(_echo,"%s%s\r",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
							USART_Text_ISR(_echo);
						}					
					}
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[3],10); //Command = (3-On)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{
						Relay_3.RelayOn();
						if(Relay_3.GetState())
						{
							sprintf(_echo,"%s%s\r",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
							USART_Text_ISR(_echo);
						}				
					}
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[4],10); //Command = (4-On)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{
						Relay_4.RelayOn();
						if(Relay_4.GetState())
						{
							sprintf(_echo,"%s%s\r",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
							USART_Text_ISR(_echo);
						}			
					}
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[5],10); //Command = (1-Off)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{						
						Relay_1.RelayOff();
						if(!Relay_1.GetState())
						{
							sprintf(_echo,"%s%s\r\n",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
							USART_Text_ISR(_echo);	
						}
					}
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[6],10); //Command = (2-Off)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{
						Relay_2.RelayOff();
						if(!Relay_2.GetState())
						{
							sprintf(_echo,"%s%s\r",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
							USART_Text_ISR(_echo);
						}
					}
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[7],10); //Command = (3-Off)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{
						Relay_3.RelayOff();
						if(!Relay_3.GetState())
						{
							sprintf(_echo,"%s%s\r",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
							USART_Text_ISR(_echo);
						}
					}
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[8],10); //Command = (4-Off)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{
						Relay_4.RelayOff();
						if(!Relay_4.GetState())
						{
							sprintf(_echo,"%s%s\r",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
							USART_Text_ISR(_echo);
						}
					}
					
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[12],10); //Command = (All-Off)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{
						Relay_1.RelayOff();
						Relay_2.RelayOff();
						Relay_3.RelayOff();
						Relay_4.RelayOff();
						sprintf(_echo,"%s%s\r",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
						USART_Text_ISR(_echo);						
					}
					
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[13],10); //Command = (All-On)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{
						Relay_1.RelayOn();
						Relay_2.RelayOn();
						Relay_3.RelayOn();
						Relay_4.RelayOn();
						sprintf(_echo,"%s%s\r",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
						USART_Text_ISR(_echo);						
					}
					
					
				// ***** Zmiana trybu aut/semi/manual***********
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[9],10); //Command = (ModFull)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{
							Relay_1.SetMode(2);
							Relay_2.SetMode(2);
							Relay_3.SetMode(2);
							Relay_4.SetMode(2);
							
							sprintf(_echo,"%s%s\r",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
							USART_Text_ISR(_echo);							
					}
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[10],10); //Command = (ModSemi)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{
						    Relay_1.SetMode(1);
						    Relay_2.SetMode(1);
						    Relay_3.SetMode(1);
						    Relay_4.SetMode(1);
							
							sprintf(_echo,"%s%s\r",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
							USART_Text_ISR(_echo);						
					}
					eeprom_read_block(_command,EEPROM_USART_Command_Relay[11],10); //Command = (ModManu)
					if(!strcmp(USARTDataStruct1.dane,_command))
					{						
						    Relay_1.SetMode(0);
						    Relay_2.SetMode(0);
						    Relay_3.SetMode(0);
						    Relay_4.SetMode(0);
							
						   	sprintf(_echo,"%s%s\r\n",USARTDataStruct1.funkcja,USARTDataStruct1.dane);
						   	USART_Text_ISR(_echo);
					}
					
				}

// Time
				eeprom_read_block(_command,EEPROM_USART_Command_Time[0],10); //Command = [TimeSet]
				if(!strcmp(USARTDataStruct1.funkcja,_command))
				{
					int _hour,_min,_sec;
					char _buff[2];					
					if((USARTDataStruct1.dane[3]==':')&&(USARTDataStruct1.dane[6]==':'))
					{
							_buff[0]=USARTDataStruct1.dane[1];
							_buff[1]=USARTDataStruct1.dane[2];
							
							_hour=atoi(_buff);
							
							_buff[0]=USARTDataStruct1.dane[4];
							_buff[1]=USARTDataStruct1.dane[5];
							
							_min=atoi(_buff);
							
							_buff[0]=USARTDataStruct1.dane[7];
							_buff[1]=USARTDataStruct1.dane[8];
							
							_sec=atoi(_buff);
							
							Zegar.SaveTime(_hour,_min,_sec);
							
							sprintf(_echo,"%s(%d:%d:%d)\r",_command,Zegar.GetHour(),Zegar.GetMin(),Zegar.GetSec());

							USART_Text_ISR(_echo);
					}					
				}				
//PWM
				eeprom_read_block(_command,EEPROM_USART_Command_PWM[0],10); //Command = [TimeSet]
				if(!strcmp(USARTDataStruct1.funkcja,_command))
				{
					int _OCR0,_OCR1A,_OCR2;
					int _OCR0P,_OCR1AP,_OCR2P; //Odczyt w procentach i przeliczenie na wartosc od 0 do 255
					char _buff[2];
					
					if((USARTDataStruct1.dane[3]==':')&&(USARTDataStruct1.dane[6]==':'))
					{
							_buff[0]=USARTDataStruct1.dane[1];
							_buff[1]=USARTDataStruct1.dane[2];
					
							_OCR0P=atoi(_buff);
					
							_buff[0]=USARTDataStruct1.dane[4];
							_buff[1]=USARTDataStruct1.dane[5];
					
							_OCR1AP=atoi(_buff);
					
							_buff[0]=USARTDataStruct1.dane[7];
							_buff[1]=USARTDataStruct1.dane[8];
					
							_OCR2P=atoi(_buff);	
						
							_OCR0=(255*_OCR0P)/100;
							_OCR1A=(255*_OCR1AP)/100;
							_OCR2=(255*_OCR2P)/100;
					
							Led_RBG.Change_Color_UART(_OCR0,_OCR1A,_OCR2);					
										
							sprintf(_echo,"%s(%d:%d:%d)\r",_command,_OCR0P,_OCR1AP,_OCR2P);

							USART_Text_ISR(_echo);
					}
					
				}



}

//*****************************************************//
//**********************FUNKCJE_MENU*******************//
//*****************************************************//
// Funkcje wyswietlania i wyboru funkcji z opcji
void Sub_Menu_Function_1(int _iNumberOfSubMenu)
{
	switch(_iNumberOfSubMenu)
	{
		case 0:
				write_to_lcd(clear,C);
				Zegar.SetTime();
		break;		
	}
}
void Sub_Menu_Function_2(int _iNumberOfSubMenu)
{
	switch(_iNumberOfSubMenu)
	{
		case 0:
				write_to_lcd(clear,C);
				Relay_General_Set_Time();
		break;
		case 1:
				write_to_lcd(clear,C);
				Relay_Set_State();
		break;
		case 2:
				write_to_lcd(clear,C);
				Relay_Set_Mode();			
		break;
		case 3:
				write_to_lcd(clear,C);
				Relay_Time_Info();
		break;
		case 4:
				write_to_lcd(clear,C);
				Relay_Time_Reset();
		break;

	}
}
void Sub_Menu_Function_3(int _iNumberOfSubMenu)
{
	switch(_iNumberOfSubMenu)
	{
		case 0:
				write_to_lcd(clear,C);
				Led_RBG.On();
				x_y(0,0);
				txt("Led On ");
				_delay_ms(500);
		break;
		case 1:
				write_to_lcd(clear,C);
				Led_RBG.Off();
				x_y(0,0);
				txt("Led Off");
				_delay_ms(500);
		break;
		case 2:
				write_to_lcd(clear,C);
				Led_RBG.Set_Time_Day();
			
		break;
		case 3:
				write_to_lcd(clear,C);
				Led_RBG.Set_Time_Night();

		break;
		case 4:
				write_to_lcd(clear,C);
				Led_RBG.Change_Color();
		break;
		case 5:
				write_to_lcd(clear,C);
				//inline
				Led_Change_Mod();
			
		break;
		case 6:
				write_to_lcd(clear,C);					
			   //Zrobione zabezpieczeni aby nie trzeba bylo wciskac guzik dopuki funnkcja sie nie skonczy,														
			   //Jest ona automatycznie konczona poprzez przerwanie , flaga jest setowana i sprawdzana w tej funkcji, jezeli jest TRUE to idzie do continue i wychodzi z funkcji.		
				Led_Test_Function();													
		break;

	}
}
void Sub_Menu_Function_4(int _iNumberOfSubMenu)
{
	switch(_iNumberOfSubMenu)
	{								//Zostalo zmienione
		case 0:
				write_to_lcd(clear,C);
				Temperature_Sensor_Test_1();				
		break;
		case 1:					
				write_to_lcd(clear,C);
				Temperature_Sensor_Test_2();					
		break;			
		case 2:
				write_to_lcd(clear,C);
				Temperature_Sensor_Find_1();
				
		break;		
		case 3:
				write_to_lcd(clear,C);
				Temperature_Sensor_Find_2();
		break;
		case 4:
				write_to_lcd(clear,C);
				Temperature_Sensor_No_Definied_Function();
		break;
	}
}
void Sub_Menu_Function_5(int _iNumberOfSubMenu)
{
	switch(_iNumberOfSubMenu)
	{
		case 0:
		break;
		case 1:
		break;
		case 2:
		break;
		case 3:
		break;
		case 4:
		break;
	}
}
void Menu_Function(int _iNumberOfMenu, int _iNumberOfSubMenu)
{
	switch(_iNumberOfMenu)
	{
		case 0:
		Sub_Menu_Function_1(_iNumberOfSubMenu);
		break;
		case 1:
		Sub_Menu_Function_2(_iNumberOfSubMenu);
		break;
		case 2:
		Sub_Menu_Function_3(_iNumberOfSubMenu);
		break;
		case 3:
		Sub_Menu_Function_4(_iNumberOfSubMenu);
		break;
	}
	
}

void Show_Sub_Menu(void)
{
	char _text[16];
	switch(g_iMenu)
	{
		case 0:				
				//EEPROM
				x_y(0,0);
				eeprom_read_block(_text,EEPROM_Menu_Opcje[g_iMenu],16);
				txt(_text);
				
				x_y(0,1);
				eeprom_read_block(_text,EEPROM_Pod_Menu_Opcje_1[g_iSubMenu],16);
				txt(_text);
				
				x_y(13,1);
				txt("<--");
				
				
				//FLASH
				//x_y(0,0);
				//txt(&Menu_Opcje[g_iMenu][0]);
				
				//x_y(0,1);
				//txt(&Pod_Menu_Opcje_1[g_iSubMenu][0]);
				
				//x_y(13,1);
				//txt("<--");

		break;
		case 1:		
				//EEPROM
				x_y(0,0);
				eeprom_read_block(_text,EEPROM_Menu_Opcje[g_iMenu],16);
				txt(_text);
				
				x_y(0,1);
				eeprom_read_block(_text,EEPROM_Pod_Menu_Opcje_2[g_iSubMenu],16);
				txt(_text);
				
				x_y(13,1);
				txt("<--");
				
				//FLASH
				//x_y(0,0);
				//txt(&Menu_Opcje[g_iMenu][0]);
				
				//x_y(0,1);
				//txt(&Pod_Menu_Opcje_2[g_iSubMenu][0]);
				
				//x_y(13,1);
				//txt("<--");
		break;
		case 2:							
				//EEPROM
				x_y(0,0);
				eeprom_read_block(_text,EEPROM_Menu_Opcje[g_iMenu],16);
				txt(_text);
				
				x_y(0,1);
				eeprom_read_block(_text,EEPROM_Pod_Menu_Opcje_3[g_iSubMenu],16);
				txt(_text);
				
				x_y(13,1);
				txt("<--");
				
				//FLASH
				//x_y(0,0);
				//txt(&Menu_Opcje[g_iMenu][0]);
				
				//x_y(0,1);
				//txt(&Pod_Menu_Opcje_3[g_iSubMenu][0]);
				
				//x_y(13,1);
				//txt("<--");

		break;
		case 3:	
				//EEPROM
				x_y(0,0);
				eeprom_read_block(_text,EEPROM_Menu_Opcje[g_iMenu],16);
				txt(_text);
				
				x_y(0,1);
				eeprom_read_block(_text,EEPROM_Pod_Menu_Opcje_4[g_iSubMenu],16);
				txt(_text);
				
				x_y(13,1);
				txt("<--");
				
				//FLASH
				//x_y(0,0);
				//txt(&Menu_Opcje[g_iMenu][0]);
				
				//x_y(0,1);
				//txt(&Pod_Menu_Opcje_4[g_iSubMenu][0]);
				
				//x_y(13,1);
				//txt("<--");

		break;
		case 4:			
				//EEPROM
				x_y(0,0);
				eeprom_read_block(_text,EEPROM_Menu_Opcje[g_iMenu],16);
				txt(_text);
				
				x_y(0,1);
				eeprom_read_block(_text,EEPROM_Pod_Menu_Opcje_5[g_iSubMenu],16);
				txt(_text);
				
				x_y(13,1);
				txt("<--");
				
				//FLASH
				//x_y(0,0);
				//txt(&Menu_Opcje[g_iMenu][0]);
				
				//x_y(0,1);
				//txt(&Pod_Menu_Opcje_5[g_iSubMenu][0]);
				
				//x_y(13,1);
				//txt("<--");

		break;
	}
	
	/*
				x_y(0,0);
				txt(&Menu_Opcje[g_iMenu][0][0]);
				x_y(0,1);
				txt(&Menu_Opcje[g_iMenu][g_iSubMenu][0]);
				x_y(13,1);
				txt("<--");
	*/
}
void Selection_Sub_Menu(void)
{	
	bool _fFlaga1=false;
	
	switch(g_iMenu)
	{
		case 0:
			g_iPoziomSubMenu=1;
		break;
		case 1:
			g_iPoziomSubMenu=5;
		break;
		case 2:
			g_iPoziomSubMenu=7;
		break;
		case 3:
			g_iPoziomSubMenu=5;
		break;
		case 4:
			g_iPoziomSubMenu=5;
		break;
	}
	

	while(g_fSubMenu==0)
	{
		
	// Wyswietlanie menu w submenu
		Show_Sub_Menu();
		
	// Sprawdzanie przyciskow i zmiana parametrow
		_fFlaga1 = Button_Up_One_Press();
		if(_fFlaga1==true)
		{
			write_to_lcd(clear,C);
			g_iSubMenu++;	 if(g_iSubMenu > g_iPoziomSubMenu) g_iSubMenu=0;
		}	

		_fFlaga1 = Button_Down_One_Press();
		if(_fFlaga1==true)
		{
			write_to_lcd(clear,C);
			g_iSubMenu--;	if(g_iSubMenu < 0) g_iSubMenu = g_iPoziomSubMenu; 
		}	
		

		_fFlaga1 = Button_Enter_One_Press();
		if(_fFlaga1==true)
		{
			if(g_iSubMenu==g_iPoziomSubMenu)									//IF ENTER >> EXIT
			{
				write_to_lcd(clear,C);
				x_y(0,0);
				g_fSubMenu=true;			
				write_to_lcd(clear,C);
				continue;
			}	
			else																//IF ENTER GO TO Funkcje menu glownego
			{
				write_to_lcd(clear,C);
				Menu_Function(g_iMenu, g_iSubMenu);				
			}		

			
		}
	}
}	
void Show_Menu(void)
{
	char _text[16];
	
	if(g_iMenu==g_iPoziomyMainMenu)
	{
		wdt_disable();
				
		//EEPROM
		x_y(0,0);
		eeprom_read_block(_text,EEPROM_Menu_Opcje[g_iMenu],16);
		txt(_text);
		
		x_y(0,1);
		eeprom_read_block(_text,EEPROM_Menu_Opcje[0],16);
		txt(_text);
		
		x_y(14,0);
		txt("<-");
		
		//FLASH
		//x_y(0,0);
		//txt(&Menu_Opcje[g_iMenu][0]);
		
		//x_y(14,0);
		//txt("<-");
		
		//x_y(0,1);
		//txt(&Menu_Opcje[0][0]);
	}
	else
	{
		wdt_disable();	
		
		//EEPROM
		x_y(0,0);
		eeprom_read_block(_text,EEPROM_Menu_Opcje[g_iMenu],16);
		txt(_text);
		
		x_y(0,1);
		eeprom_read_block(_text,EEPROM_Menu_Opcje[g_iMenu+1],16);
		txt(_text);
		
		x_y(14,0);
		txt("<-");
		
		//FLASH
		//x_y(0,0);
		//txt(&Menu_Opcje[g_iMenu][0]);
		
		//x_y(0,1);
		//txt(&Menu_Opcje[g_iMenu+1][0]);
		
		//x_y(14,0);
		//txt("<-");	

	}
}
void Selection_Menu(void)
{
	while(g_fMenu_1==false)
	{
		Show_Menu();
		wdt_disable();

			if(Button_Up_One_Press())
			{
				write_to_lcd(clear,C);
				g_iMenu++;	if(g_iMenu<0) g_iMenu = g_iPoziomyMainMenu; if(g_iMenu > g_iPoziomyMainMenu) g_iMenu=0;
			}
			if(Button_Down_One_Press())
			{
				write_to_lcd(clear,C);
				g_iMenu--;	if(g_iMenu<0) g_iMenu = g_iPoziomyMainMenu; if(g_iMenu > g_iPoziomyMainMenu) g_iMenu=0;
			}		
			if(Button_Enter_One_Press())
			{
				write_to_lcd(clear,C);
				
				g_iSubMenu=0;
				g_fSubMenu=false;
				
				if(g_iMenu==g_iPoziomyMainMenu)		//Sprawdza czy exit
				{
					g_fMenu_1=true;
					continue;
				}
				else
				{
					Selection_Sub_Menu();
				}							
				
			}		
	}
}

void Main_Loop(void)
{
// Funkcje sprawdzane sa co 1 sekunde
	if(Zegar.CheckSecond())
	{	
		Zegar.ShowTime(3,0);	
		
		Sensor_Ds18b20_No_1.SetXY(1,0);
		Sensor_Ds18b20_No_2.SetXY(1,8);	
		Sensor_Ds18b20_No_1.ReadTemperature();
		Sensor_Ds18b20_No_1.WriteTemperatureOnLcd();		
		Sensor_Ds18b20_No_2.ReadTemperature();
		Sensor_Ds18b20_No_2.WriteTemperatureOnLcd();
		Sensor_Ds18b20_No_2.ConvertTemperature();
		Sensor_Ds18b20_No_1.AllowToRead(true);
		Sensor_Ds18b20_No_2.AllowToRead(true);
		
		Led_RBG.Automatic(Zegar.GetHour(),Zegar.GetMin());
		
		Relay_1.Automatic(Zegar.GetHour(),Zegar.GetMin());
		Relay_2.Automatic(Zegar.GetHour(),Zegar.GetMin());
		Relay_3.Automatic(Zegar.GetHour(),Zegar.GetMin());
		Relay_4.Automatic(Zegar.GetHour(),Zegar.GetMin());
		
	}	

	//Funkcje instrukcji USART
	if(_fCommandExist==true)
	{
		USART_Command();
	}
	else
	{
		USART_Instruction_Process();
	}


	//if(BUTTON_ENTER_OFF)
	//{		
	//	Zegar.ShowTime(3,0);	//Tylko do symulacji w Proteusie
	//}	

	//Czyszczenie ekranu z smieci
	if (BUTTON_ENTER_OFF && g_fMainMenu==true)
	{
		write_to_lcd(clear,C);
		g_fMainMenu=false;
	}
	//Wyswietlanie stanu przekaznikow gdy wcisniety jest przycisk UP
	while(BUTTON_UP_ON)
	{
		USART_Text_ISR("\r\n");
		USART_Text_ISR(USARTDataStruct1.funkcja);
		USART_Text_ISR("");
		USART_Text_ISR(USARTDataStruct1.dane);
		
		Relay_Show_Status();
		if(BUTTON_UP_OFF)
		write_to_lcd(clear,C);
	}
	
	while(BUTTON_DOWN_ON)
	{
		write_to_lcd(clear,C);
		Sensor_Ds18b20_No_1.SetXY(0,0);
		Sensor_Ds18b20_No_2.SetXY(1,0);
		Sensor_Ds18b20_No_1.MinMaxTemperature();
		Sensor_Ds18b20_No_2.MinMaxTemperature();
		_delay_ms(4000);
		write_to_lcd(clear,C);
	}
	//Wchodzenie do menu glownego	
	while(BUTTON_ENTER_ON)
	{		
			wdt_disable();
			_delay_ms(100);
			if(BUTTON_ENTER_OFF && g_fMainMenu==false)
			{
				write_to_lcd(clear,C);
				//Selection_Menu();
				//Show_Menu();
				g_fMainMenu=true;
				g_fMenu_1=false;
				g_iMenu=0;
				g_iSubMenu=0;
				Selection_Menu();												//Bardzo istotne gdzie wywoluje sie t¹ funkcje.
			}	
	}
}

//Inicjalizacje
void Init_Przerwania(void)
{
	//Konfiguracja przerwan od TIMERA
	//TCCR0 |= (1<<CS02) | (1<<CS00);						// Ÿród³em CLK, preskaler 1024
	//TIMSK |= (1<<TOIE0);									//Przerwanie overflow (przepe³nienie timera)
	
	//Konfiguracja przerwan zewnetrznych INT0 i INT1
	MCUCR|=_BV(ISC11)|_BV(ISC10);							//Przerwania generowane beda od stanu opadajacego na pinach INT 0 i 1
	GICR|=_BV(INT1);
	
	//Konfiguracja przerwan UART
	UCSRB|=_BV(RXCIE);//|(1<<UDRIE);//_BV(TXCIE);
	
	sei();
}	
void Init_Button(void)
{
	DDRD&=~_BV(2)&~_BV(3)&~_BV(4);
	PORTD|=_BV(2)|_BV(3)|_BV(4);
}
void Init_Przekazniki(void)
{
	Relay_1.RelayOff();
	Relay_2.RelayOff();
	Relay_3.RelayOff();
	Relay_4.RelayOff();
	Relay_Time_Reset();
}
void Init_Start_Lcd(void)
{
	lcd_init();
	lcd_init();
	x_y(5,0);
	txt("LCD ON");
	_delay_ms(255);
	write_to_lcd(clear,C);
	write_to_lcd(cursor_off,C);
}
void Init_Termometr(void)
{
	int _fSensorIsFound;
	char _text[16];
	
	_fSensorIsFound=OWSearch();
	if(_fSensorIsFound==1)
	{
		Sensor_Ds18b20_No_1.SetRomNo(ROM_NO);
		Sensor_Ds18b20_No_1.SetExist(true);
		sprintf(_text,"%d:%d-%d:%d",Sensor_Ds18b20_No_1.GetRomNo(0),Sensor_Ds18b20_No_1.GetRomNo(1),Sensor_Ds18b20_No_1.GetRomNo(6),Sensor_Ds18b20_No_1.GetRomNo(7));	
		write_to_lcd(clear,C);
		x_y(0,0);
		txt("Found Ds No:");
		x_y(0,1);
		txt(_text);
		sprintf(_text,"%d:%d:%d:%d:%d:%d:%d:%d\r\n",Sensor_Ds18b20_No_1.GetRomNo(0),Sensor_Ds18b20_No_1.GetRomNo(1),Sensor_Ds18b20_No_1.GetRomNo(2),Sensor_Ds18b20_No_1.GetRomNo(3),Sensor_Ds18b20_No_1.GetRomNo(4),Sensor_Ds18b20_No_1.GetRomNo(5),Sensor_Ds18b20_No_1.GetRomNo(6),Sensor_Ds18b20_No_1.GetRomNo(7));
		USART_Text_ISR(_text);
		_delay_ms(250);
		write_to_lcd(clear,C);
	}
	else
	{
		Sensor_Ds18b20_No_1.SetRomNo(ROM_NO);
		Sensor_Ds18b20_No_1.SetExist(false);
		x_y(0,0);
		txt("No Found Ds 1");
		USART_Text_ISR("\r\nNo Ds 1");
		_delay_ms(250);
		write_to_lcd(clear,C);
	}

	_fSensorIsFound=OWSearch();
	if(_fSensorIsFound==1)
	{
		Sensor_Ds18b20_No_2.SetRomNo(ROM_NO);
		Sensor_Ds18b20_No_2.SetExist(true);
		sprintf(_text,"%d:%d-%d:%d",Sensor_Ds18b20_No_2.GetRomNo(0),Sensor_Ds18b20_No_2.GetRomNo(1),Sensor_Ds18b20_No_2.GetRomNo(6),Sensor_Ds18b20_No_2.GetRomNo(7));	
		write_to_lcd(clear,C);
		x_y(0,0);
		txt("Found Ds No:");
		x_y(0,1);
		txt(_text);
		sprintf(_text,"%d:%d:%d:%d:%d:%d:%d:%d\r\n",Sensor_Ds18b20_No_2.GetRomNo(0),Sensor_Ds18b20_No_2.GetRomNo(1),Sensor_Ds18b20_No_2.GetRomNo(2),Sensor_Ds18b20_No_2.GetRomNo(3),Sensor_Ds18b20_No_2.GetRomNo(4),Sensor_Ds18b20_No_2.GetRomNo(5),Sensor_Ds18b20_No_2.GetRomNo(6),Sensor_Ds18b20_No_2.GetRomNo(7));
		USART_Text_ISR(_text);
		_delay_ms(250);
		write_to_lcd(clear,C);
	}
	else
	{
		Sensor_Ds18b20_No_2.SetRomNo(ROM_NO);
		Sensor_Ds18b20_No_2.SetExist(false);
		x_y(0,0);
		txt("No Found Ds 2");	
		USART_Text_ISR("\r\nNo Ds 2");
		_delay_ms(250);
		write_to_lcd(clear,C);
	}
	
	//Wyslanie komendy przetwarzania do DS18b20
	ONE_WIRE_RESET();
	ONE_WIRE_SEND_BYTE(0xCC);
	ONE_WIRE_SEND_BYTE(0x44);	
	
}
void Init_Led(void)
{
	Led_RBG.On();
	//PWM_ON();
	//_delay_ms(255);
	//PWM_OFF();
}
void Inicjalizacje(void)
{
	//LCD
	Init_Start_Lcd();
	//Uart
	USARTInit();
	//PRZYCISKI
	Init_Button();
	//PRZEKAZNIKI
    Init_Przekazniki();
	//TERMOMETR - szukanie czujnikow
	Init_Termometr();
	//PWM-LED
	Init_Led();
	//Przerwania
	Init_Przerwania();
	
}

//Funkcje przerwañ
ISR(INT1_vect)
{
	g_fISR_INT1=true;
}

// Aby dodac nowe menu
//1 Edytowac tablice char, do menu glownego i pod menu
//2 Edytowac Show_Sub_Menu
//3 Edytowac Selection_Sub_Menu -> poziom podmenu
//4 Dodac Funkcje void Funkcje_Pod_Menu_X(int _iNumberOfSubMenu)
//5 Edytowac zmienna g_iPoziomyMainMenu