
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "math.h"  
#include "float.h"
#include "FPU.h"
#include "delay.h"
#include "ShareData.h"
#include "My_Config.h"
#include "string.h"
//#include <stdio.h>
#if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
  #define ADC_MODCLK 0x3 //  HSPCLK = SYSCLKOUT/2*ADC_MODCLK2   
  // Configure CPU-Timer 0 to interrupt every 200 milliseconds:
// 150MHz CPU Freq, 50 millisecond Period (in uSeconds)
#endif

#if (CPU_FRQ_100MHZ)
  #define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 100/(2*2)   = 25.0 MHz
  // Configure CPU-Timer 0 to interrupt every 200 milliseconds:
// 100MHz CPU Freq, 20 millisecond Period (in uSeconds)
  // ConfigCpuTimer(&CpuTimer0, 100, 200000);
#endif

//Buffer alignment for the input array,
//CFFT_f32u (optional), CFFT_f32 (required)
//Output of FFT overwrites input if
//CFFT_STAGES is ODD
#pragma DATA_SECTION(CFFTin1Buff,"CFFTdata1");
float   CFFTin1Buff[CFFT_SIZE*2];                  //Output of FFT overwrites input if 
                                                //CFFT_STAGES is ODD
#pragma	DATA_SECTION(CFFTin2Buff,"CFFTdata2");  
float	CFFTin2Buff[CFFT_SIZE*2];               //Additional Buffer used in Magnitude calc

#pragma	DATA_SECTION(CFFToutBuff,"CFFTdata3");	
float	CFFToutBuff[CFFT_SIZE*2];               //Output of FFT here if CFFT_STAGES is EVEN

#pragma	DATA_SECTION(CFFTF32Coef,"CFFTdata4"); 
float	CFFTF32Coef[CFFT_SIZE];                 //Twiddle buffer
CFFT_F32_STRUCT cfft;
void CTTF_f32(CFFT_F32_STRUCT *);    //CFFT 函数声明


// Global variable for this example
Uint16 j = 0,ADC_END = 0; // ADC finish flag

#pragma DATA_SECTION(ADC,"DMARAML4");
volatile float ADC[BUF_SIZE];

//#pragma DATA_SECTION(ADC_Result,"DMARAML6");


#pragma DATA_SECTION(DMABuf1,"DMARAML4");
volatile Uint16 DMABuf1[BUF_SIZE];		    

volatile Uint16 *DMADest;
volatile Uint16 *DMASource;

// Global variables for this example
Uint16 freq_num,array_index,flag=0;
Uint16 freq_mark;
Uint32 int1_count=0;
Uint32 int2_count=0;
Uint16 alarm_flag=0;
Uint16 loop=1,mode=0;
Uint16 count=0;
Uint16 test_num=0;
Uint16 conside_tag;
float mag_first[DATA_SIZE],mag_last[DATA_SIZE];
Uint16 alarm_count=0;
float init_mean=0.0;
Uint16 sample_count1;
Uint16 sample_count2,freq_fix;
extern float FFT_Array[80];


#define DELAY 35.700L
// Prototype statements for functions found within this file.
#define WORD_SIZE 16 
//extern Uint16 sample_count;
Uint32 freq;

//void Fft_func(CFFT_F32_STRUCT *Pcfft);   //2017.07.14 comment：fft处理函数

/* void init_adc()
 {
 	AdcRegs.ADCTRL2.bit.SOC_SEQ1=0x1;  
 }*/
/////////////////////////////////////////////
void main(void)
{ 
 
  int16 i;//,m,alarm_num=0;
  int mag=START_GAIN;
  //static float ADC_Result[BUF_SIZE]={0};
 // ADC_Result[BUF_SIZE]={0};
  flag=0;
  freq=START_FREQ;
  freq_num=0;
  sample_count1=0;
  sample_count2=0; 
  freq_fix=0;
  InitSysCtrl();   //初始化DSP时钟（确定PLL倍频数、分频数）、初始化外设时钟
   EALLOW;
   SysCtrlRegs.HISPCP.all = ADC_MODCLK;	// HSPCLK = SYSCLKOUT/ADC_MODCLK
   EDIS;
// Step 2. Initalize GPIO: 
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example
   InitMcbspaGpio();  
   InitMcbspbGpio();
// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts 
   DINT;
// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.  
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt 
// Service Routines (ISR).  
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
// This function is found in DSP2833x_PieVect.c.
   InitPieVectTable();

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.  
   EALLOW;	// This is needed to write to EALLOW protected registers
   PieVectTable.DINTCH1= &DINTCH1_ISR;
   PieVectTable.TINT0 = &TINT0_ISR;
   PieVectTable.XINT13 = &INT13_ISR;    		 //2017.07.12 comment：Timer_1中断
   PieVectTable.XINT2 = &XINT2_ISR;				//外部中断2
   EDIS;   // This is needed to disable write to EALLOW protected registers
	
// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
   InitCpuTimers(); // Not required for this example
 
// Step 5. User specific code, enable interrupts:
   ConfigCpuTimer(&CpuTimer0, 150, 10);     //三个参数分别表示：定时器0、定时器输入时钟频率（MHz）、定时器计数值
   ConfigCpuTimer(&CpuTimer1, 150, 2000);
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=0;
   EDIS;
   EnableInterrupts();
   InitAdc();  // For this example, init the ADC
   InitXintf();
   InitMcbspb();
   InitMcbspa();
   ADC_Config();
//Step 5. User specific code, enable interrupts:
  // Initialize DMA 
   DMAInitialize();
    
	// Clear Table
   for (i=0; i<BUF_SIZE; i++)
   {
      DMABuf1[i] = 0xffff;
   }
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1;
   EDIS;
// Enable Xint1 and XINT2 in the PIE: Group 1 interrupt 4 & 5
// Enable int1 which is connected to WAKEINT:
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block 打开PIE控制器
//   PieCtrlRegs.PIEIER1.bit.INTx1 = 1;          // Enable PIE Gropu 1 INT1 ADCseq1
   PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // Enable PIE Gropu 1 INT5  外部中断2 
   PieCtrlRegs.PIEIER7.bit.INTx1 = 1;		    //打开DMACH1中断
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;			//打开定时器0中断
   IER |=(M_INT7|M_INT1| M_INT13);                              // Enable CPU int1   @M_INT13开启了Timer_1中断
   EINT;                                       // Enable Global Interrupts
   ERTM;
   
// Configure DMA Channel
    DMADest   = &DMABuf1[0];              //Point DMA destination to the beginning of the array
	DMASource = &AdcMirror.ADCRESULT0;    //Point DMA source to ADC result register base
	DMACH1AddrConfig(DMADest,DMASource);
	DMACH1BurstConfig(1,1,256);      //(1,1,64)
	DMACH1TransferConfig(255,-1,(-256 + 1));  //(63,-1,(-64 + 1))
	DMACH1WrapConfig(1000,1000,1000,1000);	  //Don't use wrap function
	DMACH1ModeConfig(DMA_SEQ1INT,PERINT_ENABLE,ONESHOT_DISABLE,CONT_DISABLE,SYNC_DISABLE,SYNC_SRC,
	                 OVRFLOW_DISABLE,SIXTEEN_BIT,CHINT_END,CHINT_ENABLE); 

   led0();
   alarm_stop();
   gpio_cfg();
   XINTConfig();

  
   if(WORD_SIZE == 16) 
   {
   	  InitMcbspb16bit();
   }
   InitMcbspa8bit();
   dds_init4();
   
//   init_adc();
   mcbspb_xmit(freq,0);
   
   mcbspa_AGC(mag);
   init_timer();  //定时器0，1开 时序引脚准备
   for(;;)
    { 
        if(alarm_flag==2)
		{
		    GpioDataRegs.GPACLEAR.bit.GPIO3=1;  		//GPIO1拉低 停止发射
		    StopCpuTimer0();
	        StopCpuTimer1();
            led3();
			delayxms(500);
			init_timer();
			//StartCpuTimer0();
			//StartCpuTimer1();   		//2017.07.12 modify:增加这句
            led0();
			freq_num=0;
			mode = 0;
			freq_fix=0;
			alarm_flag=0;
			ADC_END=0;
			sample_count1 = 0;
        } 
      else 
         {
		    alarm_flag=0;
         	mode=0;
         } 
	   conside_tag=0; 
	 
   }//end of for 
         
}   // end of main()
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/





