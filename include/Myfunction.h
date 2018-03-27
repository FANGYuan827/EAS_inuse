#ifndef __MY_FUNCTION_H
#define __MY_FUNCTION_H

#define NULL 0x00000000
#define size_t int
float FindMax(float *inputbuf,size_t length);

//AGC动态调整
void AGC_Adjust(float input);   //输入参数是CFFT的幅度值之和

#endif
