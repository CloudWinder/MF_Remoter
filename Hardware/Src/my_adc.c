#include "my_adc.h"
#include "oled.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ADC ����	   
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define ADC_SAMPLE_NUM	10

uint16_t adc_value[5*ADC_SAMPLE_NUM];//ADC�ɼ�ֵ��Ż�����
 

//��ʼ��ADC��ʹ��DMA����
//ͨ��PA0\PA1\PA3\PA4
void Adc_Init(void)
{ 	
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adc_value,50);
}				  
 
//ADC��ֵ�˲�
void ADC_Filter(uint16_t* adc_val)
{
	uint16_t i=0;
	uint32_t sum[5]={0,0,0,0};
	
	for(;i<ADC_SAMPLE_NUM;i++)
	{
		sum[0]+=adc_value[5*i+0];
		sum[1]+=adc_value[5*i+1];
		sum[2]+=adc_value[5*i+2];
		sum[3]+=adc_value[5*i+3];
		sum[4]+=adc_value[5*i+4];
	}
	adc_val[0]=sum[0]/ADC_SAMPLE_NUM;
	adc_val[1]=sum[1]/ADC_SAMPLE_NUM;
	adc_val[2]=sum[2]/ADC_SAMPLE_NUM;
	adc_val[3]=sum[3]/ADC_SAMPLE_NUM;
	adc_val[4]=sum[4]/ADC_SAMPLE_NUM;
}


uint16_t getAdcValue(uint8_t axis)
{
	uint32_t sum=0;
	for(uint8_t i=0;i<ADC_SAMPLE_NUM;i++)
	{
		sum += adc_value[5*i+axis];
	}
	return sum/ADC_SAMPLE_NUM;
}

void Adc_test(void)
{
	oled_showNum(0,32,getAdcValue(0),4,6,12);
	oled_showNum(32,32,getAdcValue(1),4,6,12);
	oled_showNum(64,32,getAdcValue(2),4,6,12);
	oled_showNum(32,48,getAdcValue(3),4,6,12);
	oled_showNum(64,48,getAdcValue(4),4,6,12);
	oled_refreshGram();
}


