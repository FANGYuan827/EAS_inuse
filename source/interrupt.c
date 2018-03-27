#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "ShareData.h"
#include "DSP2833x_SWPrioritizedIsrLevels.h"
extern Uint16 sample_count1;
void Fft_func(CFFT_F32_STRUCT *Pcfft)
{
	CFFT_f32_sincostable(Pcfft);
	CFFT_f32(Pcfft);
	CFFT_f32_mag(Pcfft);	
}



//int1.1
/* interrupt void adc_isr(void)
{   
 //   Uint16 i;
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0x1;  //��λ
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 0x1;
  //  StartDMACH1();
 //   for (i=0; i<BUF_SIZE/2; i++)
//    {
 //        ADC_Result[array_index++] = (float)((AdcRegs.ADCRESULT0)>>4) * 3.0 / 4095.0;
 //        ADC_Result[array_index++] = (float)((AdcRegs.ADCRESULT1)>>4) * 3.0 / 4095.0;
 //   }
 //   if(array_index==128)
//	array_index=0;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}  */

// INT7.1
/*interrupt void local_DINTCH1_ISR(void)     // DMA Channel 1
{
   Uint16 i,m,vm;
 //  static float ADC_Result[BUF_SIZE]={0};
   vm=0;
 //  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 0x1;
  // Clear input buffers:
	 for(i=0; i < (CFFT_SIZE*2); i=i+2)
		{
		CFFTin1Buff[i] = 0.0f;
		CFFTin1Buff[i+1] = 0.0f;
		CFFTin2Buff[i] = 0.0f;
		CFFTin2Buff[i+1] = 0.0f;
		CFFToutBuff[i] = 0.0f;
		CFFToutBuff[i+1] = 0.0f;
		} 
  // To receive more interrupts from this PIE group, acknowledge this interrupt
        for (i=0; i<BUF_SIZE; i++)
        {
             ADC[i] =(float) DMABuf1[i] * 3.0 / 4095.0 ;
             ADC_Result[i]+=ADC[i];
		    
        }
		if(sample_count1==10)
		{  
		    for (i=0; i<BUF_SIZE; i++)
            {
               ADC_Result[i] =ADC_Result[i]/10.0;
			 //  if((ADC_Result[i]>2.6)||(ADC_Result[i]<0.6)) vm++;
            }
		//	if(vm>30)alarm_flag1=1; 
            for(i=0,m=0;i<DATA_SIZE,m<BUF_SIZE;i++,m++)
            {
             CFFTin1Buff[m]=ADC_Result[i];
		     m++;
             CFFTin1Buff[m]=ADC_Result[i+DATA_SIZE];
            }
       // Note: In this version, CFFTin1Buff and CFFToutBuff are used in
//       ping-pong fashion. The input data is first stored in CFFTin1Buff.
//       The FFT is then calculated, including bit reversing, and
//       when done, the cfft.CurrentInPtr pointer points to
//       buffer which has the result. Depending on the FFT size,
//       it will either be in CFFTin1Buff or CFFToutBuff.
	
	    cfft.CoefPtr = CFFTF32Coef;				//Twiddle factor table
     	cfft.InPtr   = CFFTin1Buff;				//Input/output or middle stage of ping-pong buffer
    	cfft.OutPtr  = CFFToutBuff;				//Output or middle stage of ping-pong buffer
    	cfft.Stages  = CFFT_STAGES;				// FFT stages
    	cfft.FFTSize = CFFT_SIZE;				// FFT size
	
		Fft_func(&cfft);


   	//	CFFT_f32_sincostable(&cfft);            // Calculate twiddle factor
	
	//===========================================================================
	// CFFT result:
	//   CFFT_f32(&cfft);						// Calculate FFT

	// Note: To calculate magnitude, the input data is pointed by cfft.CurrentInPtr.
	//       The calculated magnitude is stored in the memory pointed by
	//       cfft.CurrentOutPtr. If not changing cfft.CurrentOutPtr after called
	//       magnitude calculation function, the output buffer would be overwrite
	//       right after phase calculation function called.
	//
	//       If Stages is ODD, the currentInPtr=CFFTin1Buff, currentOutPtr=CFFToutBuff
	//       If Stages is Even, the currentInPtr=CFFToutBuff, currentOutPtr=CFFTin1Buff
	//

		// Calculate Magnitude:
	//	CFFT_f32_mag(&cfft);					// Calculate magnitude, result stored in CurrentOutPtr
			ADC_END=1;    
			if(ADC_END&&loop) //loopֻzuo1ci
			{
			for(j=0;j<BUF_SIZE;j++){init_data[j]=ADC[j];}
			loop=0;
			for(j=0;j<CFFT_SIZE;j++) mag_first[j]= CFFTin1Buff[j]; //64����ͨ���������Ӧfft_mag��CFFTin1buff��
		//	v_ref= mag_first[1];
			ADC_END=0;
			}
           for (i=0; i<BUF_SIZE; i++)
            {
               ADC_Result[i] = 0.0;
            }
			for(i=0;i<DATA_SIZE;i++)mag_last[i]=CFFTin1Buff[i];
		    if(mag_last[0]<mag_first[0])
			{
				for(i=0;i<DATA_SIZE;i++)mag_first[i]=mag_last[i];
			//	if(sample_count1%5) freq_fix=sample_count1;
			//	else freq_fix=sample_count2;
			}
		   
		    //	if((mag_first[1]>2.0)&&(mag_first[0]<360))alarm_flag=1;
			    if(mag_first[61]>3.99)alarm_flag2=1;
		        
				loop=1;
			
		}//end if
    	sample_count1=sample_count2=0;
 PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;	
}
*/
/*-----------------��ʱ��0�жϺ���----------------------*/
/*interrupt void cpu_timer0_isr(void)
{
    int1_count= CpuTimer0.InterruptCount++;
	CpuTimer0Regs.TCR.bit.TSS = 1;   //���ж�
	if(int1_count==0){
       if(flag) GpioDataRegs.GPADAT.bit.GPIO3=0; //gpio30----gpio3  5us����
       else GpioDataRegs.GPADAT.bit.GPIO29=0; //5us����
	   
       GpioDataRegs.GPACLEAR.bit.GPIO13=1;  //50us����
 //      AdcRegs.ADCTRL2.bit.SOC_SEQ1=0x1;  //����ADC����
//	   StartDMACH1();
	}
	CpuTimer0Regs.TCR.bit.TSS = 0;    //����ʱ��0�ж�
	if(int1_count==10)   //5us�����پ���50us
	{
		GpioDataRegs.GPASET.bit.GPIO13=1;  //50us����
		CpuTimer0Regs.TCR.bit.TSS = 1;    //���ж�
	}

  
        
   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer1_isr(void) //1msʱ�䵽
{  
   CpuTimer1Regs.TCR.bit.TSS = 1;   //���ж�
//   mag=mag-10;
//   if(mag<0)mag=79;
//   mcbspa_AGC(0);
   int2_count=CpuTimer1.InterruptCount++;
    sample_count1++;
	if(sample_count1>10)sample_count1=0;
   
  // freq=freq-0x0009;
   
 //  if(int2_count==16){freq=0x4AE5;CpuTimer1.InterruptCount=0;}
//   mcbspb_xmit(freq,0);
   if(int2_count==4){  // 8.0<->count=0 8.12<->count=4
    
   // freq=0x4AE7-0x0009*4*freq_num++;  //���л����ʱfreq��ͷ��
    flag=1;
    GpioDataRegs.GPACLEAR.bit.GPIO15=1;
    GpioDataRegs.GPASET.bit.GPIO16=1;
	GpioDataRegs.GPADAT.bit.GPIO29=0; //5us����
    GpioDataRegs.GPASET.bit.GPIO3=1;
    }
   if(int2_count==9){
    freq=freq-0x000b;
	freq_num++;
    CpuTimer1.InterruptCount=0;  //���¿�ʼ����1,freq���ż���
    flag=0;
    GpioDataRegs.GPASET.bit.GPIO15=1;
    GpioDataRegs.GPACLEAR.bit.GPIO16=1;
    GpioDataRegs.GPADAT.bit.GPIO3=0;
    GpioDataRegs.GPASET.bit.GPIO29=1;
    }
   if(flag)GpioDataRegs.GPADAT.bit.GPIO3=1;  //5us�ø�׼��
   else GpioDataRegs.GPADAT.bit.GPIO29=1;
   GpioDataRegs.GPASET.bit.GPIO13=1;  //50us�ø�׼��
   CpuTimer0Regs.TCR.bit.TSS = 0;    //����ʱ��0�ж�
   CpuTimer0.InterruptCount=0;
   mcbspb_xmit(freq,0);
   if(freq_num==16){freq=0x4AE5;CpuTimer1.InterruptCount=0;freq_num=0;}
   CpuTimer1Regs.TCR.bit.TSS = 0;   //����ʱ��1�ж�
   // The CPU acknowledges the interrupt.
   
}
/////////////////////////////////////////////
interrupt void xint2_isr(void)
{
//	GpioDataRegs.GPBCLEAR.all = 0x4;   // GPIO34 is low
//	Xint2Count++;
//	LED=MyLed;
  //  sample_count2++;
  
//	if(sample_count2>20)sample_count1=sample_count2=0;
    led0();
//	MyLed=0xFF;
	AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1=1;    //
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0x1;
//	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1;
    StartDMACH1();		
//	 Acknowledge this interrupt to get more from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
 //////�жϷ���������////////

*/
