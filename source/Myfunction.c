#include "Myfunction.h"
#include "ShareData.h"
#include "My_Config.h"

//����һ������ �ҳ�FFT��������ֵ
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


/*-------------��̬����AGC������----------------------*/
void AGC_Adjust(float input)     			/*@input��ʾCFFT�ķ���ֵ֮��*/
{
	int AGC_GAIN = 0;
	if(input<0.0)
	{
		return;
	}

	if(input<=10)             		 //��ֵ1 
	{
		AGC_GAIN = MAX_GAIN;
		mcbspa_AGC(AGC_GAIN);
		goto lable;
	}

	if(input<=20)               	 //��ֵ2
	{
		AGC_GAIN = START_GAIN+5;
		mcbspa_AGC(AGC_GAIN);
		goto lable;
	}

	if(input<=30)            		//��ֵ3
	{
		AGC_GAIN = START_GAIN-5;
		mcbspa_AGC(AGC_GAIN);
		goto lable;	
	}

	if(input>30)		           //��ֵ4
	{
		AGC_GAIN = START_GAIN-10;
		mcbspa_AGC(AGC_GAIN);	
	}

lable:
	AGC_GAIN = 0;

}


