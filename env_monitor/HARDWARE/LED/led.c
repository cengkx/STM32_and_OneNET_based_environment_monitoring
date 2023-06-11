#include "led.h"
#include "stm32f10x.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	
 	GPIO_InitStructure.GPIO_Pin = LED1_Pin;				 //PA1端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(LED_Port, &GPIO_InitStructure);				 //初始化IO口
 	GPIO_SetBits(LED_Port,LED1_Pin);						 //PA1 输出高
			    
}

