#ifndef __LED_H
#define __LED_H

#define LED1_Pin GPIO_Pin_1
#define LED_Port GPIOA

#define LED1_ON()  PAout(1)=0;
#define LED1_OFF()  PAout(1)=1;

void LED_Init(void);

#endif
