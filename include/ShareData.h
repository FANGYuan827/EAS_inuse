#ifndef SHAREDATA_H
#define SHAREDATA_H

#include"FPU.h"
#define ADC_CKPS   0x2   // 改ADC module clock = HSPCLK/2*ADC_CKPS   = 25.0MHz/(3) = 8.3MHz 没验证
#define ADC_SHCLK  0x1   // 改 S/H width in ADC module periods  fs=8.3/2+1=2.76M                       = 16 ADC clocks
#define AVG        1000  // Average sample limit
#define ZOFFSET    0x00  // Average Zero offset
#define BUF_SIZE   64*4*2    // Sample buffer size
#define DATA_SIZE 64*4
#define FREQ_SIZE 16

//#define START_FREQ 0x4AE5      //@0x4AE5
#define START_FREQ 0x4A7E		//@0x4A7E 8.197MHz

#define START_GAIN 60
#define MAX_GAIN 79
// FFT Parameters
#define CFFT_STAGES 8  //点数为2^6=64 -->>> 2^8=256
#define CFFT_SIZE (1 << CFFT_STAGES)
//static float ADC_Result[BUF_SIZE];
extern float  	CFFTin1Buff[CFFT_SIZE*2];                  //Output of FFT overwrites input if                                                //CFFT_STAGES is OD
extern float	CFFTin2Buff[CFFT_SIZE*2];               //Additional Buffer used in Magnitude calc
extern float	CFFToutBuff[CFFT_SIZE*2];               //Output of FFT here if CFFT_STAGES is EVEN
extern float	CFFTF32Coef[CFFT_SIZE];                 //Twiddle buffer

//extern float ADC_Result[BUF_SIZE];

extern volatile float ADC[BUF_SIZE];

extern volatile Uint16 DMABuf1[BUF_SIZE];
extern volatile Uint16 *DMADest;
extern volatile Uint16 *DMASource;
extern CFFT_F32_STRUCT cfft;

//extern Uint16 sample_count1;
extern Uint16 sample_count2;
extern Uint16 conside_tag;
extern Uint16 j ;
extern Uint16 ADC_END ; // ADC finish flag
extern float mag_first[DATA_SIZE];
extern float mag_last[DATA_SIZE];


extern Uint16 freq_num,array_index,flag,freq_mark;
extern Uint32 int1_count;
extern Uint32 int2_count;
extern Uint16 alarm_flag,alarm_flag2;
extern Uint16 loop,mode;
extern Uint16 test_num;
extern Uint16 alarm_count;
extern float init_mean;
extern float init_data[BUF_SIZE];
extern Uint32 freq;


#endif /*TIMEADJUST_H*/



