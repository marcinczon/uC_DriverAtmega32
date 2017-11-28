/*
 * rtc_pcf8583.cpp
 *
 * Created: 2013-10-28 17:24:28
 *  Author: Marcin
 */ 
#include "rtc_pcf8583.h"

class rtc_pcf8583
{
	
	private:
		int _hour, _min, _sec,_sec_old;
		int _BCD;
		int _x,_y;	
		
	public:
		rtc_pcf8583(void);
		void SetHour(int);
		void SetMin(int);
		void SetSec(int);
		void SaveTime(int, int, int);
		void SetTime(void);
		int GetHour(void);
		int GetMin(void);
		int GetSec(void);	
		void ShowTime(int, int);
		int CheckSecond(void);
		
};
rtc_pcf8583::rtc_pcf8583()
{

}
void rtc_pcf8583::SetHour(int hour)
{
		hour=DecToBcd(hour);
		EEPROM_write(4,hour);
}
void rtc_pcf8583::SetMin(int min)
{
		min=DecToBcd(min);
		EEPROM_write(3,min);
}
void rtc_pcf8583::SetSec(int sec)
{
		sec=DecToBcd(sec);
		EEPROM_write(2,sec);
}
void rtc_pcf8583::SaveTime(int _Hour, int _Min, int _Sec)
{
		if(_Hour>23) _Hour=0;
		if(_Hour<0) _Hour=23;
		_BCD=DecToBcd(_Hour);
		EEPROM_write(4,_BCD);
		if(_Min>59) _Min=59;	
		if(_Min<0) _Min=0;
		_BCD=DecToBcd(_Min);
		EEPROM_write(3,_BCD);
		if(_Sec>59) _Sec=59;
		if(_Sec<0) _Sec=0;
		_BCD=DecToBcd(_Sec);
		EEPROM_write(2,_BCD);
}
void rtc_pcf8583::SetTime(void)
{
		bool _fFlaga1=false;
		int _iSwich=1;
		char _text[16];		
		
		_sec=EEPROM_read(2);
		_sec=BcdToDec(_sec);
		_min=EEPROM_read(3);
		_min=BcdToDec(_min);
		_hour=EEPROM_read(4);
		_hour=BcdToDec(_hour);

		write_to_lcd(clear,C);
		do
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
						txt("Set Clock: Hour   ");
						sprintf(_text,"%2d : %2d : %2d",_hour,_min,_sec);
						_hour=Button_Counter(0,23,_hour,1);
						//_hour=ButtonCounterTime.Count(0,23,_hour,1);
						x_y(0,1);
						txt(_text);
		
				break;
				case 2:
						x_y(0,0);
						txt("Set Clock: Min    ");
						sprintf(_text,"%2d : %2d : %2d",_hour,_min,_sec);
						_min=Button_Counter(0,59,_min,1);
						//_min=ButtonCounterTime.Count(0,59,_min,1);
						x_y(0,1);
						txt(_text);
				break;
				case 3:
						x_y(0,0);
						txt("Set Clock: Sec    ");
						sprintf(_text,"%2d : %2d : %2d",_hour,_min,_sec);
						_sec=Button_Counter(0,59,_sec,1);
						//_sec=ButtonCounterTime.Count(0,59,_sec,1);
						x_y(0,1);
						txt(_text);
				break;
			}
		}while(_iSwich!=4);
	    SaveTime(_hour,_min,_sec);
	
}
int rtc_pcf8583::GetHour(void)
{
		_hour=EEPROM_read(4);
		_hour=BcdToDec(_hour);
		return _hour;	
}
int rtc_pcf8583::GetMin(void)
{
		_min=EEPROM_read(3);
		_min=BcdToDec(_min);
		return _min;
}
int rtc_pcf8583::GetSec(void)
{
		_sec=EEPROM_read(2);
		_sec=BcdToDec(_sec);
		return _sec;
}
void rtc_pcf8583::ShowTime(int X=3,int Y=0)
{
		char _text[10];
		_x=X;
		_y=Y;
		
		_sec=EEPROM_read(2);
		_sec=BcdToDec(_sec);

		//Min
		_min=EEPROM_read(3);
		_min=BcdToDec(_min);
		
		//Godz
		_hour=EEPROM_read(4);
		_hour=BcdToDec(_hour);

		sprintf(_text," %2d:%2d:%2d",_hour,_min,_sec);
		x_y(_x,_y);
		txt(_text);

}
int rtc_pcf8583::CheckSecond(void)
{
		_sec=EEPROM_read(2);
		_sec=BcdToDec(_sec);
		
		if(_sec!=_sec_old)
		{
			_sec_old=_sec;
			return 1;
		}
		else
		{
			return 0;
		}
}

/* Stare Funkcje - liniowe dot RTC*/
/*
void Clock_SetTime(void)
{
	bool _fFlaga1=false;
	int _iSwich=1;
	int _godziny=0, _minuty=0, _sekundy=0;
	
	_sekundy=EEPROM_read(2);
	_sekundy=BcdToDec(_sekundy);
	_minuty=EEPROM_read(3);
	_minuty=BcdToDec(_minuty);
	_godziny=EEPROM_read(4);
	_godziny=BcdToDec(_godziny);

		write_to_lcd(clear,C);
		do		
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
						txt("Set Clock: Hour   ");				
						sprintf(g_cTextTab," %2d : %2d : %2d",_godziny,_minuty,_sekundy);
						_godziny=Button_Counter(0,23,_godziny);
						x_y(0,1);
						txt(g_cTextTab);
				
				break;
				case 2:
						x_y(0,0);
						txt("Set Clock: Min    ");
						sprintf(g_cTextTab,"%2d : %2d : %2d",_godziny,_minuty,_sekundy);
						_minuty=Button_Counter(0,59,_minuty);
						x_y(0,1);
						txt(g_cTextTab);
				break;
				case 3:
						x_y(0,0);
						txt("Set Clock: Sec    ");			
						sprintf(g_cTextTab,"%2d : %2d : %2d",_godziny,_minuty,_sekundy);
						_sekundy=Button_Counter(0,59,_sekundy);
						x_y(0,1);
						txt(g_cTextTab);
				break;				
			}
		}while(_iSwich!=4);

		write_to_lcd(clear,C);
		_godziny=DecToBcd(_godziny);
		EEPROM_write(4,_godziny);
		_minuty=DecToBcd(_minuty);
		EEPROM_write(3,_minuty);
		_sekundy=DecToBcd(_sekundy);
		EEPROM_write(2,_sekundy);	
}
*/
/*
void ShowTime(void)
{
	//RTC ODCZYT CZASU GODZ MIN SEK
	int _iDana;
	
	//Sec
	_iDana=EEPROM_read(2);
	g_iSecBuff=BcdToDec(_iDana);

	//Min
	_iDana=EEPROM_read(3);
	g_iMinBuff=BcdToDec(_iDana);
	
	//Godz
	_iDana=EEPROM_read(4);
	g_iHourBuff=BcdToDec(_iDana);
	
	sprintf(g_cTextTab," %2d:%2d:%2d",g_iHourBuff,g_iMinBuff,g_iSecBuff);
	x_y(3,0);
	txt(g_cTextTab);
}
*/ 
/*
int ClockCheckSecond(void)
{
	if(g_iSecBuff!=g_iTimeSecOldBuff)	
	{
		g_iTimeSecOldBuff=g_iSecBuff;
		return 1;
	}
	else
	{
		return 0;
	}		
}
*/