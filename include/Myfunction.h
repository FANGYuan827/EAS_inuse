#ifndef __MY_FUNCTION_H
#define __MY_FUNCTION_H

#define NULL 0x00000000
#define size_t int
float FindMax(float *inputbuf,size_t length);

//AGC��̬����
void AGC_Adjust(float input);   //���������CFFT�ķ���ֵ֮��

#endif
