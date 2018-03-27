#ifndef _My_Config_H
#define _My_Config_H


void ADC_Config();
void mcbspb_xmit(int a, int b);
void mcbspa_AGC(int a);
void dds_init4();                         
void error(void);
void alarm_start(void);
void alarm_stop(void);

void gpio_cfg(void);
void init_timer();
void XINTConfig();

//LED���ú�������
void led0();
void led1(void);
void led2(void);
void led3(void);
void led4(void);
void led5(void);
void led6(void);
void led7(void);
void led8(void);
void led9(void);
void led10(void);
void alarm_stop(void);

//AGC��̬����
void AGC_Adjust(float input);      //���������CFFT�ķ���ֵ֮��
#endif


