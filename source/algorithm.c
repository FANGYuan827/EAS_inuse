
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

void alarm_start(void);
void alarm_stop(void);

/*////////////////////////////////////////////////////////////////////////////////////////*/ 
void alarm_start(void)
{   
    EALLOW;
    GpioDataRegs.GPACLEAR.bit.GPIO0=1;     //�˴������ø�while��1����һֱִ�иú�����
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
  1.�������ݻ��߸����������ĸ���
  2.



*/
