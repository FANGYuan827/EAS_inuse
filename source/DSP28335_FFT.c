// self program: /fft_main/4 $

//###########################################################################
//
// FILE:	DSP28335_FFT.c
//
// TITLE:	DSP28335_FFT main.
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.01 $
// $Release Date: September 26, 2007 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


// FFT Parameters
#define CFFT_STAGES 6  //µãÊýÎª64
#define CFFT_SIZE (1 << CFFT_STAGES)
//Buffer alignment for the input array,
//CFFT_f32u (optional), CFFT_f32 (required)
//Output of FFT overwrites input if
//CFFT_STAGES is ODD
#pragma DATA_SECTION(CFFTin1Buff,"CFFTdata1");
float CFFTin1Buff[CFFT_SIZE*2];                  //Output of FFT overwrites input if 
                                                //CFFT_STAGES is ODD

#pragma	DATA_SECTION(CFFTin2Buff,"CFFTdata2");  
float	CFFTin2Buff[CFFT_SIZE*2];               //Additional Buffer used in Magnitude calc

#pragma	DATA_SECTION(CFFToutBuff,"CFFTdata3");	
float	CFFToutBuff[CFFT_SIZE*2];               //Output of FFT here if CFFT_STAGES is EVEN

#pragma	DATA_SECTION(CFFTF32Coef,"CFFTdata4"); 
float	CFFTF32Coef[CFFT_SIZE];                 //Twiddle buffer
CFFT_F32_STRUCT cfft;

void Initfft(void)
{
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
}
//===========================================================================
// End of file.
//===========================================================================
