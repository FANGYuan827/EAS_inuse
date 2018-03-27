
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

void alarm_start(void);
void alarm_stop(void);

/*////////////////////////////////////////////////////////////////////////////////////////*/ 
void alarm_start(void)
{   
    EALLOW;
    GpioDataRegs.GPACLEAR.bit.GPIO0=1;     //此处可以用个while（1）来一直执行该函数；
    GpioDataRegs.GPACLEAR.bit.GPIO2=1;
    delayxus(100);
    GpioDataRegs.GPASET.bit.GPIO0=1;
    GpioDataRegs.GPASET.bit.GPIO2=1;
    delayxus(100);
    EDIS;
}
void alarm_stop(void)
{
    EALLOW;
    GpioDataRegs.GPASET.bit.GPIO0=1;
    GpioDataRegs.GPASET.bit.GPIO2=1;
	EDIS;
}


/*
  1.整形数据或者浮点数据用哪个？
  2.



*/
