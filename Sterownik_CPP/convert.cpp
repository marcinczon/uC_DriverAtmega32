#include "convert.h"

void FloatToString(float n, char *w)
{
	unsigned short s,d,j,t,temp;
	temp=n*100;
	t=(temp-temp%1000);
	s=(temp-t)-(temp-t)%100;
	d=(temp-t-s)-(temp-t-s)%10;
	j=temp-t-s-d;
	t=t/1000;
	s=s/100;
	d=d/10;
	w[0]=48+t;
	w[1]=48+s;
	w[2]=46;
	w[3]=48+d;
	w[4]=48+j;
	w[5]='\0';
}
uint8_t BcdToDec(uint8_t val)
{
	return ( (val/16*10) + (val%16) );
}
uint8_t DecToBcd(uint8_t val)
{
	return ( (val/10*16) + (val%10) );
}