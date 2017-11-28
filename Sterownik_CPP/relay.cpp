/*
 * rtc_pcf8583.cpp
 *
 * Created: 2013-10-28 17:24:28
 *  Author: Marcin
 */ 
#include "relay.h"


class Relay
{
	
	private:
	int _pin;
	int _hours_on, _hours_off, _min_on, _min_off;
	int _mode;
	//int _time_hour,_time_min;
	
	public:
	Relay(int,int,int,int,int,int); //ktore wejscie
	void RelayOff(void);
	void RelayOn(void);
	bool GetState(void);
	int GetHourOn(void);
	int GetHourOff(void);
	int GetMinOn(void);
	int GetMinOff(void);
	void SetTimes(int,int,int,int);
	void Automatic(int,int);
	void ChangeTime(const char *);
	int GetMode(void);
	void SetMode(int);
	
};
Relay::Relay(int _PIN,int Hour_on=12,int Min_on=0,int Hour_off=12,int Min_off=30,int Mode=0)
{
	_pin=_PIN;
	DDRB|=_BV(_PIN);
	_hours_on=Hour_on;
	_hours_off=Hour_off;
	_min_on=Min_on;
	_min_off=Min_off;
	_mode=Mode;
}
void Relay::RelayOff(void)
{
	PORTB&=~_BV(_pin);
}
void Relay::RelayOn(void)
{
	PORTB|=_BV(_pin);
}
bool Relay::GetState(void)
{
	bool _state;
	if(PINB & _BV(_pin))
	_state=true;
	if(!(PINB & _BV(_pin)))
	_state=false;
	return _state;
}
int Relay::GetHourOn(void)
{
	return _hours_on;
}
int Relay::GetHourOff(void)
{
	return _hours_off;
}
int Relay::GetMinOn(void)
{
	return _min_on;
}
int Relay::GetMinOff(void)
{
	return _min_off;
}
int Relay::GetMode(void)
{
	return _mode;
}
void Relay::SetMode(int Mode)
{
	_mode=Mode;
}

void Relay::Automatic(int Time_hours, int Time_min)
{
	if(_mode==2)
	{
		int _iTime,_iOnTime,_iOffTime;
		//Przeliczenia czasu, dla latwiejszego porownywania : Time=(Hour*100)+Minutes
		_iTime=(Time_hours*100)+Time_min;
		_iOnTime=(_hours_on*100)+_min_on;
		_iOffTime=(_hours_off*100)+_min_off;

		if(_iOnTime<_iOffTime)
		{
			//....||||....
			if(_iOnTime<=_iTime && _iOffTime>_iTime)
			{
				RelayOn();
			}
			if(_iOnTime>_iTime || _iOffTime<=_iTime)
			{
				RelayOff();
			}
		}

		if(_iOnTime>_iOffTime)
		{
			//||||....||||
			if(_iOnTime<=_iTime || _iOffTime>_iTime)
			{
				RelayOn();
			}
			if(_iOnTime>_iTime && _iOffTime<=_iTime)
			{
				RelayOff();
			}
		}
	}
	if(_mode==1)
	{
		if((Time_hours==_hours_on)&&(Time_min==_min_on))
		{
			RelayOn();
		}
		if((Time_hours==_hours_off)&&(Time_min==_min_off))
		{
			RelayOff();
		}
	}
}
void Relay::SetTimes(int Hours_on,int Min_on, int Hours_off, int Min_off)
{
	 _hours_on=Hours_on;
	 _hours_off=Hours_off;
	 _min_on=Min_on;
	 _min_off=Min_off;
}
void Relay::ChangeTime(const char *Text="K")
{
	int _iSwich=0;
	char Text_Lcd[16];
	bool _fFlaga1=false;
	//Button_Counter ButtonCounter;
	
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
					sprintf(Text_Lcd,"%2s Hour On",Text);
					x_y(0,0);
					txt(Text_Lcd);
					_hours_on=Button_Counter(0,23,_hours_on,1);
					sprintf(Text_Lcd,"%2d:%2d-%2d:%2d",_hours_on,_min_on,_hours_off,_min_off);
					x_y(0,1);
					txt(Text_Lcd);
			break;
			case 2:
					sprintf(Text_Lcd,"%2s Min On",Text);
					x_y(0,0);
					txt(Text_Lcd);
					_min_on=Button_Counter(0,23,_min_on,1);
					sprintf(Text_Lcd,"%2d:%2d-%2d:%2d",_hours_on,_min_on,_hours_off,_min_off);
					x_y(0,1);
					txt(Text_Lcd);
			break;
			case 3:
					sprintf(Text_Lcd,"%2s Hour Off",Text);
					x_y(0,0);
					txt(Text_Lcd);
					_hours_off=Button_Counter(0,23,_hours_off,1);
					sprintf(Text_Lcd,"%2d:%2d-%2d:%2d",_hours_on,_min_on,_hours_off,_min_off);
					x_y(0,1);
					txt(Text_Lcd);
			break;
			case 4:
					sprintf(Text_Lcd,"%2s Min Off",Text);
					x_y(0,0);
					txt(Text_Lcd);
					_min_off=Button_Counter(0,23,_min_off,1);
					sprintf(Text_Lcd,"%2d:%2d-%2d:%2d",_hours_on,_min_on,_hours_off,_min_off);
					x_y(0,1);
					txt(Text_Lcd);
			break;
		}
	}
	_iSwich=0;
	
	write_to_lcd(clear,C);
	sprintf(Text_Lcd,"%2d:%2d-%2d:%2d",_hours_on,_min_on,_hours_off,_min_off);
	x_y(0,0);
	txt(Text_Lcd);
	_delay_ms(2000);
	write_to_lcd(clear,C);
}









//Stare funkcje
/*
void Relay_Set_Mode(void)
{	
	write_to_lcd(clear,C);
	while(!Button_Enter_One_Press())
	{
		x_y(0,0);
		txt("Set Mode:");
		if(g_fRelayMode==true)
		{
			x_y(0,1);
			txt("<- Full  Auto ->");			
		}
		if(g_fRelayMode==false)
		{
			x_y(0,1);
			txt("<- Semi  Auto ->");	
		}	
		
		if(BUTTON_UP_ON_ONE_PRESS()||BUTTON_DOWN_ON_ONE_PRESS())
		{		
			g_fRelayMode=!g_fRelayMode;
			if(g_fRelayMode==true)
			{
				x_y(0,1);
				txt("<- Full  Auto ->");
			}
			if(g_fRelayMode==false)
			{
				x_y(0,1);
				txt("<- Semi  Auto ->");
			}
		}				
	}	
	write_to_lcd(clear,C);	
}
void Relay_Full_Auto_Mode(int _iHourBuff, int _iMinBuff)
{
	int _iTime,_iK1OnTime,_iK1OffTime,_iK2OnTime,_iK2OffTime,_iK3OnTime,_iK3OffTime,_iK4OnTime,_iK4OffTime;
	//Przeliczenia czasu, dla latwiejszego porownywania : Time=(Hour*100)+Minutes
	_iTime=(_iHourBuff*100)+_iMinBuff;
		
	_iK1OnTime=(g_iK1HourOn*100)+g_iK1MinOn;
	_iK1OffTime=(g_iK1HourOff*100)+g_iK1MinOff;
		
	_iK2OnTime=(g_iK2HourOn*100)+g_iK2MinOn;
	_iK2OffTime=(g_iK2HourOff*100)+g_iK2MinOff;
		
	_iK3OnTime=(g_iK3HourOn*100)+g_iK3MinOn;
	_iK3OffTime=(g_iK3HourOff*100)+g_iK3MinOff;
		
	_iK4OnTime=(g_iK4HourOn*100)+g_iK4MinOn;
	_iK4OffTime=(g_iK4HourOff*100)+g_iK4MinOff;
		
	//Sprawdzanie dla K1
	if(_iK1OnTime<_iK1OffTime)
	{
		//....||||....
		if(_iK1OnTime<=_iTime && _iK1OffTime>_iTime)
		{
			K1_ON;
		}
		if(_iK1OnTime>_iTime || _iK1OffTime<=_iTime)
		{
			K1_OFF;
		}
	}

	if(_iK1OnTime>_iK1OffTime)
	{
		//||||....||||
		if(_iK1OnTime<=_iTime || _iK1OffTime>_iTime)
		{
			K1_ON;
		}
		if(_iK1OnTime>_iTime && _iK1OffTime<=_iTime)
		{
			K1_OFF;
		}
	}
		
	//Sprawdzanie dla K2
	if(_iK2OnTime<_iK2OffTime)
	{
		//....||||....
		if(_iK2OnTime<=_iTime && _iK2OffTime>_iTime)
		{
			K2_ON;
		}
		if(_iK2OnTime>_iTime || _iK2OffTime<=_iTime)
		{
			K2_OFF;
		}
	}

	if(_iK2OnTime>_iK2OffTime)
	{
		//||||....||||
		if(_iK2OnTime<=_iTime || _iK2OffTime>_iTime)
		{
			K2_ON;
		}
		if(_iK2OnTime>_iTime && _iK2OffTime<=_iTime)
		{
			K2_OFF;
		}
	}
		
	//Sprawdzanie dla K3
	if(_iK3OnTime<_iK3OffTime)
	{
		//....||||....
		if(_iK3OnTime<=_iTime && _iK3OffTime>_iTime)
		{
			K3_ON;
		}
		if(_iK3OnTime>_iTime || _iK3OffTime<=_iTime)
		{
			K3_OFF;
		}
	}

	if(_iK3OnTime>_iK3OffTime)
	{
		//||||....||||
		if(_iK3OnTime<=_iTime || _iK3OffTime>_iTime)
		{
			K3_ON;
		}
		if(_iK3OnTime>_iTime && _iK3OffTime<=_iTime)
		{
			K3_OFF;
		}
	}
		
	//Sprawdzanie dla K4
	if(_iK4OnTime<_iK4OffTime)
	{
		//....||||....
		if(_iK4OnTime<=_iTime && _iK4OffTime>_iTime)
		{
			K4_ON;
		}
		if(_iK4OnTime>_iTime || _iK4OffTime<=_iTime)
		{
			K4_OFF;
		}
	}

	if(_iK4OnTime>_iK4OffTime)
	{
		//||||....||||
		if(_iK4OnTime<=_iTime || _iK4OffTime>_iTime)
		{
			K4_ON;
		}
		if(_iK4OnTime>_iTime && _iK4OffTime<=_iTime)
		{
			K4_OFF;
		}
	}
}
void Relay_Semi_Auto_Mode(int _iHourBuff, int _iMinBuff)
{
	//K1
	if((_iHourBuff==g_iK1HourOn)&&(_iMinBuff==g_iK1MinOn))
	{
		K1_ON;
	}
	if((_iHourBuff==g_iK1HourOff)&&(_iMinBuff==g_iK1MinOff))
	{
		K1_OFF;
	}
	//K2
	if((_iHourBuff==g_iK2HourOn)&&(_iMinBuff==g_iK2MinOn))
	{
		K2_ON;
	}
	if((_iHourBuff==g_iK2HourOff)&&(_iMinBuff==g_iK2MinOff))
	{
		K2_OFF;
	}
	//K3
	if((_iHourBuff==g_iK3HourOn)&&(_iMinBuff==g_iK3MinOn))
	{
		K3_ON;
	}
	if((_iHourBuff==g_iK3HourOff)&&(_iMinBuff==g_iK3MinOff))
	{
		K3_OFF;
	}
	//K4
	if((_iHourBuff==g_iK4HourOn)&&(_iMinBuff==g_iK4MinOn))
	{
		K4_ON;
	}
	if((_iHourBuff==g_iK4HourOff)&&(_iMinBuff==g_iK4MinOff))
	{
		K4_OFF;
	}
}

void Relay_K1_Set_Time(void)
{
	int _iSwich=0;
	bool _fFlaga1=false;

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
						g_iK1HourOn=Button_Counter(0,23,g_iK1HourOn);
						sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK1HourOn,g_iK1MinOn,g_iK1HourOff,g_iK1MinOff);
						x_y(0,1);
						txt(g_cTextTab);				
				break;
				case 2:
						x_y(0,0);
						txt("K1 Min On");
						g_iK1MinOn=Button_Counter(0,59,g_iK1MinOn);
						sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK1HourOn,g_iK1MinOn,g_iK1HourOff,g_iK1MinOff);
						x_y(0,1);
						txt(g_cTextTab);
				break;
				case 3:
						x_y(0,0);
						txt("K1 Hour Off");
						g_iK1HourOff=Button_Counter(0,23,g_iK1HourOff);
						sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK1HourOn,g_iK1MinOn,g_iK1HourOff,g_iK1MinOff);
						x_y(0,1);
						txt(g_cTextTab);				
				break;
				case 4:
						x_y(0,0);
						txt("K1 Min Off");
						g_iK1MinOff=Button_Counter(0,59,g_iK1MinOff);
						sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK1HourOn,g_iK1MinOn,g_iK1HourOff,g_iK1MinOff);
						x_y(0,1);
						txt(g_cTextTab);
				break;				
			}
		}
		_iSwich=0;
		
		write_to_lcd(clear,C);
		sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK1HourOn,g_iK1MinOn,g_iK1HourOff,g_iK1MinOff);
		x_y(0,0);
		txt(g_cTextTab);
		_delay_ms(3000);
		write_to_lcd(clear,C);
}
void Relay_K2_Set_Time(void)
{
	int _iSwich=0;
	bool _fFlaga1=false;

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
			g_iK2HourOn=Button_Counter(0,23,g_iK2HourOn);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK2HourOn,g_iK2MinOn,g_iK2HourOff,g_iK2MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 2:
			x_y(0,0);
			txt("K2 Min On");
			g_iK2MinOn=Button_Counter(0,59,g_iK2MinOn);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK2HourOn,g_iK2MinOn,g_iK2HourOff,g_iK2MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 3:
			x_y(0,0);
			txt("K2 Hour Off");
			g_iK2HourOff=Button_Counter(0,23,g_iK2HourOff);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK2HourOn,g_iK2MinOn,g_iK2HourOff,g_iK2MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 4:
			x_y(0,0);
			txt("K2 Min Off");
			g_iK2MinOff=Button_Counter(0,59,g_iK2MinOff);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK2HourOn,g_iK2MinOn,g_iK2HourOff,g_iK2MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
		}
	}
	_iSwich=0;
	
	write_to_lcd(clear,C);
	sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK2HourOn,g_iK2MinOn,g_iK2HourOff,g_iK2MinOff);
	x_y(0,0);
	txt(g_cTextTab);
	_delay_ms(3000);
	write_to_lcd(clear,C);
}
void Relay_K3_Set_Time(void)
{
	int _iSwich=0;
	bool _fFlaga1=false;

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
			g_iK3HourOn=Button_Counter(0,23,g_iK3HourOn);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK3HourOn,g_iK3MinOn,g_iK3HourOff,g_iK3MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 2:
			x_y(0,0);
			txt("K3 Min On");
			g_iK3MinOn=Button_Counter(0,59,g_iK3MinOn);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK3HourOn,g_iK3MinOn,g_iK3HourOff,g_iK3MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 3:
			x_y(0,0);
			txt("K3 Hour Off");
			g_iK3HourOff=Button_Counter(0,23,g_iK3HourOff);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK3HourOn,g_iK3MinOn,g_iK3HourOff,g_iK3MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 4:
			x_y(0,0);
			txt("K3 Min Off");
			g_iK3MinOff=Button_Counter(0,59,g_iK3MinOff);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK3HourOn,g_iK3MinOn,g_iK3HourOff,g_iK3MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
		}
	}
	_iSwich=0;
	
	write_to_lcd(clear,C);
	sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK3HourOn,g_iK3MinOn,g_iK3HourOff,g_iK3MinOff);
	x_y(0,0);
	txt(g_cTextTab);
	_delay_ms(3000);
	write_to_lcd(clear,C);
}
void Relay_K4_Set_Time(void)
{
	int _iSwich=0;
	bool _fFlaga1=false;

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
			g_iK4HourOn=Button_Counter(0,23,g_iK4HourOn);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK4HourOn,g_iK4MinOn,g_iK4HourOff,g_iK4MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 2:
			x_y(0,0);
			txt("K4 Min On");
			g_iK4MinOn=Button_Counter(0,59,g_iK4MinOn);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK4HourOn,g_iK4MinOn,g_iK4HourOff,g_iK4MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 3:
			x_y(0,0);
			txt("K4 Hour Off");
			g_iK4HourOff=Button_Counter(0,23,g_iK4HourOff);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK4HourOn,g_iK4MinOn,g_iK4HourOff,g_iK4MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 4:
			x_y(0,0);
			txt("K4 Min Off");
			g_iK4MinOff=Button_Counter(0,59,g_iK4MinOff);
			sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK4HourOn,g_iK4MinOn,g_iK4HourOff,g_iK4MinOff);
			x_y(0,1);
			txt(g_cTextTab);
			break;
		}
	}
	_iSwich=0;
	
	write_to_lcd(clear,C);
	sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK4HourOn,g_iK4MinOn,g_iK4HourOff,g_iK4MinOff);
	x_y(0,0);
	txt(g_cTextTab);
	_delay_ms(3000);
	write_to_lcd(clear,C);
}


void Relay_General_Set_Time(void)
{
	bool _fRelay=true;
	int _iRelayChoose=0;	
	
	while (_fRelay==true)
	{
		_iRelayChoose=Button_Counter(-1,4,_iRelayChoose);
		if(_iRelayChoose<0 || _iRelayChoose>3)// Wyjscie z menu ustawiana przekaznikow jezeli wartosc g_iI czyli podmenu bedize wieksze od 4 albo mniejsze od 1
		{
			_fRelay=false;
		}
		switch(_iRelayChoose)
		{
			case  0:
					x_y(0,0);
					txt("Ex < K1? > K2");
					x_y(0,1);
					sprintf(g_cTextTab,"%2d:%2d-%d:%2d",g_iK1HourOn,g_iK1MinOn,g_iK1HourOff,g_iK1MinOff);
					txt(g_cTextTab);				
					if(Button_Enter_One_Press())
					{
						x_y(0,1);
						Relay_K1_Set_Time();
						
					}
			break;
			
			case  1:
					x_y(0,0);
					txt("K1 < K2? > K3");
					x_y(0,1);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK2HourOn,g_iK2MinOn,g_iK2HourOff,g_iK2MinOff);
					txt(g_cTextTab);
					if(Button_Enter_One_Press())
					{
						Relay_K2_Set_Time();
						
					}			
			break;
			
			case  2:
					x_y(0,0);
					txt("K2 < K3? > K4");
					x_y(0,1);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK3HourOn,g_iK3MinOn,g_iK3HourOff,g_iK3MinOff);
					txt(g_cTextTab);
					if(Button_Enter_One_Press())
					{
						Relay_K3_Set_Time();
						
					}
			break;
			
			case  3:
					x_y(0,0);
					txt("K3 < K4? > Ex");
					x_y(0,1);
					sprintf(g_cTextTab,"%2d:%2d-%2d:%2d",g_iK4HourOn,g_iK4MinOn,g_iK4HourOff,g_iK4MinOff);
					txt(g_cTextTab);
					if(Button_Enter_One_Press())
					{
						Relay_K4_Set_Time();
						
					}
			break;
		}
	}
}
void Relay_Set_State(void)
{
	bool _fRelayCheck=true;
	int _iSwitch=1;
	
	while(_fRelayCheck==true)
	{
		_iSwitch=Button_Counter(0,5,_iSwitch);
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
					if(K1_IS_OFF) {	x_y(5,1); txt("Off");} if(K1_IS_ON ){x_y(5,1); txt("On ");}
					if(Button_Enter_One_Press())
					{
						//sprawdz_przekazniki_flaga_1=true;
						g_fK1State=!g_fK1State;
						if(g_fK1State==false)
						{
							K1_OFF
							x_y(5,1);
							txt("Off");
						}
						if(g_fK1State==true)
						{
							K1_ON
							x_y(5,1);
							txt("On ");
						}
					}
			break;
			
			case 2:
					x_y(0,1);
					txt("K2");
					if(K2_IS_OFF) {	x_y(5,1); txt("Off");} if(K2_IS_ON ){x_y(5,1); txt("On ");}
					if(Button_Enter_One_Press())
					{
						//sprawdz_przekazniki_flaga_1=true;
						g_fK2State=!g_fK2State;
						if(g_fK2State==false)
						{
							K2_OFF
							x_y(5,1);
							txt("Off");
						}
						if(g_fK2State==true)
						{
							K2_ON
							x_y(5,1);
							txt("On ");
						}
					}
			break;
			
			case 3:
					x_y(0,1);
					txt("K3");
					if(K3_IS_OFF) {	x_y(5,1); txt("Off");} if(K3_IS_ON) {x_y(5,1); txt("On ");}
					if(Button_Enter_One_Press())
					{
						//sprawdz_przekazniki_flaga_1=true;
						g_fK3State=!g_fK3State;
						if(g_fK3State==false)
						{
							K3_OFF
							x_y(5,1);
							txt("Off");
						}
						if(g_fK3State==true)
						{
							K3_ON
							x_y(5,1);
							txt("On ");
						}
					}
			break;
			
			case 4:
					x_y(0,1);
					txt("K4");
					if(K4_IS_OFF) {	x_y(5,1); txt("Off");} if(K4_IS_ON) {x_y(5,1); txt("On ");}
					if(Button_Enter_One_Press())
					{
						//sprawdz_przekazniki_flaga_1=true;
						g_fK4State=!g_fK4State;
						if(g_fK4State==false)
						{
							K4_OFF
							x_y(5,1);
							txt("Off");
						}
						if(g_fK4State==true)
						{
							K4_ON
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
	

	
	while(_fRelayCheck==1)
	{
		_iSwitch=Button_Counter(0,5,_iSwitch);
		x_y(0,0);
		if(_iSwitch<1||_iSwitch>4)
		{
			_fRelayCheck=0;
		}
		switch(_iSwitch)
		{
			case 1:
					x_y(0,0);
					sprintf(g_cTextTab,"K1 %2d:%2d-%2d:%2d",g_iK1HourOn,g_iK1MinOn,g_iK1HourOff,g_iK1MinOff);
					txt(g_cTextTab);

			break;
			case 2:
					x_y(0,0);
					sprintf(g_cTextTab,"K2 %2d:%2d-%2d:%2d",g_iK2HourOn,g_iK2MinOn,g_iK2HourOff,g_iK2MinOff);
					txt(g_cTextTab);

			break;
			case 3:
					x_y(0,0);
					sprintf(g_cTextTab,"K3 %2d:%2d-%2d:%2d",g_iK3HourOn,g_iK3MinOn,g_iK3HourOff,g_iK3MinOff);
					txt(g_cTextTab);

			break;
			case 4:
					x_y(0,0);
					sprintf(g_cTextTab,"K4 %2d:%2d-%2d:%2d",g_iK4HourOn,g_iK4MinOn,g_iK4HourOff,g_iK4MinOff);
					txt(g_cTextTab);

			break;
		}
		
	}
	
	
}

void Relay_Time_Reset(void)
{
		//Swiatlo 1
		g_iK1HourOn=10;
		g_iK1MinOn=0;
		g_iK1HourOff=20;
		g_iK1MinOff=0;
		//Swiatlo 2
		g_iK2HourOn=10;
		g_iK2MinOn=10;
		g_iK2HourOff=19;
		g_iK2MinOff=50;
		//Filtr
		g_iK3HourOn=8;
		g_iK3MinOn=0;
		g_iK3HourOff=23;
		g_iK3MinOff=0;
		//Grzalka
		g_iK4HourOn=2;
		g_iK4MinOn=0;
		g_iK4HourOff=23;
		g_iK4MinOff=0;
		
		x_y(0,0);
		txt("Reset Time");
		_delay_ms(255);
		write_to_lcd(clear,C);
}
void Relay_Show_Status(void)
{
	wdt_reset();
	
	if(K1_IS_ON)
	{
		x_y(0,0);
		txt("K1 On    ");
	}
	else
	{
		x_y(0,0);
		txt("K1 Off   ");
	}
	
	if(K2_IS_ON)
	{
		x_y(9,0);
		txt("K2 On    ");
	}
	else
	{
		x_y(9,0);
		txt("K2 Off   ");
	}
	
	
	if(K3_IS_ON)
	{
		x_y(0,1);
		txt("K3 On    ");
	}
	else
	{
		x_y(0,1);
		txt("K3 Off   ");
	}
		
	if(K4_IS_ON)
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

*/