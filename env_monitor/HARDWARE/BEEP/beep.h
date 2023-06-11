#ifndef __BEEP_H
#define __BEEP_H

#include "sys.h"
//#include "stm32f10x.h"

#define BEEP_Port  GPIOA
#define BEEP_Pin   GPIO_Pin_15

#define BEEP_ON()  PAout(15)=1;
#define BEEP_OFF()  PAout(15)=0;

void BEEP_Init(void);

#endif
