#include "Myfunction.h"
#include "ShareData.h"
#include "My_Config.h"

//定义一个函数 找出FFT的最大幅度值
float FindMax(float *inputbuf,size_t length)
{
	float max_tmp = inputbuf[1];
	int i=1;                        
	if(inputbuf==NULL||length==0)
	{
		return 0.0;
	}
    for(i=2;i<length;i++)
	{
		if(max_tmp<inputbuf[i])
		{
			max_tmp = inputbuf[i];
		}
	}
	return max_tmp;
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


