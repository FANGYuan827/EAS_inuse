#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "ShareData.h"
#include "delay.h"
#include "My_Config.h"

/*--------------------------------初始化函数部分-------------------------------*/
//DDS初始化函数
void dds_init4()
{
   mcbspb_xmit(0x2138,0);
   mcbspb_xmit(0x2038,0);
   mcbspb_xmit(0xE000,0);

   //mcbspb_xmit(0x7439,0);    //向DDS芯片的频率控制寄存器中写入频率控制字的14LSB
   mcbspb_xmit(0x4001,0);    //LSB = 0x4001 14LSB = 00-0000-0000-0001
   //mcbspb_xmit(0x4003,0);
   mcbspb_xmit(0x1038,0);  //改1038为1028，目的是SIGN BIT OUT输出频率寄存器的MSB
   delayxus(80);
   mcbspb_xmit(0x4000,0);
   delayxus(80);
   mcbspb_xmit(0x4B22,0); 		//写入初始的14MSB=0B22  
   //mcbspb_xmit(0x4223,0);
   mcbspb_xmit(0xE000,0);  //相位寄存器1置0
   mcbspb_xmit(0xD000,0);  //相位寄存器0置0
}

//定时器初始化函数
void init_timer()
{
   EALLOW;
   GpioDataRegs.GPASET.bit.GPIO3=1;      //GPIO1 发射
   GpioDataRegs.GPASET.bit.GPIO13=1;  	 //置高准备在下降沿触发ADC采样
   GpioDataRegs.GPASET.bit.GPIO16=1; 	 //GPIO1 发射时 GPIO16打开作为接收天线  
  
   GpioDataRegs.GPACLEAR.bit.GPIO15=1; 	 //GPIO1发射时 GPIO15关闭
   GpioDataRegs.GPACLEAR.bit.GPIO16=1; 	 //初始状态下 射频开关处于关闭状态   20180201;
       
   GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;
   GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;
   ///////
   GpioDataRegs.GPACLEAR.bit.GPIO1=1;  //5us低准备天线2
   EDIS;
   CpuTimer0Regs.TCR.all = 0x4021; // Use write-only instruction to set TSS bit = 0
   CpuTimer1Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
}

/*-------------------------------配置函数部分--------------------------------------*/
//McBSP配置函数

void mcbspb_xmit(int a, int b)
{
  //  McbspbRegs.DXR2.all=b;
    McbspbRegs.DXR1.all=a;
}
/*----------------------------------------------------------------------------------------*/

void mcbspa_AGC(int a)
{
   McbspaRegs.DXR1.all=a;
}


//ADC配置函数
void ADC_Config()
{
   AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;
   AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
   AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // 0 Non-Cascaded Mode; 1 Cascaded Mode级联
   AdcRegs.ADCTRL1.bit.CONT_RUN = 1; /////连续模式
//   AdcRegs.ADCTRL1.bit.SEQ_OVRD = 1;
   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1;
 //  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0x1;
 //  AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ=0x1;  //级联可配置

   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;
   AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;
   AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0x0001;   // Set up ADC to perform 4 conversions for every SOC
}

//外部中断配置函数 
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


//所有控制IO口的配置函数
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
//-----------ADG734另外两路开关-------------   2017.12.06 add..........
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 =0;
    GpioCtrlRegs.GPADIR.bit.GPIO14  =1;  
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 =0;
    GpioCtrlRegs.GPADIR.bit.GPIO17  =1;  
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


/*------------------------LED显示函数-----------------------------*/
//LED0~10 配置函数
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

/*-------------------------------------------------------------------------------------------*/
void error(void)
{
    asm("     ESTOP0");  // test failed!! Stop!
    for (;;);
}

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


/*-------------动态调整AGC的增益----------------------*/
void AGC_Adjust(float input)     			/*@input表示CFFT的幅度值之和*/
{
	int AGC_GAIN = 0;
	if(input<0.0)
	{
		return;
	}

	if(input<=10)             		 //阈值1 
	{
		AGC_GAIN = MAX_GAIN;
		mcbspa_AGC(AGC_GAIN);
		goto lable;
	}

	if(input<=20)               	 //阈值2
	{
		AGC_GAIN = START_GAIN+5;
		mcbspa_AGC(AGC_GAIN);
		goto lable;
	}

	if(input<=30)            		//阈值3
	{
		AGC_GAIN = START_GAIN-5;
		mcbspa_AGC(AGC_GAIN);
		goto lable;	
	}

	if(input>30)		           //阈值4
	{
		AGC_GAIN = START_GAIN-10;
		mcbspa_AGC(AGC_GAIN);	
	}

lable:
	AGC_GAIN = 0;

}



