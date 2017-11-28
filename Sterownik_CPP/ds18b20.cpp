#include "ds18b20.h"
  
//////////////////////////////////////////////////////////////////////////
//**************************Klasa Ds18b20*******************************//
//////////////////////////////////////////////////////////////////////////
  
class Sensor_Ds18b20
{
	private:
			bool _fStart;
			bool _fInitMinMax; // Flaga pomocnicza przy pierwszym uruchomieniu, do przepisania tylko raz temp aktualnej do temp. min max.
			bool _fAllowToRead;
			unsigned char _rom_no[8];
			char _ctext[16],_cteperature[10],_cLowTemp[10],_cHighTemp[10];
			float _fTemperature;
			float _fLSB,_fMSB;
			int _x,_y;
			unsigned int _LSB,_MSB;
			bool _fExist;
			bool _fError;
			float _fLowestTemperature, _fHeighestTemperature;
	
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
			float GetLowTemp(void);
			float GetHighTemp(void);
			void MinMaxTemperature(void);
			void AllowToRead(bool);
	
};	
Sensor_Ds18b20::Sensor_Ds18b20(unsigned char rom_no[8],int x=0, int y=0)
{
		_fTemperature=0;
		_fStart=true;
		_fInitMinMax=true;
		_fAllowToRead=false;
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
float Sensor_Ds18b20::GetLowTemp(void)
{
	return _fLowestTemperature;
}
float Sensor_Ds18b20::GetHighTemp(void)
{
	return _fHeighestTemperature;
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
void Sensor_Ds18b20::AllowToRead(bool Allow)
{
	_fAllowToRead=Allow;
}
void Sensor_Ds18b20::ReadTemperature(void)
{
	if(_fExist && _fAllowToRead)
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
				if(_fStart==true)
				{
					//USART_Text_ISR("\r\nStart");
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
		//Sprawdzanie Min Max Temp
		if(_fInitMinMax==true && !_fError && _fAllowToRead)
		{
			
			if(_fTemperature<80 && _fTemperature>-80 && _fTemperature!=0)
			{
				_fLowestTemperature=_fHeighestTemperature=_fTemperature;
				_fInitMinMax=false;
			}
		}
		if(!_fError && _fAllowToRead)
		{
			if(_fTemperature<80 && _fTemperature>-80)
			{
				if(_fTemperature>_fHeighestTemperature)
				{
					_fHeighestTemperature=_fTemperature;
				}
				if(_fTemperature<_fLowestTemperature)
				{
					_fLowestTemperature=_fTemperature;
				}
			}
		}
		
	}
}
void Sensor_Ds18b20::WriteTemperatureOnLcd(void)
{
	if(!_fError && _fAllowToRead)
	{
		if(_fExist)
		{
				if((_MSB>>7))								//sprawdzenie czy jest ujemna temperatura (bit 8 na MSB)
				{					
					FloatToString(_fTemperature,_cteperature);
					sprintf(_ctext,"-%s\xdf""C", _cteperature);
					x_y(_y,_x);
					txt(_ctext);
				}
				else											//dodatnia temperatura
				{
					FloatToString(_fTemperature,_cteperature);
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
		FloatToString(_fHeighestTemperature,_cHighTemp);
		FloatToString(_fLowestTemperature,_cLowTemp);
		if(_fHeighestTemperature<0)
		{
			sprintf(_cHighTemp,"-%s",_cHighTemp);
		}
		if(_fLowestTemperature<0)
		{
			sprintf(_cLowTemp,"-%s",_cLowTemp);
		}
		sprintf(_ctext,"L:%s H:%s",_cLowTemp,_cHighTemp);
		x_y(_y,_x);
		txt(_ctext);	
	    USART_Text_ISR(_ctext);	
}

///////////////////////////////////////////////////////////////////////////////
//*************************Procedura przeszukujaca 1Wire*********************//
//***************************return: 0 - brak urzadzen***********************//
//************1 - znaleziono urzadzenie, numer ROM jest w buforze ROM_NO*****//
///////////////////////////////////////////////////////////////////////////////

int OWSearch(void)
{
	int id_bit_number;
	int last_zero,rom_byte_number,search_result;
	int id_bit, cmp_id_bit;
	unsigned char rom_byte_mask,search_direction;

	//inicjacja
	id_bit_number=0;
	last_zero=0;
	rom_byte_number=0;
	rom_byte_mask=1;
	search_result=0;

	if(!LastDeviceFlag)
	{
		if(ONE_WIRE_RESET()!=1) //jesli nie udala sie inicjacja to zeruj zmienne
		{
			LastDiscrepancy=0;
			LastDeviceFlag=FALSE;
			LastFamilyDiscrepancy=0;
			return FALSE;
		}
		ONE_WIRE_SEND_BYTE(0xF0);//Search ROM

		do
		{
			id_bit = ONE_WIRE_READ_BIT(); //bit1
			cmp_id_bit = ONE_WIRE_READ_BIT(); //bit2
			
			if((id_bit==1)&&(cmp_id_bit==1))
			break; //jesli bit1=bit2=1 wyjdz z petli
			else
			{
				if(id_bit!=cmp_id_bit)  //kazde urzadzenie ma bit 0 lub 1 w tej pozycji
				{
					search_direction=id_bit;
				}
				else
				{ //00 kolizja
					if(id_bit_number<LastDiscrepancy)
					search_direction=((ROM_NO[rom_byte_number] & rom_byte_mask)>0);
					else //jesli id_bit_number==LastDiscrepancy zapisz 1 jesli nie zapisz 0
					search_direction=(id_bit_number==LastDiscrepancy);
					//jasli zapisal 0 to zapamietaj ta pozycje jako last_zero
					if(search_direction==0)
					{
						last_zero=id_bit_number;
						if(last_zero<9)
						LastFamilyDiscrepancy=last_zero;
					}
				}
				if(search_direction==1)
				ROM_NO[rom_byte_number] |= rom_byte_mask;
				else
				ROM_NO[rom_byte_number] &=~rom_byte_mask;

				ONE_WIRE_SEND_BIT(search_direction);
				id_bit_number++;
				rom_byte_mask<<=1;

				if(rom_byte_mask==0)
				{
					rom_byte_number++;
					rom_byte_mask=1;
				}
			}
		}while (rom_byte_number<8); //wyjdz z petli po odczytaniu 8 bajtow
		
		if(!(id_bit_number<64))
		{
			LastDiscrepancy=last_zero;
			if(LastDiscrepancy==0) LastDeviceFlag=TRUE;
			search_result=TRUE;
		}
	}

	if(!search_result||!ROM_NO[0])
	{
		LastDiscrepancy=0;
		LastDeviceFlag=FALSE;
		LastFamilyDiscrepancy=0;
		search_result = FALSE;
	}

	return search_result;
}
void STR_CPY(unsigned char ID)
{
	unsigned char CP=0;
	for(CP=0;CP<8;CP++)
	{
		DS_ID[ID][CP]=ROM_NO[CP];
	}
}
int OWFirst(void)
{
	LastDiscrepancy=0;
	LastDeviceFlag=FALSE;
	LastFamilyDiscrepancy=0;
	return OWSearch();
}
int OWNext(void)
{
	return OWSearch();
}

//////////////////////////////////////////////////////////////////////////
//*********************Odczyt wielu termometrów*************************//
//////////////////////////////////////////////////////////////////////////

// Automatyczne wyszukiwnie oraz wyswietlanie temperatury z Ds18B20, (max 2 czujniki)
//Do dokonczenia
/*
void Automatyczny_odczyt_temperatury(void)
{
	
test=ONE_WIRE_RESET();

		  if(OWFirst())
		  {
		  	test=ONE_WIRE_RESET();
			act|=1;
		  	STR_CPY(0);
		  }
		  else
		  {
		  	act&=~1;
		  	x_y(0,1);
		    txt("1#NO         ");
		  }
		  if(OWNext())
		  {
		    test=ONE_WIRE_RESET();
			act|=1<<1;
		    STR_CPY(1);
		  }
		  else
		  {
			act&=~(1<1);
		  	x_y(0,0);
		    txt("2#NO        ");	
		  }
	
	if(act)
	{
	ONE_WIRE_SEND_BYTE(0xCC);
	ONE_WIRE_SEND_BYTE(0x44);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	
	if(act&1)
	{
		  test=ONE_WIRE_RESET();
		  ONE_WIRE_SEND_BYTE(0x55);
		  for(cd=0;cd<8;cd++)
		  {
		  	ONE_WIRE_SEND_BYTE(DS_ID[0][cd]);
		  }
		  ONE_WIRE_SEND_BYTE(0xBE);
		  LSB=ONE_WIRE_READ_BYTE();
		  MSB=ONE_WIRE_READ_BYTE();
		  test=ONE_WIRE_RESET();		  	
	 	  //ujemna temperatura
		  if((MSB>>7))										//sprawdzenie czy jest ujemna temperatura (bit 8 na MSB)
		  {
				LSB_float=255-LSB;
				MSB_float=255-MSB;
				Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
				float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
				sprintf(bufor_tekstu,"-%s\xdf""C", Temperatura_Bufor_Char);
		  }
		  //dodatnia temperatura
		  else										
		  {
				LSB_float=LSB;
				MSB_float=MSB;
				Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
				float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
				sprintf(bufor_tekstu,"%s\xdf""C", Temperatura_Bufor_Char);
		  }		  
		  x_y(0,1);
		  txt(bufor_tekstu);
	}
	else
	{
	//	x_y(0,1);
	//	txt("1#NO");
	}		  

	if(act&1<<1)
	{
		test=ONE_WIRE_RESET();
		ONE_WIRE_SEND_BYTE(0x55);
		for(cd=0;cd<8;cd++)
		{
			ONE_WIRE_SEND_BYTE(DS_ID[1][cd]);
		}
		ONE_WIRE_SEND_BYTE(0xBE);
		LSB=ONE_WIRE_READ_BYTE();
		MSB=ONE_WIRE_READ_BYTE();
		test=ONE_WIRE_RESET();			
		//ujemna temperatura
		if((MSB>>7))										//sprawdzenie czy jest ujemna temperatura (bit 8 na MSB)
		{
			LSB_float=255-LSB;
			MSB_float=255-MSB;
			Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
			float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
			sprintf(bufor_tekstu,"-%s\xdf""C", Temperatura_Bufor_Char);
		}
		//dodatnia temperatura
		else										
		{
			LSB_float=LSB;
			MSB_float=MSB;
			Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
			float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
			sprintf(bufor_tekstu,"%s\xdf""C", Temperatura_Bufor_Char);
		}
		x_y(0,0);
  		txt(bufor_tekstu);	
	}
	else
	{
	//	x_y(0,0);
	//	txt("2#NO");
	}
	
}
}						
*/
//////////////////////////////////////////////////////////////////////////
//******************Odczyt pojedynczych termometrów*********************//
//////////////////////////////////////////////////////////////////////////

// Standardowe funkcje do odczytu temperatury z konkretnego 
// termometru (wraz z poleceniem konwersji itp), wymaga znajomosci nr ROM czujnika
/*

int Czujnik_1()
{
	ONE_WIRE_RESET();
    ONE_WIRE_SEND_BYTE(0xCC);
	ONE_WIRE_SEND_BYTE(0x44);
	_delay_ms(750);
	ONE_WIRE_RESET();	
	ONE_WIRE_SEND_BYTE(0x55);
	for(cd=0;cd<8;cd++)
	{
		ONE_WIRE_SEND_BYTE(DS_1_ROM[cd]);
	}
	ONE_WIRE_SEND_BYTE(0xBE);	
	LSB=ONE_WIRE_READ_BYTE();
	MSB=ONE_WIRE_READ_BYTE();

	if((MSB>>7))										//sprawdzenie czy jest ujemna temperatura (bit 8 na MSB)
	{
		LSB_float=255-LSB;
		MSB_float=255-MSB;
		Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
		float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
		sprintf(bufor_tekstu,"-%s\xdf""C", Temperatura_Bufor_Char);
		x_y(0,1);
		txt(bufor_tekstu);
	}
	else										//dodatnia temperatura
		{
		LSB_float=LSB;
		MSB_float=MSB;
		Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
		float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
		sprintf(bufor_tekstu,"%s\xdf""C", Temperatura_Bufor_Char);
		x_y(0,1);
		txt(bufor_tekstu);
		}
}
int Czujnik_2()
{	
	ONE_WIRE_RESET();
	ONE_WIRE_SEND_BYTE(0xCC);
	ONE_WIRE_SEND_BYTE(0x44);
	_delay_ms(750);
	ONE_WIRE_RESET();		
	ONE_WIRE_SEND_BYTE(0x55);
	for(cd=0;cd<8;cd++)
	{
		ONE_WIRE_SEND_BYTE(DS_2_ROM[cd]);
	}
	ONE_WIRE_SEND_BYTE(0xBE);
	LSB=ONE_WIRE_READ_BYTE();
	MSB=ONE_WIRE_READ_BYTE();

	if((MSB>>7))								//sprawdzenie czy jest ujemna temperatura (bit 8 na MSB)
	{
		LSB_float=255-LSB;
		MSB_float=255-MSB;
		Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
		float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
		sprintf(bufor_tekstu,"-%s\xdf""C", Temperatura_Bufor_Char);
		x_y(8,1);
		txt(bufor_tekstu);
	}
	else										//dodatnia temperatura
	{
		LSB_float=LSB;
		MSB_float=MSB;
		Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
		float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
		sprintf(bufor_tekstu,"%s\xdf""C", Temperatura_Bufor_Char);
		x_y(8,1);
		txt(bufor_tekstu);
	}
}
*/
//Funkcje do odczytu temperatury z 2 termometrow, oraz funkcja polecenia konwersji i odczytu, bez odmierzania 750ms. Urzywana byla w glownej petli co 1 sekunde.
/*
void Czujnik_1(void)
{
	//Sam odczyt temperatury bez wysylania polecenia konwersji, opis znajduje się w funkcji Ds18B20_Polecenie_Konwersji();
	ONE_WIRE_RESET();
	ONE_WIRE_SEND_BYTE(0x55);
	for(cd=0;cd<8;cd++)
	{
		ONE_WIRE_SEND_BYTE(DS_1_ROM[cd]);
	}
	ONE_WIRE_SEND_BYTE(0xBE);
	LSB=ONE_WIRE_READ_BYTE();
	MSB=ONE_WIRE_READ_BYTE();

	if((MSB>>7))										//sprawdzenie czy jest ujemna temperatura (bit 8 na MSB)
	{
		LSB_float=255-LSB;
		MSB_float=255-MSB;
		Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
		Temperatura_Ds_1=Temperatura_Bufor_Float;
		float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
		sprintf(bufor_tekstu,"-%s\xdf""C", Temperatura_Bufor_Char);
		x_y(0,1);
		txt(bufor_tekstu);
	}
	else										//dodatnia temperatura
	{
		LSB_float=LSB;
		MSB_float=MSB;
		Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
		Temperatura_Ds_1=Temperatura_Bufor_Float;
		float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
		sprintf(bufor_tekstu," %s\xdf""C", Temperatura_Bufor_Char);
		x_y(0,1);
		txt(bufor_tekstu);
	}
}
void Czujnik_2(void)
{

	ONE_WIRE_RESET();
	ONE_WIRE_SEND_BYTE(0x55);
	for(cd=0;cd<8;cd++)
	{
		ONE_WIRE_SEND_BYTE(DS_2_ROM[cd]);
	}
	ONE_WIRE_SEND_BYTE(0xBE);
	LSB=ONE_WIRE_READ_BYTE();
	MSB=ONE_WIRE_READ_BYTE();

	if((MSB>>7))								//sprawdzenie czy jest ujemna temperatura (bit 8 na MSB)
	{
		LSB_float=255-LSB;
		MSB_float=255-MSB;
		Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
		Temperatura_Ds_2=Temperatura_Bufor_Float;
		float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
		sprintf(bufor_tekstu,"-%s\xdf""C", Temperatura_Bufor_Char);
		x_y(8,1);
		txt(bufor_tekstu);
	}
	else										//dodatnia temperatura
	{
		LSB_float=LSB;
		MSB_float=MSB;
		Temperatura_Bufor_Float=(LSB_float+(MSB_float*256))/16;
		Temperatura_Ds_2=Temperatura_Bufor_Float;
		float_to_str_convert(Temperatura_Bufor_Float,Temperatura_Bufor_Char);
		sprintf(bufor_tekstu," %s\xdf""C", Temperatura_Bufor_Char);
		x_y(8,1);
		txt(bufor_tekstu);
	}
}
void Ds18B20_Polecenie_Konwersji(void)
{
	// Ze wzgledu na to aby czujnik byl odczytywany co rowna 1 sekunde
	// Najpierw odczytywany jest bufor starszej konwersji temperatury a potem
	// wysylana jest komenda do nowej konwersji.
	// Funkcja dostosowana jest w taki sposob aby odczytywana i wysyłana byla komenda co 1 sekunde
	// nie tracac oczekiwania 750 ms na przetworzenie. W tym czasie wykonywane są inne funkcje.
	Czujnik_1();
	Czujnik_2();
	ONE_WIRE_RESET();
	ONE_WIRE_SEND_BYTE(0xCC);
	ONE_WIRE_SEND_BYTE(0x44);
}
*/