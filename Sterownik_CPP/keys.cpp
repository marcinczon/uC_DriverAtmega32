#include "keys.h"
/*
class Button_Up
{
	private:
		bool _fButtonOnePressUp;
	
	public:
		bool PressUp(void);	
		Button_Up(void)
		{
		 _fButtonOnePressUp=false;
		}
};
bool Button_Up::PressUp(void)
{	
	bool _fFlagaUp=false;
	if(BUTTON_UP_OFF && _fButtonOnePressUp==true)
	{
		_fButtonOnePressUp=false;
	}
	if(_fButtonOnePressUp==false)
	while(BUTTON_UP_OFF)
	{
		_fButtonOnePressUp=true;
		_fFlagaUp=true;
	}	
	return _fFlagaUp;
}

class Button_Down
{	
	private:
		bool _fButtonOnePressDown;
	
	
	public:
		bool PressDown(void);
		Button_Down(void)
		{
			_fButtonOnePressDown=false;
		}
};
bool Button_Down::PressDown(void)
{	
	bool _fFlagaDown=false;
	if(BUTTON_DOWN_OFF && _fButtonOnePressDown==true)
	{
		_fButtonOnePressDown=false;
	}
	if(_fButtonOnePressDown==false)
	while(BUTTON_DOWN_ON)
	{
		_fButtonOnePressDown=true;
		_fFlagaDown=true;
	}		
	return _fFlagaDown;		
}

class Button_Enter
{
	private:
	bool _fButtonOnePressEnter;
	
	public:
	bool PressEnter(void);
	Button_Enter(void)
	{
		_fButtonOnePressEnter=false;
	}
};
bool Button_Enter::PressEnter(void)
{
	bool _fFlagaEnter=false;
	if(BUTTON_ENTER_OFF && _fButtonOnePressEnter==true)
	{
		_fButtonOnePressEnter=false;
	}
	if(_fButtonOnePressEnter==false)
	while(BUTTON_ENTER_ON)
	{
		_fButtonOnePressEnter=true;
		_fFlagaEnter=true;
	}
		
	return _fFlagaEnter;
}
*/
/*
class Button_Counter //: public Button_Down,public Button_Up //, public Button_Enter
{
	public:
	int Count(int,int,int,int);
		
	private:
	//int _range_Low;
	//int _range_High;
	//int _i;
	//int _k;
};
int Button_Counter::Count(int RangeLow=0,int RangeHigh=10, int I=0,int K=1)
{	 
	bool _fFlagaUp=false;
	bool _fFlagaDown=false;

	_fFlagaDown=BUTTON_DOWN_ON_ONE_PRESS();
	if(_fFlagaDown==true)
	{
		I=I-K;
	}	

	_fFlagaUp=Button_Down_One_Press();
	if(_fFlagaUp==true)
	{
		I=I+K;
	}
	 	
	if(I>RangeHigh) I=RangeLow;
	if(I<RangeLow)  I=RangeHigh;
	
	return I;	
}
*/
int Button_Enter_One_Press(void)
{
	bool _fFlaga1=false;
	if(BUTTON_ENTER_OFF && g_fButtonOnePress==true)
	{
		g_fButtonOnePress=false;
	}
	if(g_fButtonOnePress==false)
	while(BUTTON_ENTER_ON)
	{
		g_fButtonOnePress=true;
		_fFlaga1=true;
	}
	
	return _fFlaga1;
}
int Button_Up_One_Press(void)
{
	bool _fFlaga1=false;
	if(BUTTON_UP_OFF && g_fButtonOnePress==true)
	{
		g_fButtonOnePress=false;
	}
	if(g_fButtonOnePress==false)
	while(BUTTON_UP_ON)
	{
		g_fButtonOnePress=true;
		_fFlaga1=true;
	}
	
	return _fFlaga1;
}
int Button_Down_One_Press(void)
{
	bool _fFlaga1=false;
	if(BUTTON_DOWN_OFF && g_fButtonOnePress==true)
	{
		g_fButtonOnePress=false;
	}
	if(g_fButtonOnePress==false)
	while(BUTTON_DOWN_ON)
	{
		g_fButtonOnePress=true;
		_fFlaga1=true;
	}
	
	return _fFlaga1;
}
//BUTTON_UP_OR_DOWN
int Button_Counter(int _range_Low=0,int _range_High=10, int _i=0,int _k=1)
{
	bool _fFlaga1=false;
	
	_fFlaga1=Button_Up_One_Press();
	if(_fFlaga1==true)
	{
		_i=_i+_k;
		//_i++;
	}

	_fFlaga1=Button_Down_One_Press();
	if(_fFlaga1==true)
	{
		//_i--;
		_i=_i-_k;
	}
	
	if(_i>_range_High) _i=_range_Low;
	if(_i<_range_Low)  _i=_range_High;
	
	return _i;
}



//Dokonczyc prze³ozenie przyciskow na klasy!
