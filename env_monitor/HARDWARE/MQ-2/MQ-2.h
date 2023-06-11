#ifndef __MQ2_H
#define __MQ2_H

//#include "app.h"
#include "stm32f10x.h"

#define MQ2_Pin 	GPIO_Pin_7
#define MQ2_Channel ADC_Channel_7

uint16_t GetadcValue(uint8_t ch);
uint16_t readMQ2(void);
void MQ2_Init(void);

#endif

