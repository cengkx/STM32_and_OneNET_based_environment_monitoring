#ifndef __BT_H
#define __BT_H
#include "stdio.h"	
#include "sys.h" 

#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	

#define USART_BT		USART2		//调试打印所使用的串口组 

void uart2_init(u32 bound);
void USART2_SendString(u8* Str);


#endif
