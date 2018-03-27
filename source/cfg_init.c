#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

extern Uint16 m_sequence_send[15];

void gpio_cfg(void)
{
  EALLOW;
//---------------报警红灯与喇叭-------------
    GpioCtrlRegs.GPAMUX1.bit.GPIO0=0;
    GpioCtrlRegs.GPADIR.bit.GPIO0=1;   
    GpioCtrlRegs.GPAMUX1.bit.GPIO2=0;
    GpioCtrlRegs.GPADIR.bit.GPIO2=1;   
//---------------外部中断引脚---------------
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;         // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO6 = 0;          // input
    GpioCtrlRegs.GPAQSEL1.bit.GPIO6 = 0;        // Xint1 Synch to SYSCLKOUT only
//---------------波形控制1------------------
    GpioCtrlRegs.GPAMUX1.bit.GPIO13=0;
    GpioCtrlRegs.GPADIR.bit.GPIO13=1;   
//---------------波形控制2------------------
    GpioCtrlRegs.GPAMUX1.bit.GPIO15=0;
    GpioCtrlRegs.GPADIR.bit.GPIO15=1;  
    GpioCtrlRegs.GPAMUX2.bit.GPIO16=0;
    GpioCtrlRegs.GPADIR.bit.GPIO16=1;  
    GpioCtrlRegs.GPAMUX1.bit.GPIO1=0;
    GpioCtrlRegs.GPADIR.bit.GPIO1=1;  
	GpioCtrlRegs.GPAMUX1.bit.GPIO3=0;
    GpioCtrlRegs.GPADIR.bit.GPIO3=1;
//------------//AD9834reset部分-------------
	GpioCtrlRegs.GPAMUX2.bit.GPIO25=0;   
    GpioCtrlRegs.GPADIR.bit.GPIO25=1;
//---------------报警LED--------------------
	GpioCtrlRegs.GPBMUX2.bit.GPIO49=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO49=1;
	GpioCtrlRegs.GPBMUX2.bit.GPIO50=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO50=1;
	GpioCtrlRegs.GPBMUX2.bit.GPIO51=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO51=1;
	GpioCtrlRegs.GPBMUX2.bit.GPIO55=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO55=1;
	GpioCtrlRegs.GPBMUX2.bit.GPIO56=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO56=1;
	GpioCtrlRegs.GPAMUX2.bit.GPIO28=0;
	GpioCtrlRegs.GPADIR.bit.GPIO28=1;
	GpioCtrlRegs.GPBMUX1.bit.GPIO34=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO34=1;
	GpioCtrlRegs.GPBMUX1.bit.GPIO35=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO35=1;
	GpioCtrlRegs.GPBMUX1.bit.GPIO37=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO37=1;
	GpioCtrlRegs.GPBMUX1.bit.GPIO41=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO41=1;
//------------------------------------------
  EDIS;  
}
/*---------------------------------------------------------------------------------*/
void init_timer()
{
       EALLOW;
	   if(m_sequence_send[0]==1)
	   	  GpioDataRegs.GPASET.bit.GPIO1=1;     //2017.08.08comment：@1 发射
	   else
	      GpioDataRegs.GPASET.bit.GPIO1=0;    //2017.08.08comment：@0  不发射
	   
	   GpioDataRegs.GPASET.bit.GPIO13=1;  //50us置高准备
       GpioDataRegs.GPASET.bit.GPIO15=1;  //5ms置高准备天线1
       GpioDataRegs.GPACLEAR.bit.GPIO16=1; //5ms拉低准备天线2
       GpioDataRegs.GPACLEAR.bit.GPIO3=1;  //5us低准备天线2
	   EDIS;
	   CpuTimer0Regs.TCR.all = 0x4021; // Use write-only instruction to set TSS bit = 0
	   CpuTimer1Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
}
void XINTConfig()
{
	EALLOW;
	//   GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 29;   // Xint1 is GPI29
	GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 6;   // XINT2 is GPIO1
    EDIS;

	// Configure XINT1
	XIntruptRegs.XINT2CR.bit.POLARITY = 0;      // Falling edge interrupt
	//   XIntruptRegs.XINT2CR.bit.POLARITY = 1;      // Rising edge interrupt
	   
	// Enable XINT1 and XINT2   
	 //  XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // Enable Xint1
	XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // Enable XINT2
}
//LED0~10*****************************//
void led0()
{
    EALLOW;
    GpioDataRegs.GPBSET.bit.GPIO49=1;
	GpioDataRegs.GPBSET.bit.GPIO50=1;
	GpioDataRegs.GPBSET.bit.GPIO51=1;
	GpioDataRegs.GPBSET.bit.GPIO55=1;
    GpioDataRegs.GPBSET.bit.GPIO56=1;
    GpioDataRegs.GPASET.bit.GPIO28=1;
    GpioDataRegs.GPBSET.bit.GPIO34=1;
    GpioDataRegs.GPBSET.bit.GPIO35=1;
    GpioDataRegs.GPBSET.bit.GPIO37=1;
    GpioDataRegs.GPBSET.bit.GPIO41=1;
	EDIS;
}

void led1(void)
{
    EALLOW;
	GpioDataRegs.GPBCLEAR.bit.GPIO49=1;
	EDIS;
}
void led2(void)
{
    EALLOW;
	GpioDataRegs.GPBCLEAR.bit.GPIO49=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
	EDIS;
}
void led3(void)
{
    EALLOW;
	GpioDataRegs.GPBCLEAR.bit.GPIO49=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO51=1;
	EDIS;
}

void led4(void)
{
    EALLOW;
    GpioDataRegs.GPBCLEAR.bit.GPIO49=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO51=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO55=1;
	EDIS;
}
void led5(void)
{
    EALLOW;
    GpioDataRegs.GPBCLEAR.bit.GPIO49=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO51=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO55=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO56=1;
	EDIS;
}
void led6(void)
{
    EALLOW;
	GpioDataRegs.GPBCLEAR.bit.GPIO49=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO51=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO55=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO56=1;
    GpioDataRegs.GPACLEAR.bit.GPIO28=1;
	EDIS;
}
void led7(void)
{    
    EALLOW;
	GpioDataRegs.GPBCLEAR.bit.GPIO49=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO51=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO55=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO56=1;
    GpioDataRegs.GPACLEAR.bit.GPIO28=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO34=1;
	EDIS;
}
void led8(void)
{
    EALLOW;
	GpioDataRegs.GPBCLEAR.bit.GPIO49=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO51=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO55=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO56=1;
    GpioDataRegs.GPACLEAR.bit.GPIO28=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO34=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO35=1;
	EDIS;
}
void led9(void)
{   
    EALLOW;
	GpioDataRegs.GPBCLEAR.bit.GPIO49=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO51=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO55=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO56=1;
    GpioDataRegs.GPACLEAR.bit.GPIO28=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO34=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO35=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO37=1;
	EDIS;
}
void led10(void)
{   EALLOW;
	GpioDataRegs.GPBCLEAR.bit.GPIO49=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO51=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO55=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO56=1;
    GpioDataRegs.GPACLEAR.bit.GPIO28=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO34=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO35=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO37=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO41=1;
	EDIS;
}


