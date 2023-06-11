#include "MQ-2.h"

void MQ2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
  
	GPIO_InitStructure.GPIO_Pin = MQ2_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}

uint16_t GetadcValue(uint8_t ch)
{
	uint16_t temp;
	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	temp = ADC_GetConversionValue(ADC1);
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
	return temp;
}	

// ÖÐÖµÂË²¨
uint16_t readMQ2(void)
{
	uint8_t i,j;
	uint16_t adcBuff[50], adc, t;
	for(i=0; i<50; i++)
	{
		adcBuff[i]= GetadcValue(MQ2_Channel);
	}
	for(i=0; i<50/2+2; i++)
	{
		for(j=i+1; j<50; j++)
		{
			if(adcBuff[i] > adcBuff[j])
			{
				t = adcBuff[i];
				adcBuff[i] = adcBuff[j];
				adcBuff[j] = t;
			}
		}
	}
	adc = (adcBuff[50/2-1] + adcBuff[50/2])/2;
	return (uint16_t)((adc/4096.0)*330);
}
