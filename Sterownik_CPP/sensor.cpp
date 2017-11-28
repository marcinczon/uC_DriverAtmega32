

 float g_fHighTemp, g_fLowTemp;
 
class Sensor_Ds18b20
{
	private:
			bool _fStart;
			unsigned char _rom_no[8];
			char _ctext[16],_cteperature[10];//,_cLowTemp[10],_cHighTemp[10];
			float _fTemperature;
			float _fLSB,_fMSB;
			int _x,_y;
			unsigned int _LSB,_MSB;
			bool _fExist;
			bool _fError;
			//double _fLowestTemperature, _fHeighestTemperature;
	
	public:	
			Sensor_Ds18b20(unsigned char *,int,int);
			~Sensor_Ds18b20(void);
		//	void ReadTemperatureToLcd(void);
			void ConvertTemperature(void);
			void ReadTemperature(void);
			void WriteTemperatureOnLcd(void);			//dodaj mozliwosc zmiany pozycji, nie tylko przez set
			void SetRomNo(unsigned char *);
			void SetX(int);
			void SetY(int);
			void SetXY(int,int);
			float GetTemperature(void);
			char GetRomNo(int);
			void SetExist(bool);
			bool GetExist(void);
			void MinMaxTemperature(void);
	
};	

Sensor_Ds18b20::Sensor_Ds18b20(unsigned char rom_no[8],int x=0, int y=0)
{
		_fTemperature=0;
		_fStart=true;
		_fLSB=0;
		_fMSB=0;
		_LSB=0;
		_MSB=0;		
		_x=x;
		_y=y;		    
		for(int i=0;i<8;i++)
		{
			_rom_no[i]=rom_no[i];
		}
		
}
Sensor_Ds18b20::~Sensor_Ds18b20()
{	
}
void Sensor_Ds18b20::SetRomNo(unsigned char rom_no[8])
{
		for(int i=0;i<8;i++)
		{
			_rom_no[i]=rom_no[i];
		}
}
void Sensor_Ds18b20::SetX(int x)
{
	_x=x;
}
void Sensor_Ds18b20::SetY(int y)
{
	_y=y;
}
void Sensor_Ds18b20::SetXY(int X,int Y)
{
	_x=X;
	_y=Y;
}
void Sensor_Ds18b20::SetExist(bool Exist)
{
	_fExist=Exist;
}
bool Sensor_Ds18b20::GetExist(void)
{
	return _fExist;
}
float Sensor_Ds18b20::GetTemperature(void)
{
	return _fTemperature;
}
char Sensor_Ds18b20::GetRomNo(int NoBajt)
{	
	return _rom_no[NoBajt];
}
//void Sensor_Ds18b20::ReadTemperatureToLcd(void)
/*
void Sensor_Ds18b20::ReadTemperatureToLcd(void)
{
	ONE_WIRE_RESET();
	ONE_WIRE_SEND_BYTE(0xCC);
	ONE_WIRE_SEND_BYTE(0x44);
	_delay_ms(750);
	ONE_WIRE_RESET();
	ONE_WIRE_SEND_BYTE(0x55);
	for(int i=0;i<8;i++)
	{
		ONE_WIRE_SEND_BYTE(_rom_no[i]);
	}
	ONE_WIRE_SEND_BYTE(0xBE);
	_LSB=ONE_WIRE_READ_BYTE();
	_MSB=ONE_WIRE_READ_BYTE();

	if((_MSB>>7))								//sprawdzenie czy jest ujemna temperatura (bit 8 na MSB)
	{
		_fLSB=255-_LSB;
		_fMSB=255-_MSB;
		_fTemperature=(_fLSB+(_fMSB*256))/16;
		float_to_str_convert(_fTemperature,Temperatura_Bufor_Char);
		sprintf(bufor_tekstu,"-%s\xdf""C", Temperatura_Bufor_Char);
		x_y(_y,_x);
		txt(bufor_tekstu);
	}
	else										//dodatnia temperatura
	{
		_fLSB=_LSB;
		_fMSB=_MSB;
		_fTemperature=(_fLSB+(_fMSB*256))/16;
		float_to_str_convert(_fTemperature,Temperatura_Bufor_Char);
		sprintf(bufor_tekstu,"%s\xdf""C", Temperatura_Bufor_Char);
		x_y(_y,_x);
		txt(bufor_tekstu);
	}
}
*/
void Sensor_Ds18b20::ConvertTemperature(void)
{
		ONE_WIRE_RESET();
		ONE_WIRE_SEND_BYTE(0xCC);
		ONE_WIRE_SEND_BYTE(0x44);
		
		//ONE_WIRE_RESET();
		//ONE_WIRE_SEND_BYTE(0x55);
		//for(int i=0;i<8;i++)
		//{
		  // ONE_WIRE_SEND_BYTE(_rom_no[i]);
		//}
		//ONE_WIRE_SEND_BYTE(0x44);		
}
void Sensor_Ds18b20::ReadTemperature(void)
{
	if(_fExist)
	{
		ONE_WIRE_RESET();
		ONE_WIRE_SEND_BYTE(0x55);
		for(int i=0;i<8;i++)
		{
			ONE_WIRE_SEND_BYTE(_rom_no[i]);
		}
		ONE_WIRE_SEND_BYTE(0xBE);
		_LSB=ONE_WIRE_READ_BYTE();
		_MSB=ONE_WIRE_READ_BYTE();
		if(_LSB==255 && _MSB==255)	_fError=true;	
		else						_fError=false;
		if(!_fError)
		{
				if((g_fHighTemp>80)||(g_fLowTemp<-80))
				{
					_fStart=true;
				}
				if(_fStart==true)
				{
					USART_Text("\r\nStart");
					_fStart=false;
					if((_MSB>>7))								//sprawdzenie czy jest ujemna temperatura (bit 8 na MSB)
					{
						_fLSB=255-_LSB;
						_fMSB=255-_MSB;
						_fTemperature=(_fLSB+(_fMSB*256))/16;					
					}
					else										//dodatnia temperatura
					{
						_fLSB=_LSB;
						_fMSB=_MSB;
						_fTemperature=(_fLSB+(_fMSB*256))/16;						
					}	
				}			

				
				if(_fStart==false)
				{
					if((_MSB>>7))								//sprawdzenie czy jest ujemna temperatura (bit 8 na MSB)
					{
						_fLSB=255-_LSB;
						_fMSB=255-_MSB;
						_fTemperature=(_fLSB+(_fMSB*256))/16;
					}
					else										//dodatnia temperatura
					{
						_fLSB=_LSB;
						_fMSB=_MSB;
						_fTemperature=(_fLSB+(_fMSB*256))/16;
					}		
				}
				
		}
	}
}
void Sensor_Ds18b20::WriteTemperatureOnLcd(void)
{
	if(!_fError)
	{
		if(_fExist)
		{
				if((_MSB>>7))								//sprawdzenie czy jest ujemna temperatura (bit 8 na MSB)
				{					
					float_to_str_convert(_fTemperature,_cteperature);
					sprintf(_ctext,"-%s\xdf""C", _cteperature);
					x_y(_y,_x);
					txt(_ctext);
				}
				else											//dodatnia temperatura
				{
					float_to_str_convert(_fTemperature,_cteperature);
					sprintf(_ctext," %s\xdf""C", _cteperature);
					x_y(_y,_x);
					txt(_ctext);
				}
		}
		else
		{
				x_y(_y,_x);
				txt("  ----  ");
		}
	}
	else
	{
		x_y(_y,_x);
		txt(" Error  ");
	}
}
void Sensor_Ds18b20::MinMaxTemperature(void)
{			
		//float_to_str_convert(_fLowestTemperature,_cteperature);
		//sprintf(_ctext," %s\xdf""C", _cteperature);
		//float_to_str_convert(_fHeighestTemperature,_cteperature);
		//sprintf(_ctext,"%.2f %.2f",_fLowestTemperature,_fHeighestTemperature);
		//x_y(_y,_x);
		//txt(_ctext);	
	    //USART_Text(_ctext);	
}