// =====================================================================================
//  This software is licensed for use with Texas Instruments C28x
//  family DSCs.  This license was provided to you prior to installing
//  the software.  You may review this license by consulting a copy of
//  the agreement in the doc directory of this library.
// -------------------------------------------------------------------------------------
//          Copyright (C) 2011-2012 Texas Instruments, Incorporated.
//                          All Rights Reserved.
//======================================================================================
//
//  FILE:    Test_FPU_ComplexAbsVec.c
//
//  TITLE:   Absolute value of a complex vector test   
//
//  AUTHOR:  David M. Alter, Texas Instruments Inc.
//
//  HISTORY: 
//      12/18/07 - original (D. Alter)
//
//######################################################################################
// $TI Release: C28x Floating Point Unit Library V1.31 $
// $Release Date: Sep 10, 2012 $
//######################################################################################

#include "DSP2833x_Device.h"      // Device Headerfile and Examples Include File
#include "ShareData.h"
//extern float32 y[DATA_SIZE]={0};
#define m_len 15
extern complex_float x[DATA_SIZE];
extern Uint16 sample_count1;
extern float ADC_SUM[m_len*2][DATA_SIZE];                                   //2017.08.08 modify:10->16
void adc_Complexabs(float32 *y,float *adc,const Uint16 N)
{
   int i;
// Create some dummy test data
    for(i=0;i<N;i++){
    	x[i].dat[0]=*(adc+i);  // x_re
		x[i].dat[1]=*(adc+i+64); // x_im
    }

// Absolute value of a complex vector test
// Exected result: y = [3.60555, 3.16228, 4.83011, 7.38241
//                      0.0      7.38241, 7.38241, 0.0
//                      0.0,     0.0]
/*    asm(" NOP");
	abs_SP_CV(y, x, N);			 	    // complex absolute value
    asm(" NOP");
*/
    asm(" NOP");
    abs_SP_CV_2(y, x, N);               // complex absolute value //2017.07.19COMMENT! 这个绝对不能注释掉！！！
    asm(" NOP");
	for(i=0;i<DATA_SIZE;i++)                    //2017.07.19 MODIFY N->DATA_SIZE
	ADC_SUM[sample_count1][i]=*(y+i);
} // end of main()

//===========================================================================
// End of File
//===========================================================================
