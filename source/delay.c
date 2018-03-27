#include "delay.h"
void delayxms(Uint32 xms)
{
	Uint32 i,j;
	i=15000;
	j=xms*i;

    while(j>0)
	  j--;
}
//-----------------------------------------------------------------------------------------------
void delayxus(Uint32 xus)
{
	Uint32 i,j;
	i=15;
	j=xus*i;

    while(j>0)
 	  j--;
}
