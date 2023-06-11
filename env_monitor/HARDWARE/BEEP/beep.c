#include "beep.h"
#include "stm32f10x.h"

void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //开启复用时钟
	
 	GPIO_InitStructure.GPIO_Pin = BEEP_Pin;				 //PA15端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(BEEP_Port, &GPIO_InitStructure);				 //初始化IO口
 	GPIO_ResetBits(BEEP_Port,BEEP_Pin);						 //PA15 输出低
			
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //默认作为调试接口引脚、禁用JTAG
}

