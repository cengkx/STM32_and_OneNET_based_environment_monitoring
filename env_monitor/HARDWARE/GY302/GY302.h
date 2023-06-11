#ifndef __GY302_H
#define __GY302_H

//#include "app.h"
#include "stm32f10x.h"
#include "delay.h"

#define I2C_PORT GPIOA
#define SDA_Pin	GPIO_Pin_6
#define SCL_Pin GPIO_Pin_5   

#define SlaveAddress 0x46  //ADDR接GND时的器件地址
#define GY302_PWR_DOWN     0x00 //关闭模块
#define GY302_PWR_ON       0x01 //打开模块等待测量命令
#define GY302_RST          0x07 //重置数据寄存器值在PowerOn模式下有效
#define GY302_CON_H        0x10 // 连续高分辨率模式,1lx,120ms
#define GY302_CON_H2       0x11 // 连续高分辨率模式.5lx,120ms
#define GY302_CON_L        0x13 // 连续低分辨率模式,4lx,16ms
#define GY302_ONE_H        0x20 // 一次高分辨率模式,1lx,120ms,测量后模块转到PowerDown模式
#define GY302_ONE_H2       0x21 // 一次高分辨率模式,0.5lx,120ms,测量后模块转到PowerDown模式
#define GY302_ONE_L        0x23 // 一次低分辨率模式,4lx,16ms,测量后模块转到PowerDown模式

void GY302_Start(void);
void GY302_Stop(void);
void mread(void);
void Delay_mms(uint16_t tmp);
void GY302_SendACK(int ack);
uint8_t GY302_RecvACK(void);
void GY302_SendByte(uint8_t dat);
uint8_t GY302_RecvByte(void);
int I2C_ReadData(uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData, uint16_t dataLen);
void Single_Write_GY302(uint8_t REG_Address);

void GY302_Init(void);
uint32_t readGY302(void);

#endif
