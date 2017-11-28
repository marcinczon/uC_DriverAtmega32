#include "led_rgb.h"

class Led
{
	private:
	bool _fLed_State;
	int _Day_Hour_On, _Day_Hour_Off, _Day_Min_On, _Day_Min_Off;
	int _Night_Hour_On, _Night_Hour_Off, _Night_Min_On, _Night_Min_Off;
	
	unsigned int r,b,g;
	unsigned int czas_r,czas_b,czas_g,dr,dg,db;// dr dg db byly signed int
	signed int deltar,deltag,deltab;
	//Static?
	//signed int czas_rs,czas_bs,czas_gs;
	unsigned int czas_rs,czas_bs,czas_gs;
	signed int dczas_r,dczas_b,dczas_g;//,dr,dg,db;
	bool _mode;//0 - manual 1 - auto
	
	public:
	Led(int,int,int,int,int,int,int,int);
	void On(void);
	void Off(void);
	void Rand_1(bool);
	void Rand_2(void);
	void Rainbow(void);
	void Change_Color_Auto(int,int,int);	
	void Change_Color_UART(int,int,int);
	void Automatic(int,int);
	void Set_Time_Day(void);
	void Set_Time_Night(void);
	void Change_Color(void);	
	void Change_Mode(void);	
	void Set_Mode(bool);
	bool Get_Mode(void);
};

Led::Led(int Day_Hour_On=10,int Day_Min_On=0, int Day_Hour_Off=20,int Day_Min_Off=0,int Night_Hour_On=20,int Night_Min_On=1, int Night_Hour_Off=23,int Night_Min_Off=59)
{
	 _Day_Hour_On=Day_Hour_On;
	 _Day_Hour_Off=Day_Hour_Off;
	 _Day_Min_On=Day_Min_On;
	 _Day_Min_Off=Day_Min_Off;
	 
	 _Night_Hour_On=Night_Hour_On;
	 _Night_Hour_Off=Night_Hour_Off;
	 _Night_Min_On=Night_Min_On;
	 _Night_Min_Off=Night_Min_Off;	
	 
	 deltar=0;
	 deltag=0;
	 deltab=0;
	 
	 _mode=0;
}
void Led::On(void)
{
		_fLed_State=true;
		DDRD|=_BV(5); //OC1A
		DDRD|=_BV(7); //OC2
		DDRB|=_BV(3); //OC0
		
		OCR0=0;
		OCR1A=0;
		OCR2=0;//255
		
		czas_rs=0;//255
		czas_bs=0;
		czas_gs=0;
		// Ustaw preskalery tak aby wypelnienie bylo 20 ms,
		TCCR1A|= _BV(COM1A1)|_BV(WGM10);								//ustawienie trybu PWM dla timera 8 bit (timer1) - mozliwa zmiana na 10 bit
		TCCR1B|=_BV(CS10)|_BV(CS11)|_BV(WGM12);							//prescaler 64
		TCCR0|=_BV(WGM00)|_BV(WGM01)|_BV(CS00)|_BV(CS01)|_BV(COM01);	//prescaler 64
		TCCR2|=_BV(WGM20)|_BV(WGM21)|_BV(CS22)|_BV(COM21);				//prescaler 64
}
void Led::Off(void)
{
		_fLed_State=false;
		Change_Color_Auto(0,0,0);
		TCCR1A&=~(_BV(COM1A1)|_BV(WGM10));								//ustawienie trybu PWM dla timera 8 bit (timer1) - mozliwa zmiana na 10 bit
		TCCR1B&=~(_BV(CS10)|_BV(CS11)|_BV(WGM12));							//prescaler 64
		TCCR0&=~(_BV(WGM00)|_BV(WGM01)|_BV(CS00)|_BV(CS01)|_BV(COM01));
		TCCR2&=~(_BV(WGM20)|_BV(WGM21)|_BV(CS22)|_BV(COM21));
		
		//	DDRD|=_BV(5); //OC1A
		//	DDRD|=_BV(7); //OC2
		//	DDRB|=_BV(3); //OC0
		
		DDRD&=~_BV(5); //OC1A
		DDRD&=~_BV(7); //OC2
		DDRB&=~_BV(3); //OC0
		
		//	PORTD&=~_BV(5);
		//	PORTD|=_BV(7);
		//	PORTB&=~_BV(3);
		//	PORTD|=_BV(5);
		//	PORTD|=_BV(7);
		//	PORTB|=_BV(3);
}
void Led::Rainbow(void)
{
	Change_Color_Auto(255,255,255);	
	Change_Color_Auto(0,0,255);
    Change_Color_Auto(0,255,0);
	Change_Color_Auto(255,0,0);	
	Change_Color_Auto(0,255,255);
	Change_Color_Auto(255,0,255);
	Change_Color_Auto(255,255,0);
	Change_Color_Auto(255,255,255);
}
void Led::Change_Color_Auto(int _Red,int _Blue,int _Green)
{
	int i;
	
	czas_r=_Red;
	czas_b=_Blue;
	czas_g=_Green;
	
	dczas_r=(czas_r-czas_rs);
	dczas_b=(czas_b-czas_bs);
	dczas_g=(czas_g-czas_gs);
	
	deltab=deltar=deltag=0;
	do
	{
		if(db!=czas_b)
		{
			if(dczas_b>0)
			{
				db=czas_bs+deltab;
				deltab++;
			}
			if(dczas_b<0)
			{
				db=czas_bs+deltab;
				deltab--;
			}
		}
		if(dr!=czas_r)
		{
			if(dczas_r>0)
			{
				dr=czas_rs+deltar;
				deltar++;
			}
			if(dczas_r<0)
			{
				dr=czas_rs+deltar;
				deltar--;
			}
		}
		if(dg!=czas_g)
		{
			if(dczas_g>0)
			{
				dg=czas_gs+deltag;
				deltag++;
			}
			if(dczas_g<0)
			{
				dg=czas_gs+deltag;
				deltag--;
			}
		}


		for(i=0;i<2;i++)
		{
			OCR0=db;
			OCR1A=dg;
			OCR2=dr;			
			_delay_ms(5);
			
		}

	}while(!((dr==czas_r)&&(db==czas_b)&&(dg==czas_g)));
	czas_rs=czas_r;
	czas_bs=czas_b;
	czas_gs=czas_g;
}
void Led::Change_Color_UART(int _OCR0, int _OCR1A, int _OCR2)
{
		OCR0=czas_r=_OCR0;
		OCR1A=czas_b=_OCR1A;
		OCR2=czas_g=_OCR2;
}
void Led::Rand_1(bool show=0)
{
	int i=0;
	char Lcd_Text[16];

	r=rand();
	//srand(r);
	r=(r*109+89)%251;
	
	g=rand();
	//srand(g);
	g=(g*109+89)%251;
	
	b=rand();
	//srand(b);
	b=(b*109+89)%251;

	czas_r=r;
	czas_b=b;
	czas_g=g;
	
	dczas_r=(czas_r-czas_rs);
	dczas_b=(czas_b-czas_bs);
	dczas_g=(czas_g-czas_gs);
	
	deltab=deltar=deltag=0;
	do
	{
		if(db!=czas_b)
		{
			if(dczas_b>0)
			{
				db=czas_bs+deltab;
				deltab++;
			}
			if(dczas_b<0)
			{
				db=czas_bs+deltab;
				deltab--;
			}
		}
		if(dr!=czas_r)
		{
			if(dczas_r>0)
			{
				dr=czas_rs+deltar;
				deltar++;
			}
			if(dczas_r<0)
			{
				dr=czas_rs+deltar;
				deltar--;
			}
		}
		if(dg!=czas_g)
		{
			if(dczas_g>0)
			{
				dg=czas_gs+deltag;
				deltag++;
			}
			if(dczas_g<0)
			{
				dg=czas_gs+deltag;
				deltag--;
			}
		}


		for(i=0;i<2;i++)
		{
			if(show==true)
			{
			sprintf(Lcd_Text,"%d=%2d:%d=%2d:%d=%2d",b,db,g,dg,r,dr);
			x_y(0,1);
			txt(Lcd_Text);
			}
			OCR0=db;
			OCR1A=dg;
			OCR2=dr;
			
			_delay_ms(5);
			
		}

	}while(!((dr==czas_r)&&(db==czas_b)&&(dg==czas_g)));
	czas_rs=czas_r;
	czas_bs=czas_b;
	czas_gs=czas_g;
}
void Led::Rand_2(void)
{		
	if(czas_rs==czas_r && czas_bs==czas_b && czas_gs==czas_g)
	{
		r=rand();
		r=(r*109+89)%251;
	
		g=rand();
		g=(g*109+89)%251;
	
		b=rand();
		b=(b*109+89)%251;		
		
		czas_r=r;
		czas_b=b;
		czas_g=g;			
		dczas_r=(czas_r-czas_rs);
		dczas_b=(czas_b-czas_bs);
		dczas_g=(czas_g-czas_gs);			
		deltab=deltar=deltag=0;
	}

	if(db!=czas_b)
	{
		if(dczas_b>0)
		{
			db=czas_bs+deltab;
			deltab++;
		}
		if(dczas_b<0)
		{
			db=czas_bs+deltab;
			deltab--;
		}
	}
	if(dr!=czas_r)
	{
		if(dczas_r>0)
		{
			dr=czas_rs+deltar;
			deltar++;
		}
		if(dczas_r<0)
		{
			dr=czas_rs+deltar;
			deltar--;
		}
	}
	if(dg!=czas_g)
	{
		if(dczas_g>0)
		{
			dg=czas_gs+deltag;
			deltag++;
		}
		if(dczas_g<0)
		{
			dg=czas_gs+deltag;
			deltag--;
		}
	}
	OCR0=db;
	OCR1A=dg;
	OCR2=dr;

	if((dr==czas_r)&&(db==czas_b)&&(dg==czas_g))
	{
	czas_rs=czas_r;
	czas_bs=czas_b;
	czas_gs=czas_g;
	}	
}

void Led::Automatic(int Time_hours, int Time_min)
{
		if(_mode==true)
		{			
			
			int _iTime,_iOnDayTime,_iOffDayTime;//_iOnNightTime,_iOffNightTime;
			//Przeliczenia czasu, dla latwiejszego porownywania : Time=(Hour*100)+Minutes
			_iTime=(Time_hours*100)+Time_min;
			_iOnDayTime=(_Day_Hour_On*100)+_Day_Min_On;
			_iOffDayTime=(_Day_Hour_Off*100)+_Day_Min_Off;
			
			//_iOnNightTime=(_Night_Hour_On*100)+_Night_Min_On;
			//_iOffNightTime=(_Night_Hour_Off*100)+_Night_Min_Off;				
//Dzien
			if(_iOnDayTime<_iOffDayTime)
			{
				//....||||....
				if(_iOnDayTime<=_iTime && _iOffDayTime>_iTime)
				{
					if(_fLed_State==false)
					{
						On();
					}
					Rand_2();
				}
				if(_iOnDayTime>_iTime || _iOffDayTime<=_iTime)
				{
					Off();
				}
			}

			if(_iOnDayTime>_iOffDayTime)
			{
				//||||....||||
				if(_iOnDayTime<=_iTime || _iOffDayTime>_iTime)
				{
					if(_fLed_State==false)
					{
						On();
					}
					Rand_2();
				}
				if(_iOnDayTime>_iTime && _iOffDayTime<=_iTime)
				{
					Off();
				}
			}
	}

}
//Tryb Noc wylaczony			
/*
			if(_iOnNightTime<_iOffNightTime)
			{
				//....||||....
				if(_iNightOnTime<=_iTime && _iNightOffTime>_iTime)
				{
					_iSwitch=1;
				}
				if(_iNightOnTime>_iTime || _iNightOffTime<=_iTime)
				{
					relay_off();
				}
			}

			if(_iNightOnTime>_iNightOffTime)
			{
				//||||....||||
				if(_iNightOnTime<=_iTime || _iNightOffTime>_iTime)
				{
					_iSwitch=1;
				}
				if(_iNightOnTime>_iTime && _iNightOffTime<=_iTime)
				{
					relay_off();
				}
			}
*/

void Led::Set_Time_Day(void)
{
		int _iSwich=1;
		bool _fFlaga1=false;
		char Lcd_Text[16];		

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
						x_y(0,1);				
					    sprintf(Lcd_Text,"N: %2d:%2d-%2d:%2d", _Night_Hour_On,_Night_Min_On,_Night_Hour_Off, _Night_Min_Off);
						txt(Lcd_Text);
				
						_Day_Hour_On=Button_Counter(0,23,_Day_Hour_On,1);
						sprintf(Lcd_Text,"D: %2d:%2d-%2d:%2d",_Day_Hour_On, _Day_Min_On, _Day_Hour_Off,  _Day_Min_Off);
						x_y(0,0);
						txt(Lcd_Text);
				break;
				
				case 2:				
						x_y(0,1);
					    sprintf(Lcd_Text,"N: %2d:%2d-%2d:%2d", _Night_Hour_On,_Night_Min_On,_Night_Hour_Off, _Night_Min_Off);
						txt(Lcd_Text);
						
						_Day_Min_On=Button_Counter(0,59,_Day_Min_On,1);
						sprintf(Lcd_Text,"D: %2d:%2d-%2d:%2d",_Day_Hour_On, _Day_Min_On, _Day_Hour_Off, _Day_Min_Off);
						x_y(0,0);
						txt(Lcd_Text);
				break;
				case 3:				
						x_y(0,1);
					    sprintf(Lcd_Text,"N: %2d:%2d-%2d:%2d", _Night_Hour_On,_Night_Min_On,_Night_Hour_Off, _Night_Min_Off);
						txt(Lcd_Text);
										
						_Day_Hour_Off=Button_Counter(0,23,_Day_Hour_Off,1);
						sprintf(Lcd_Text,"D: %2d:%2d-%2d:%2d",_Day_Hour_On, _Day_Min_On, _Day_Hour_Off, _Day_Min_Off);
						x_y(0,0);
						txt(Lcd_Text);
				break;
				case 4:
				
						x_y(0,1);
						sprintf(Lcd_Text,"N: %2d:%2d-%2d:%2d", _Night_Hour_On,_Night_Min_On,_Night_Hour_Off, _Night_Min_Off);
						txt(Lcd_Text);
						
						_Day_Min_Off=Button_Counter(0,59,_Day_Min_Off,1);
						sprintf(Lcd_Text,"D: %2d:%2d-%2d:%2d",_Day_Hour_On, _Day_Min_On, _Day_Hour_Off, _Day_Min_Off);
						x_y(0,0);
						txt(Lcd_Text);
				break;
			}
		}
		_iSwich=0;
		write_to_lcd(clear,C);
		x_y(0,1);
		sprintf(Lcd_Text,"N: %2d:%2d-%2d:%2d", _Night_Hour_On,_Night_Min_On,_Night_Hour_Off, _Night_Min_Off);
		txt(Lcd_Text);
		sprintf(Lcd_Text,"D: %2d:%2d-%2d:%2d",_Day_Hour_On, _Day_Min_On, _Day_Hour_Off, _Day_Min_Off);
		x_y(0,0);
		txt(Lcd_Text);
		_delay_ms(3000);
		write_to_lcd(clear,C);
}
void Led::Set_Time_Night(void)
{
			int _iSwich=1;
			bool _fFlaga1=false;
			char Lcd_Text[16];
			
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
							x_y(0,1);
							sprintf(Lcd_Text,"Night: %2d:%2d-%2d:%2d", _Night_Hour_On,_Night_Min_On,_Night_Hour_Off, _Night_Min_Off);
							txt(Lcd_Text);
							
							_Night_Hour_On=Button_Counter(0,23,_Night_Hour_On,1);
							sprintf(Lcd_Text,"Day:   %2d:%2d-%2d:%2d",_Day_Hour_On, _Day_Min_On, _Day_Hour_Off,  _Day_Min_Off);
							x_y(0,0);
							txt(Lcd_Text);
					break;					
					case 2:
							x_y(0,1);
							sprintf(Lcd_Text,"Night: %2d:%2d-%2d:%2d", _Night_Hour_On,_Night_Min_On,_Night_Hour_Off, _Night_Min_Off);
							txt(Lcd_Text);
					
							_Night_Min_On=Button_Counter(0,59,_Night_Min_On,1);
							sprintf(Lcd_Text,"Day:   %2d:%2d-%2d:%2d",_Day_Hour_On, _Day_Min_On, _Day_Hour_Off, _Day_Min_Off);
							x_y(0,0);
							txt(Lcd_Text);
					break;
					case 3:
							x_y(0,1);
							sprintf(Lcd_Text,"Night: %2d:%2d-%2d:%2d", _Night_Hour_On,_Night_Min_On,_Night_Hour_Off, _Night_Min_Off);
							txt(Lcd_Text);
					
							_Night_Hour_Off=Button_Counter(0,23,_Night_Hour_Off,1);
							sprintf(Lcd_Text,"Day:   %2d:%2d-%2d:%2d",_Day_Hour_On, _Day_Min_On, _Day_Hour_Off, _Day_Min_Off);
							x_y(0,0);
							txt(Lcd_Text);
					break;
					case 4:					
							x_y(0,1);
							sprintf(Lcd_Text,"Night: %2d:%2d-%2d:%2d", _Night_Hour_On,_Night_Min_On,_Night_Hour_Off, _Night_Min_Off);
							txt(Lcd_Text);
					
							_Night_Min_Off=Button_Counter(0,59,_Night_Min_Off,1);
							sprintf(Lcd_Text,"Day:   %2d:%2d-%2d:%2d",_Day_Hour_On, _Day_Min_On, _Day_Hour_Off, _Day_Min_Off);
							x_y(0,0);
							txt(Lcd_Text);
					break;
				}
			}
			_iSwich=0;
			write_to_lcd(clear,C);
			x_y(0,1);
			sprintf(Lcd_Text,"Night: %2d:%2d-%2d:%2d", _Night_Hour_On,_Night_Min_On,_Night_Hour_Off, _Night_Min_Off);
			txt(Lcd_Text);
			sprintf(Lcd_Text,"Day:   %2d:%2d-%2d:%2d",_Day_Hour_On, _Day_Min_On, _Day_Hour_Off, _Day_Min_Off);
			x_y(0,0);
			txt(Lcd_Text);
			_delay_ms(3000);
			write_to_lcd(clear,C);
}
void Led::Change_Color(void)
{
		int _iSwich=1;
		int _iIntensityPercent=0;
		//int _iIntensityPWM;
		bool _fFlaga1=false;
		char Lcd_Text[16];
		
		czas_rs=czas_r;
		czas_bs=czas_b;
		czas_gs=czas_g;
		
		while(_iSwich!=4)
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
						txt("RED");
						czas_r=OCR2=Button_Counter(0,255,OCR2,5);
						_iIntensityPercent=(OCR2*100)/255;
						sprintf(Lcd_Text,"%3d%% %3d",_iIntensityPercent,OCR2);
						x_y(0,1);
						txt(Lcd_Text);
				break;
				case 2:
						x_y(0,0);
						txt("BLUE");
						czas_b=OCR0=Button_Counter(0,255,OCR0,5);
						_iIntensityPercent=(OCR0*100)/255;
						sprintf(Lcd_Text,"%3d%% %3d",_iIntensityPercent,OCR0);
						x_y(0,1);
						txt(Lcd_Text);
				break;
				case 3:
						x_y(0,0);
						txt("GREEN");
						czas_g=OCR1A=Button_Counter(0,255,OCR1A,5);
						_iIntensityPercent=(OCR1A*100)/255;
						sprintf(Lcd_Text,"%3d%% %3d",_iIntensityPercent,OCR1A);
						x_y(0,1);
						txt(Lcd_Text);
				break;
			}
		}
		_iSwich=0;
		Change_Color_Auto(czas_r,czas_b,czas_g);			
		write_to_lcd(clear,C);
		sprintf(Lcd_Text,"R:%3d B:%3d G:%3d",OCR2,OCR0,OCR1A);
		x_y(0,0);
		txt(Lcd_Text);
		_delay_ms(2000);
		write_to_lcd(clear,C);
}
void Led::Set_Mode(bool Mode)
{
	_mode=Mode;
}
bool Led::Get_Mode(void)
{
	return _mode;
}
void Led::Change_Mode(void)
{	
			x_y(0,0);
			txt("Set Mode:");
			if(_mode==true)
			{
				x_y(0,1);
				txt("<-    Auto    ->");
			}
			if(_mode==false)
			{
				x_y(0,1);
				txt("<-   Manual   ->");
			}
			
			if(Button_Up_One_Press()||Button_Down_One_Press())
			{
				_mode=!_mode;
				if(_mode==true)
				{
					x_y(0,1);
					txt("<-    Auto    ->");
				}
				if(_mode==false)
				{
					x_y(0,1);
					txt("<-   Manual   ->");
				}
			}		
}
/*
void LED_Set_Time_Day(void)
{
	int _iSwich=0;
	bool _fFlaga1=false;
	
	write_to_lcd(clear,C);
	x_y(0,0);
	txt(" >>Press Enter<< ");
	
	while(_iSwich!=5)
	{
		_fFlaga1=Button_Enter_One_Press();
		if(_fFlaga1==true)
		{
			_iSwich++;
			g_iI=0;
			write_to_lcd(clear,C);
		}
		switch(_iSwich)
		{
			case 1:
			x_y(0,0);
			
			sprintf(g_cTextTab,"Night: %d:%d-%d:%d",g_iLedTimeOnHourNight,g_iLedTimeOnMinNight,g_iLedTimeOffHourNight,g_iLedTimeOffMinNight);
			x_y(0,0);
			txt(g_cTextTab);
			g_iHour=Button_Counter();
			if(g_iHour>23) g_iHour=g_iI=0;
			if(g_iHour<0) g_iHour=g_iI=23;
			g_iLedTimeOnHourDay=g_iHour;
			sprintf(g_cTextTab,"Day: (%d):%d-%d:%d",g_iLedTimeOnHourDay,g_iLedTimeOnMinDay,g_iLedTimeOffHourDay,g_iLedTimeOffMinDay);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 2:
			
			sprintf(g_cTextTab,"Night: %d:%d-%d:%d",g_iLedTimeOnHourNight,g_iLedTimeOnMinNight,g_iLedTimeOffHourNight,g_iLedTimeOffMinNight);
			x_y(0,0);
			txt(g_cTextTab);
			g_iMin=Button_Counter();
			if(g_iMin>59) g_iMin=g_iI=0;
			if(g_iMin<0) g_iMin=g_iI=59;
			g_iLedTimeOnMinDay=g_iMin;
			sprintf(g_cTextTab,"Day: %d:(%d)-%d:%d",g_iLedTimeOnHourDay,g_iLedTimeOnMinDay,g_iLedTimeOffHourDay,g_iLedTimeOffMinDay);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 3:
			
			sprintf(g_cTextTab,"Night: %d:%d-%d:%d",g_iLedTimeOnHourNight,g_iLedTimeOnMinNight,g_iLedTimeOffHourNight,g_iLedTimeOffMinNight);
			x_y(0,0);
			txt(g_cTextTab);
			g_iHour=Button_Counter();
			if(g_iHour>23) g_iHour=g_iI=0;
			if(g_iHour<0) g_iHour=g_iI=23;
			g_iLedTimeOffHourDay=g_iHour;
			sprintf(g_cTextTab,"Day: %d:%d-(%d):%d",g_iLedTimeOnHourDay,g_iLedTimeOnMinDay,g_iLedTimeOffHourDay,g_iLedTimeOffMinDay);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 4:
			
			sprintf(g_cTextTab,"Night: %d:%d-%d:%d",g_iLedTimeOnHourNight,g_iLedTimeOnMinNight,g_iLedTimeOffHourNight,g_iLedTimeOffMinNight);
			x_y(0,0);
			txt(g_cTextTab);
			g_iMin=Button_Counter();
			if(g_iMin>59) g_iMin=g_iI=0;
			if(g_iMin<0) g_iMin=g_iI=59;
			g_iLedTimeOffMinDay=g_iMin;
			sprintf(g_cTextTab,"Day: %d:%d-%d:(%d)",g_iLedTimeOnHourDay,g_iLedTimeOnMinDay,g_iLedTimeOffHourDay,g_iLedTimeOffMinDay);
			x_y(0,1);
			txt(g_cTextTab);
			break;
		}
	}
	_iSwich=0;
	//	sprawdz_K1_1=sprawdz_K1_2=0;
	write_to_lcd(clear,C);
	sprintf(g_cTextTab,"Day:   %d:%d-%d:%d",g_iLedTimeOnHourDay,g_iLedTimeOnMinDay,g_iLedTimeOffHourDay,g_iLedTimeOffMinDay);
	x_y(0,0);
	txt(g_cTextTab);
	sprintf(g_cTextTab,"Night: %d:%d-%d:%d",g_iLedTimeOnHourNight,g_iLedTimeOnMinNight,g_iLedTimeOffHourNight,g_iLedTimeOffMinNight);
	x_y(0,1);
	txt(g_cTextTab);
	_delay_ms(3000);
	write_to_lcd(clear,C);
}
void LED_Set_Time_Night(void)
{
	int _iSwich=0;
	bool _fFlaga1=false;
	
	write_to_lcd(clear,C);
	x_y(0,0);
	txt(" >>Press Enter<< ");
	
	while(_iSwich!=5)
	{
		_fFlaga1=Button_Enter_One_Press();
		if(_fFlaga1==true)
		{
			_iSwich++;
			//	sprawdz_K1_2=1;
			g_iI=0;
			write_to_lcd(clear,C);
		}
		switch(_iSwich)
		{
			case 1:
			sprintf(g_cTextTab,"Day: %d:%d-%d:%d",g_iLedTimeOnHourDay,g_iLedTimeOnMinDay,g_iLedTimeOffHourDay,g_iLedTimeOffMinDay);
			x_y(0,0);
			txt(g_cTextTab);
			g_iHour=Button_Counter();
			if(g_iHour>23) g_iHour=g_iI=0;
			if(g_iHour<0) g_iHour=g_iI=23;
			g_iLedTimeOnHourNight=g_iHour;
			sprintf(g_cTextTab,"Night: (%d):%d-%d:%d",g_iLedTimeOnHourNight,g_iLedTimeOnMinNight,g_iLedTimeOffHourNight,g_iLedTimeOffMinNight);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 2:
			sprintf(g_cTextTab,"Day: %d:%d-%d:%d",g_iLedTimeOnHourDay,g_iLedTimeOnMinDay,g_iLedTimeOffHourDay,g_iLedTimeOffMinDay);
			x_y(0,0);
			txt(g_cTextTab);
			g_iMin=Button_Counter();
			if(g_iMin>59) g_iMin=g_iI=0;
			if(g_iMin<0) g_iMin=g_iI=59;
			g_iLedTimeOnMinNight=g_iMin;
			sprintf(g_cTextTab,"Night: %d:(%d)-%d:%d",g_iLedTimeOnHourNight,g_iLedTimeOnMinNight,g_iLedTimeOffHourNight,g_iLedTimeOffMinNight);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 3:
			sprintf(g_cTextTab,"Day: %d:%d-%d:%d",g_iLedTimeOnHourDay,g_iLedTimeOnMinDay,g_iLedTimeOffHourDay,g_iLedTimeOffMinDay);
			x_y(0,0);
			txt(g_cTextTab);
			g_iHour=Button_Counter();
			if(g_iHour>23) g_iHour=g_iI=0;
			if(g_iHour<0) g_iHour=g_iI=23;
			g_iLedTimeOffHourNight=g_iHour;
			sprintf(g_cTextTab,"Night: %d:%d-(%d):%d",g_iLedTimeOnHourNight,g_iLedTimeOnMinNight,g_iLedTimeOffHourNight,g_iLedTimeOffMinNight);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 4:
			sprintf(g_cTextTab,"Day: %d:%d-%d:%d",g_iLedTimeOnHourDay,g_iLedTimeOnMinDay,g_iLedTimeOffHourDay,g_iLedTimeOffMinDay);
			x_y(0,0);
			txt(g_cTextTab);
			
			g_iMin=Button_Counter();
			if(g_iMin>59) g_iMin=g_iI=0;
			if(g_iMin<0) g_iMin=g_iI=59;
			g_iLedTimeOffMinNight=g_iMin;
			sprintf(g_cTextTab,"Night: %d:%d-%d:(%d)",g_iLedTimeOnHourNight,g_iLedTimeOnMinNight,g_iLedTimeOffHourNight,g_iLedTimeOffMinNight);
			x_y(0,1);
			txt(g_cTextTab);
			break;
		}
	}
	_iSwich=0;
	//	sprawdz_K1_1=sprawdz_K1_2=0;
	write_to_lcd(clear,C);
	sprintf(g_cTextTab,"Day:   %d:%d-%d:%d",g_iLedTimeOnHourDay,g_iLedTimeOnMinDay,g_iLedTimeOffHourDay,g_iLedTimeOffMinDay);
	x_y(0,0);
	txt(g_cTextTab);
	sprintf(g_cTextTab,"Night: %d:%d-%d:%d",g_iLedTimeOnHourNight,g_iLedTimeOnMinNight,g_iLedTimeOffHourNight,g_iLedTimeOffMinNight);
	x_y(0,1);
	txt(g_cTextTab);

	_delay_ms(3000);
	write_to_lcd(clear,C);
}
void LED_Color(void)
{
	int _iSwich=0;
	int _iIntensityPercent=0;
	int _iIntensityPWM;
	bool _fFlaga1=false;
	
	write_to_lcd(clear,C);
	x_y(0,0);
	txt(" >>Press Enter<< ");
	
	while(_iSwich!=4)
	{
		_fFlaga1=Button_Enter_One_Press();
		if(_fFlaga1==true)
		{
			_iSwich++;
			g_iI=0;
			write_to_lcd(clear,C);
		}
		switch(_iSwich)
		{
			case 1:
			x_y(0,0);
			txt("RED");
			_iIntensityPercent=Button_Counter();
			_iIntensityPWM=(_iIntensityPercent*255)/100;
			if(_iIntensityPWM>255) _iIntensityPWM=255;
			OCR2=_iIntensityPWM;
			if(_iIntensityPercent>100) _iIntensityPercent=g_iI=0;
			if(_iIntensityPercent<0) _iIntensityPercent=g_iI=100;
			sprintf(g_cTextTab,"%d%% %d",_iIntensityPercent,OCR2);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 2:
			x_y(0,0);
			txt("BLUE");
			_iIntensityPercent=Button_Counter();
			_iIntensityPWM=(_iIntensityPercent*255)/100;
			if(_iIntensityPWM>255) _iIntensityPWM=255;
			OCR0=_iIntensityPWM;
			if(_iIntensityPercent>100) _iIntensityPercent=g_iI=0;
			if(_iIntensityPercent<0) _iIntensityPercent=g_iI=100;
			sprintf(g_cTextTab,"%d%% %d",_iIntensityPercent,OCR0);
			x_y(0,1);
			txt(g_cTextTab);
			break;
			case 3:
			x_y(0,0);
			txt("GREEN");
			_iIntensityPercent=Button_Counter();
			_iIntensityPWM=(_iIntensityPercent*255)/100;
			if(_iIntensityPWM>255) _iIntensityPWM=255;
			OCR1A=_iIntensityPWM;
			if(_iIntensityPercent>100) _iIntensityPercent=g_iI=0;
			if(_iIntensityPercent<0) _iIntensityPercent=g_iI=100;
			sprintf(g_cTextTab,"%d%% %d",_iIntensityPercent,OCR1A);
			x_y(0,1);
			txt(g_cTextTab);
			break;
		}
	}
	_iSwich=0;
	//	sprawdz_K1_1=sprawdz_K1_2=0;
	write_to_lcd(clear,C);
	sprintf(g_cTextTab,"R%d B%d G%d",OCR2,OCR0,OCR1A);
	x_y(0,0);
	txt(g_cTextTab);
	_delay_ms(2000);
	write_to_lcd(clear,C);
}


void PWM_ON(void)
{	
	DDRD|=_BV(5); //OC1A
	DDRD|=_BV(7); //OC2
	DDRB|=_BV(3); //OC0					
				
	OCR0=0;
	OCR1A=0;
	OCR2=254;	
	
	czas_rs=255;
	czas_bs=0;
	czas_gs=0;				
																	// Ustaw preskalery tak aby wypelnienie bylo 20 ms,
	TCCR1A|= _BV(COM1A1)|_BV(WGM10);								//ustawienie trybu PWM dla timera 8 bit (timer1) - mozliwa zmiana na 10 bit
	TCCR1B|=_BV(CS10)|_BV(CS11)|_BV(WGM12);							//prescaler 64
	TCCR0|=_BV(WGM00)|_BV(WGM01)|_BV(CS00)|_BV(CS01)|_BV(COM01);	//prescaler 64
	TCCR2|=_BV(WGM20)|_BV(WGM21)|_BV(CS22)|_BV(COM21);				//prescaler 64
}
void PWM_OFF(void)
{	
	TCCR1A&=~(_BV(COM1A1)|_BV(WGM10));								//ustawienie trybu PWM dla timera 8 bit (timer1) - mozliwa zmiana na 10 bit
	TCCR1B&=~(_BV(CS10)|_BV(CS11)|_BV(WGM12));							//prescaler 64
	TCCR0&=~(_BV(WGM00)|_BV(WGM01)|_BV(CS00)|_BV(CS01)|_BV(COM01));
	TCCR2&=~(_BV(WGM20)|_BV(WGM21)|_BV(CS22)|_BV(COM21));	
	
//	DDRD|=_BV(5); //OC1A
//	DDRD|=_BV(7); //OC2
//	DDRB|=_BV(3); //OC0
	
	DDRD&=~_BV(5); //OC1A
	DDRD&=~_BV(7); //OC2
	DDRB&=~_BV(3); //OC0
	
//	PORTD&=~_BV(5);
//	PORTD|=_BV(7);
//	PORTB&=~_BV(3);
//	PORTD|=_BV(5);
//	PORTD|=_BV(7);
//	PORTB|=_BV(3);
											
}
void LED_Dzien(void)
{
				OCR0=255;
				OCR1A=255;
				OCR2=255;	
}
void LED_Random(void)
{
	int i=0;
	//unsigned int m,n;
	unsigned int r=0,b=0,g=0;
	unsigned int czas_r=0,czas_b=0,czas_g=0;

		r=rand();
		//srand(r);		
		r=(r*109+89)%251;
		
		g=rand();
		//srand(g);
		g=(g*109+89)%251;
		
		b=rand();
		//srand(b);
		b=(b*109+89)%251;

		czas_r=r;
		czas_b=b;
		czas_g=g;
		
		dczas_r=(czas_r-czas_rs);
		dczas_b=(czas_b-czas_bs);
		dczas_g=(czas_g-czas_gs);
		
		deltab=deltar=deltag=0;
		do
		{
			if(db!=czas_b)
			{
				if(dczas_b>0)
				{
					db=czas_bs+deltab;
					deltab++;
				}
				if(dczas_b<0)
				{
					db=czas_bs+deltab;
					deltab--;
				}
			}
			if(dr!=czas_r)
			{
				if(dczas_r>0)
				{
					dr=czas_rs+deltar;
					deltar++;
				}
				if(dczas_r<0)
				{
					dr=czas_rs+deltar;
					deltar--;
				}
			}
			if(dg!=czas_g)
			{
				if(dczas_g>0)
				{
					dg=czas_gs+deltag;
					deltag++;
				}
				if(dczas_g<0)
				{
					dg=czas_gs+deltag;
					deltag--;
				}
			}


			for(i=0;i<2;i++)
			{
			//	sprintf(tekst,"%d=%2d:%d=%2d:%d=%2d",b,db,g,dg,r,dr);
			//	x_y(0,1);
			//	txt(tekst);
				OCR0=db;
				OCR1A=dg;
				OCR2=dr;
							//	OCR0=100;
							//	OCR1A=100;
							//	OCR2=100;
				_delay_ms(4);
				
			}

		}while(!((dr==czas_r)&&(db==czas_b)&&(dg==czas_g)));
		czas_rs=czas_r;
		czas_bs=czas_b;
		czas_gs=czas_g;	
			

}
void LED_Random_2(void)
{
	int i=0;
	//unsigned int m,n;
	unsigned int r=0,b=0,g=0;
	unsigned int czas_r=0,czas_b=0,czas_g=0;

	r=rand();
	//srand(r);
	r=(r*109+89)%251;
	
	g=rand();
	//srand(g);
	g=(g*109+89)%251;
	
	b=rand();
	//srand(b);
	b=(b*109+89)%251;

	sprintf(tekst,"%3d %3d %3d",b,g,r);
	x_y(0,0);
	txt(tekst);

	czas_r=r;
	czas_b=b;
	czas_g=g;
	
	dczas_r=(czas_r-czas_rs);
	dczas_b=(czas_b-czas_bs);
	dczas_g=(czas_g-czas_gs);
	
	deltab=deltar=deltag=0;
	do
	{
		if(db!=czas_b)
		{
			if(dczas_b>0)
			{
				db=czas_bs+deltab;
				deltab++;
			}
			if(dczas_b<0)
			{
				db=czas_bs+deltab;
				deltab--;
			}
		}
		if(dr!=czas_r)
		{
			if(dczas_r>0)
			{
				dr=czas_rs+deltar;
				deltar++;
			}
			if(dczas_r<0)
			{
				dr=czas_rs+deltar;
				deltar--;
			}
		}
		if(dg!=czas_g)
		{
			if(dczas_g>0)
			{
				dg=czas_gs+deltag;
				deltag++;
			}
			if(dczas_g<0)
			{
				dg=czas_gs+deltag;
				deltag--;
			}
		}


		for(i=0;i<2;i++)
		{
			OCR0=db;
			OCR1A=dg;
			OCR2=dr;
			//	OCR0=100;
			//	OCR1A=100;
			//	OCR2=100;
			_delay_ms(5);
			
		}

	}while(!((dr==czas_r)&&(db==czas_b)&&(dg==czas_g)));
	czas_rs=czas_r;
	czas_bs=czas_b;
	czas_gs=czas_g;
	

}

*/






