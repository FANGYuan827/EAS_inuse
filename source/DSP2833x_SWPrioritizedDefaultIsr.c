// TI File $Revision: /main/2 $
// Checkin $Date: April 4, 2007   14:25:27 $
//###########################################################################
//
// FILE:    DSP2833x_SWPrioritizedDefaultIsr.c
//
// TITLE:   DSP2833x Device Default Software Prioritized Interrupt Service Routines.
//
//###########################################################################
//
// $TI Release: DSP2833x Header Files V1.01 $
// $Release Date: September 26, 2007 $
//###########################################################################


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "DSP2833x_SWPrioritizedIsrLevels.h" 
#include "ShareData.h"
#include "My_Config.h"
#include "delay.h"
#include "math.h"
Uint32 hold;
extern Uint16 sample_count1;
float min[2]={0};
int mark[2]={0};
float FFT_Array[80] = {0};        //用于存储天线1发射时 接收到的80个频点的FFT的幅度值 
// Connected to INT13 of CPU (use MINT13 mask):
// Note CPU-Timer1 is reserved for TI use, however XINT13
// ISR can be used by the user. 
#if (INT13PL != 0)
interrupt void INT13_ISR(void)     // INT13 or CPU-Timer1
{
	 IER |= MINT13;                 // Set "global" priority
	 EINT;

 // Insert ISR Code here
    CpuTimer1Regs.TCR.bit.TSS = 1;   //关中断  

    int2_count=CpuTimer1.InterruptCount++;
    sample_count1++;
	if(sample_count1>1)
	{
	   sample_count1=0;  
	}
   
	if(int2_count==0)                   //GPIO3 发射结束
	{  
	    flag=1;
	    //GpioDataRegs.GPACLEAR.bit.GPIO15=1;			//20180322 close......
	    //GpioDataRegs.GPASET.bit.GPIO16=1;          
		//GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;        //20180322 close......
		GpioDataRegs.GPADAT.bit.GPIO3=0; //5us拉低
	    GpioDataRegs.GPASET.bit.GPIO1=1;
    }
   if(int2_count==1)                   //GPIO1 发射结束                
   {													
	    freq_num++;
	    if(!mode)                 //mode=0 正常扫频 mode=1锁定一个频率
	    {
	    	freq=freq-0x0002;     //更改频率控制字  
	    }
		else
		{
		     freq=hold;
		}
	    CpuTimer1.InterruptCount=0;  //重新开始天线1,freq接着计数
	    
	    flag=0;
	    //GpioDataRegs.GPASET.bit.GPIO15=1;  
	    //GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;            //20180322 close.......
	    //GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;			//20180322 close......
	    
	    GpioDataRegs.GPADAT.bit.GPIO1=0;
	    GpioDataRegs.GPASET.bit.GPIO3=1;
   } 
                                                                                                                                                                            
   if(flag==1)
      GpioDataRegs.GPADAT.bit.GPIO1=1;  //5us置高准备
   else 
   	  GpioDataRegs.GPADAT.bit.GPIO3=1;

   CpuTimer0Regs.TCR.bit.TSS = 0;    //开定时器0中断
   CpuTimer0.InterruptCount=0;
   
  
   
   if(freq_num==80)              	 //如果完成80个频率点的扫频  完成一个周期的过程
   {
   	  freq=START_FREQ;
   	  CpuTimer1.InterruptCount=0;
   	  freq_num=0;
   }  
   mcbspb_xmit(freq,0);                   // 改变DDS产生的频率   
   
   CpuTimer1Regs.TCR.bit.TSS = 0;   //开定时器1中断
   // The CPU acknowledges the interrupt.
   
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  //   asm ("      ESTOP0");
  //   for(;;);
}
#endif

// Connected to PIEIER1_5 (use MINT1 and MG15 masks):
#if (G15PL != 0)
interrupt void  XINT2_ISR(void)     //外部中断
{
	// Set interrupt priority:
	volatile Uint16 TempPIEIER = PieCtrlRegs.PIEIER1.all;
	IER |= MINT1;                      // Set "global" priority
	PieCtrlRegs.PIEIER1.all &= MG15;   // Set "group"  priority	
	PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts	
	EINT;

  	// Insert ISR Code here.......
	AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1=1;    //
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0x1;    //2017.07.13 comment：写1立即复位到CONV00的状态
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1;
    StartDMACH1();		
//	 Acknowledge this interrupt to get more from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
  	

	// Restore registers saved:
	DINT;
	PieCtrlRegs.PIEIER1.all = TempPIEIER;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
//    asm ("      ESTOP0");
//    for(;;);
}
#endif
// Connected to PIEIER1_7 (use MINT1 and MG17 masks):
#if (G17PL != 0)
interrupt void  TINT0_ISR(void)      // CPU-Timer 0
{
	// Set interrupt priority:
	volatile Uint16 TempPIEIER = PieCtrlRegs.PIEIER1.all;
	IER |= M_INT1;
	IER	&= MINT1;	  	               // Set "global" priority    @MINT1=0
	PieCtrlRegs.PIEIER1.all &= MG17;   // Set "group"  priority	   @MG17=0xff40
	PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts	
	EINT;

  // Insert ISR Code here.......
    int1_count= CpuTimer0.InterruptCount++;
	CpuTimer0Regs.TCR.bit.TSS = 1;   	//查资料TSS位应该是启停定时器。并不是控制中断

	if(int1_count==0)                    //高电平持续时间结束
	{
       if(flag) 
       	  {
       	  	GpioDataRegs.GPADAT.bit.GPIO1=0; 		//GPIO1 发射之后 拉低
			delayxus(1);
			GpioDataRegs.GPASET.bit.GPIO16 = 1;    //20180201:GPIO1发射 结束 打开开关进行接收
		  }
       else 
          {
          	GpioDataRegs.GPADAT.bit.GPIO3=0; 		//GPIO3 发射之后 拉低
		    delayxus(1);
		    GpioDataRegs.GPASET.bit.GPIO15 = 1;     //20180201:GPIO3发射 结束 打开开关进行接收
		  }
	    
       GpioDataRegs.GPACLEAR.bit.GPIO13=1;  //拉低  下降沿触发外部中断 开始采样-->>>DMA传输 并会进入DMA中断

	}

	//CpuTimer0Regs.TCR.bit.TSS = 0;    //开定时器0中断   //TSS位 写0启动/重启；写1关闭
	
	
	//20180322_close......
	/*
	if(int1_count==10)   
	{
		GpioDataRegs.GPASET.bit.GPIO13=1;  				//10*t0 之后 拉高 为下次触发外部中断做准备
		AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x0; 		//10*t0 之后禁止ADC_seq中断
		CpuTimer0Regs.TCR.bit.TSS = 1;    //关中断  	//2017.07.13comment：写1 停止定时器
	}
    */
   // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
  	// Restore registers saved:
	DINT;
	PieCtrlRegs.PIEIER1.all = TempPIEIER;        //2017.07.13comment：@PIEIER1 PIE中断组1使能寄存器
      
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
 //   asm ("      ESTOP0");
 //   for(;;);
}     
#endif

// -----------------------------------------------------------
// PIE Group 7 - MUXed into CPU INT7
// -----------------------------------------------------------
// INT7.1
#if (G71PL != 0)
interrupt void DINTCH1_ISR(void)     // DMA
{
    Uint16 i=0,m=0;//,t=0;
    float fft_sum = 0.0;
	//float fft_ref,v_time;             //2017.07.14 add!!!
	//Uint16 vm=0;
	//static int Antenna_Num=0;
	//static int q=0;
	//static float correl_freq[2][80]={0};     //保存自相关数值  一个频点上两根天线各发射一次 
    //static float CFFT_mag[CFFT_SIZE] = {0.0};
    float N_value = 0.0;
  	
  	volatile Uint16 TempPIEIER = PieCtrlRegs.PIEIER6.all;      //2017.07.14 comment：DMA属于Group7 为什么不是IER7？？？
	IER |= M_INT7;
	IER	&= MINT7;	  	               // Set "global" priority
	PieCtrlRegs.PIEIER6.all &= MG71;   // Set "group"  priority	
	PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts	
	EINT;

  // Insert ISR Code here
   
   GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1;         //拉低
   //20180201:采样结束之后 关闭开关
   if(flag)
   {
   	  GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;
   }
   else
   {
	  GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;
   }

   GpioDataRegs.GPASET.bit.GPIO13=1;  				  //采样结束之后拉高 为下次触发外部中断做准备
   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x0; 			 //采样结束之后禁止ADC_seq中断
    
   AdcRegs.ADCST.bit.INT_SEQ1_CLR = 0x1; //清除ADC序列转换标志
  // Clear input buffers:
	 for(i=0; i < (CFFT_SIZE*2)-1; i=i+2)      // CFFT_SIZE ==64
		{
			CFFTin1Buff[i] = 0.0f;
			CFFTin1Buff[i+1] = 0.0f;
			//CFFTin2Buff[i] = 0.0f;
			//CFFTin2Buff[i+1] = 0.0f;
			CFFToutBuff[i] = 0.0f;
			CFFToutBuff[i+1] = 0.0f;
		} 
  // To receive more interrupts from this PIE group, acknowledge this interrupt
        for (i=0; i<BUF_SIZE; i++)
        {
             ADC[i] =(float) DMABuf1[i] * 3.0 / 4095.0 ;
        }
			
	    for(i=0,m=0;i<DATA_SIZE&&m<BUF_SIZE;i++,m++)     //2017.07.14comment：将电压值（float）传入cfft的输入数组以便做cfft
	    {
		    CFFTin1Buff[m]=ADC[i];        //ADC通道1 采样得到的电压 作为实部
		    //CFFTin2Buff[m]=CFFTin1Buff[m];
		    m++; 
		    CFFTin1Buff[m]=ADC[i+DATA_SIZE]; //ADC通道2 采样得到的电压 作为虚部
	        //CFFTin2Buff[m]=CFFTin1Buff[m];
	    }
	 
		cfft.CoefPtr = CFFTF32Coef;				//Twiddle factor table
		cfft.InPtr   = CFFTin1Buff;				//Input/output or middle stage of ping-pong buffer
		cfft.OutPtr  = CFFToutBuff;				//Output or middle stage of ping-pong buffer
		cfft.Stages  = CFFT_STAGES;				// FFT stages
		cfft.FFTSize = CFFT_SIZE;				// FFT size
		
  	    CFFT_f32_sincostable(&cfft);            // Calculate twiddle factor
	    CFFT_f32(&cfft);						// Calculate FFT
	    CFFT_f32_mag(&cfft);					// Calculate magnitude, result stored in CurrentOutPtr

		/*
		for(i=0;i<CFFT_SIZE;i++)
		{
			CFFT_mag[i] = CFFToutBuff[i];
		}
		*/
			  ///*	//开机预存储，无标签信号用作比对
			   
			   /*
			    ADC_END=1;    
				if(ADC_END&&loop) //loop只zuo1ci //2017.07.14 comment:表示只做一次环境噪声收集
				{
					for(j=0;j<BUF_SIZE;j++)
					{
						init_data[j]=ADC_Result[j];
					}
					loop=0;
					for(j=0;j<CFFT_SIZE;j++)
					     mag_first[j]= CFFTin1Buff[j]; 			//64个单通道采样点对应fft_mag在CFFTin1buff中
					for(i=1;i<DATA_SIZE;i++) 
						fft_ref+=mag_first[i] ;
					ADC_END=0;
				}
				else 
				{
				   for(j=0;j<BUF_SIZE;j++)
				   	  v_time+=pow(ADC_Result[j]-2.0,2);
				}
				if(v_time>2.0)
					alarm_flag++;
			  */
			/*
			for(i=0;i<DATA_SIZE;i++)
				mag_last[i]=CFFTin1Buff[i];
			*/

			for(i=2;i<6;i++)
				fft_sum+=CFFTin1Buff[i];               //求完CFFT之后 取前四个点的幅度值作为判断依据 除掉第一个点 是直流分量
		    FFT_Array[freq_num] = fft_sum;
			//AGC_Adjust(FFT_Array[freq_num]);    	 //动态调整ADC阈值门限
			
			if(FFT_Array[freq_num]>28.5)
			{
			    if(freq_num>=1&&freq_num<=78&&FFT_Array[freq_num-1]>20&&FFT_Array[freq_num+1]>20)   //
				{
					alarm_flag++;
				}
			}
			
			//AGC_Adjust(FFT_Array[freq_num]);     //动态调整ADC阈值门限

		    fft_sum=0;  
		    //vm=0;
			N_value=mag_last[0];
        	//correl_freq[sample_count1][freq_num]=N_value;   	 //取出FFT之后的直流分量
			 
			 //1
			 
			 /*
				if(freq_num==79)     		//所有频点都发射结束  
				{       
				     
		            min[q]=correl_freq[Antenna_Num][0];               		 //一轮只能找出一组的最小值
				    for(i=1;i<freq_num+1;i++)              					//找出correl_freq[][]的最小值
				    	if(correl_freq[Antenna_Num][i]<min[q]) 
				   	 	{
				    		min[q]=correl_freq[Antenna_Num][i];
				    		mark[q]=i;
				    	}
					if(min[q]<155)         		//找出所有频点下CFFT的直流分量的最小值
						q++;
					else 
					{
					    Antenna_Num++;
					    if(Antenna_Num>1)
					    	Antenna_Num=0;
					}
					if(q==2)
					{
					    if(fabs(mark[0]-mark[1])<2)     //如果是相邻的两个频点直流分量 就锁频
					    {
							//mode=1;
						    //alarm_flag++;
							freq_mark=mark[q-1];
		                    hold=START_FREQ-freq_mark*0x0002;
		                    mcbspb_xmit(hold,0);
						}
					    q=0;
					}
			        	
	                 //  	max=0;
					//	mark=0;
				}  //end if(freq_num==79)
		  	 //1
			 */

    GpioDataRegs.GPBTOGGLE.bit.GPIO41 = 1;              //拉高 
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;	
  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;

	// Restore registers saved:
	DINT;
	PieCtrlRegs.PIEIER6.all = TempPIEIER;
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
//   asm ("      ESTOP0");
//   for(;;);
}
#endif
/*void  findArray_MaxMin(float *data_avrage,int size,float max,float min)
{
    int p_max[10]={0}；
    int p_min[10]={0};  //最大、最小值对应的点
	int hold_count=0;
    max=min=*data_avrage;
    for (i=0; i<size; i++)
	{
		if(*(data_avrage+i)<min){min=*(data_avrage+i);p_min=i;}
		if(*(data_avrage+i)>max){max=*(data_avrage+i);p_max=i;}
	}
	if(top_old&&fabs(top-top_old)<3)flag=1;
	top_old=top; 
}
*/
//---------------------------------------------------------------------------
// Catch All Default ISRs: 
//

interrupt void EMPTY_ISR(void)  // Empty ISR - only does a return.
{

}

interrupt void PIE_RESERVED(void)  // Reserved space.  For test.
{
  asm ("      ESTOP0");
  for(;;);
}

interrupt void INT_NOTUSED_ISR(void)  // Reserved space.  For test.
{
  asm ("      ESTOP0");
  for(;;);
}

interrupt void rsvd_ISR(void)      // For test
{
  asm ("      ESTOP0");
  for(;;);
}

//===========================================================================
// No more.
//===========================================================================
