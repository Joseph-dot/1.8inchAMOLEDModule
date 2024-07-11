#include "softwarei2c_driver.h"

void Soft_I2C_Delay(void)
{
	delay_us(2);
}

//I2C初始化
void I2C_Init(void)
{
	I2C_SCL_H;
	I2C_SDA_H;
}

//产生I2C起始信号
/*    _____
 *SDA      \_____________
 *    __________
 *SCL           \________
*/
void I2C_Start(void)
{
	I2C_SDA_H;
	I2C_SCL_H;
	Soft_I2C_Delay();
	I2C_SDA_L; //START:when CLK is high,DATA change form high to low
	Soft_I2C_Delay();
	I2C_SCL_L; //钳住I2C总线，准备发送或接收数据
	Soft_I2C_Delay();
}	  

//产生I2C停止信号
/*               _______
 *SDA __________/
 *          ____________
 *SCL _____/
*/
void I2C_Stop(void)
{
	I2C_SDA_L; //STOP:when CLK is high DATA change form low to high
	Soft_I2C_Delay();
	I2C_SCL_H;
	Soft_I2C_Delay();
	I2C_SDA_H; //发送I2C总线结束信号
	Soft_I2C_Delay();			   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t I2C_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	uint8_t rack = 0;
	
	I2C_SDA_H;
	Soft_I2C_Delay();
	I2C_SCL_H;
	Soft_I2C_Delay();
	while (I2C_READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			I2C_Stop();
			rack = 1;
			break;
		}
		Soft_I2C_Delay();
	}
	I2C_SCL_L; //时钟输出0
	Soft_I2C_Delay();

	return rack;
} 

//产生ACK应答
/*           ____
 *SCL ______/    \______
 *    ____         _____
 *SDA     \_______/
*/
void I2C_Ack(void)
{
	I2C_SDA_L;
	Soft_I2C_Delay();
	I2C_SCL_H;
	Soft_I2C_Delay();
	I2C_SCL_L;
	Soft_I2C_Delay();
	I2C_SDA_H;
	Soft_I2C_Delay();
}

//不产生ACK应答		
/*           ____
 *SCL ______/    \______
 *    __________________
 *SDA
*/
void I2C_NAck(void)
{
	I2C_SDA_H;
	Soft_I2C_Delay();
	I2C_SCL_H;
	Soft_I2C_Delay();
	I2C_SCL_L;
	Soft_I2C_Delay();
}	

//I2C发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void I2C_Send_Byte(uint8_t Byte)
{                        
  uint8_t i;    
	for(i = 0; i < 8; i++) {
		if(Byte & 0x80) {
			I2C_SDA_H;
		} else{
			I2C_SDA_L;
		}
		Soft_I2C_Delay();
		I2C_SCL_H;
		Soft_I2C_Delay();
		I2C_SCL_L;
		if(i == 7) {
			I2C_SDA_H;
		}
		Byte <<= 1;
		Soft_I2C_Delay();
	}
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t I2C_Read_Byte(uint8_t ack)
{
	uint8_t i, receive=0;
  for(i = 0; i < 8; i++ )
	{
		receive <<= 1;
		I2C_SCL_H;
		Soft_I2C_Delay();
		if(I2C_READ_SDA) receive++;
		I2C_SCL_L;
		Soft_I2C_Delay(); 
  }					 
	if (!ack)
		I2C_NAck();//发送nACK
	else
		I2C_Ack(); //发送ACK   
  return receive;
}