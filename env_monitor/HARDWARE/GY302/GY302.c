#include "GY302.h"

uint8_t dataBUF[3];

void SDA_Input_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

void SDA_Output_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

void SDA_Output( uint8_t val )
{
	if ( val ) {
		GPIO_SetBits(I2C_PORT,SDA_Pin);
	} else {
		GPIO_ResetBits(I2C_PORT,SDA_Pin);
	}
}


void SCL_Output( uint8_t val )
{
	if ( val ) {
		GPIO_SetBits(I2C_PORT,SCL_Pin);
	} else {
		GPIO_ResetBits(I2C_PORT,SCL_Pin);
	}
}

uint8_t SDA_Input()
{
	return GPIO_ReadInputDataBit( I2C_PORT, SDA_Pin);
}

/***开始信号**/
void GY302_Start()
{
    SDA_Output(1);                   //拉高数据线
    SCL_Output(1);                   //拉高时钟线
    delay_us(5);
    SDA_Output(0);                   //产生下降沿
    delay_us(5);
    SCL_Output(0);                   //拉低时钟线
}

/*****停止信号******/
void GY302_Stop()
{
    SDA_Output(0);                   //拉低数据线
    SCL_Output(1);                   //拉高时钟线
    delay_us(5);
    SDA_Output(1);                   //产生上升沿
    delay_us(5);
}
/*****初始化GY302******/
void GY302_Init()
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SDA_Pin | SCL_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

//连续读出GY302内部数据
void mread(void)
{
    uint8_t i;
    GY302_Start();                          //起始信号
    GY302_SendByte(SlaveAddress+1);         //发送设备地址·+读信号

     for (i=0; i<3; i++)                      //连续读取6个地址数据到dataBUF
    {
        dataBUF[i] = GY302_RecvByte();
        if (i == 3)
        {
           GY302_SendACK(1);                //最后一个数据需要回NOACK
        }
        else
        {
          GY302_SendACK(0);                //回应ACK
        }
   }

    GY302_Stop();                          //停止信号
    delay_us(5);

}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void GY302_SendACK(int ack)
{
    SDA_Output_Mode();

    if(ack == 1)   //写应答信号
        SDA_Output(1);
    else if(ack == 0)
        SDA_Output(0);
    else
        return;

    SCL_Output(1);
    delay_us(5);
    SCL_Output(0);
    delay_us(5);
}

/**************************************
接收应答信号
**************************************/
uint8_t GY302_RecvACK()
{
	uint8_t mcy = 0;

    SDA_Input_Mode();

    SCL_Output(1);              //拉高时钟线
    delay_us(5);

    if(SDA_Input() == 1 )//读应答信号
        mcy = 1 ;
    else
        mcy = 0 ;

    SCL_Output(0);                    //拉低时钟线
    delay_us(5);

    SDA_Output_Mode();

    return mcy;
}

/**************************************
向iic总线发送一个字节数据
**************************************/
void GY302_SendByte(uint8_t dat)
{
    uint8_t i;

    for (i=0; i<8; i++)         //8位计数器
      {
        if( 0X80 & dat )
            SDA_Output(1);
        else
            SDA_Output(0);

        dat <<= 1;
        SCL_Output(1);               //拉高时钟线
        delay_us(5);
        SCL_Output(0);                //拉低时钟线
        delay_us(5);
      }
    GY302_RecvACK();
}

//我们对GY302发送命令时，要先发送器件地址+写入位，然后发送指令
//读取数据的时候，需要先发送器件地址+读入位，然后读取两字节数据

//写入指令
void Single_Write_GY302(uint8_t REG_Address)//REG_Address是要写入的指令
{
    GY302_Start();                  //起始信号
    GY302_SendByte(SlaveAddress);  //发送器件地址+写信号
    GY302_SendByte(REG_Address);   //写入指令，内部寄存器地址
    GY302_Stop();                   //结束信号
}
/**************************************
从iic总线读取一个字节数据
**************************************/
uint8_t GY302_RecvByte()
{
    uint8_t i;
    uint8_t dat = 0;
    uint8_t bit;

    SDA_Input_Mode();

    SDA_Output(1);          //准备读取数据
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL_Output(1);               //拉高时钟线
        delay_us(5);

        if( SET == SDA_Input() )
            bit = 0X01;
        else
            bit = 0x00;

        dat |= bit;             //读数据

        SCL_Output(0);                //拉低时钟线
        delay_us(5);
    }

    SDA_Output_Mode();

    return dat;
}

uint32_t readGY302(void)
{
    uint16_t dis_data;
    uint16_t Value_GY_302;

    Single_Write_GY302(GY302_PWR_ON);   // power on
    Single_Write_GY302(GY302_CON_H);   // H- resolution mode
    delay_ms(180);            //延时180ms
    mread();       //连续读出数据，存储在dataBUF中
    dis_data = dataBUF[0];
    dis_data = (dis_data<<8)+dataBUF[1];//字节合成数据
    Value_GY_302 = (uint32_t)(dis_data/1.2);
    return Value_GY_302;
}
