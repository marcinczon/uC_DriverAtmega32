

class BuforCykliczny
{
	private:
	int Rozmiar;
	char data[65];
	int poczatek;
	int koniec;
	int zajetosc;
	
	public:
	
	BuforCykliczny();
	bool Empty(void);
	char Pop(void);
	void Pop_UDR(void);
	void Push(char);
	void Delete(void);
	int Occupied(void);
	
};

BuforCykliczny::BuforCykliczny()
{
		poczatek=0;
		koniec=0;
		Rozmiar=sizeof(data);
		zajetosc=0;
}

bool BuforCykliczny::Empty()
{
		if(poczatek==koniec)
		return true;
		else
		return false;
}

void BuforCykliczny::Push(char _data)
{
		if(koniec<Rozmiar)
		{
			
			if(koniec+1!=poczatek)
			{
				data[koniec++]=_data;
			}
			//	koniec++;
		}
		else
		{
			if(poczatek!=0)
			{
				data[0]=_data;
				koniec=1;
			}
		}
}

void BuforCykliczny::Delete()
{
	data[poczatek-1]=0;
}

char BuforCykliczny::Pop()
{
		if(poczatek!=koniec)
		{
			
			if(poczatek>=Rozmiar)
			{
				poczatek=0;
			}
			
			return data[poczatek++];
		}
		else
		return 0;
}

void BuforCykliczny::Pop_UDR()
{
	if(poczatek!=koniec)
	{
		
		if(poczatek>=Rozmiar)
		{
			poczatek=0;
		}
		
		UDR = data[poczatek++];
	}
}

int BuforCykliczny::Occupied(void)
{
	if(poczatek<=koniec)
	{
		//zajetosc=((koniec-poczatek)/Rozmiar)*100; 
		zajetosc=koniec-poczatek;		
	}
	if(poczatek>koniec)
	{
		//zajetosc=(((Rozmiar-poczatek)+koniec)/Rozmiar)*100;
		zajetosc=Rozmiar-poczatek+koniec;
	}
	return zajetosc;
}
