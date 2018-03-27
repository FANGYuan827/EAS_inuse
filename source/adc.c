#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "ShareData.h"
void ADC_Config()
{
   AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;
   AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
   AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // 0 Non-Cascaded Mode; 1 Cascaded Mode级联
   AdcRegs.ADCTRL1.bit.CONT_RUN = 1; /////连续模式
//   AdcRegs.ADCTRL1.bit.SEQ_OVRD = 1;
   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1;
 //  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0x1;
 //  AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ=0x1;  //级联可配置

   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;
   AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;
   AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0x0001;   // Set up ADC to perform 4 conversions for every SOC
}
