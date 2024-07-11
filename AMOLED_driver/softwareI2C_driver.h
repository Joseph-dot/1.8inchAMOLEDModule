#ifndef SOFTWAREI2C_DRIVER_H
#define SOFTWAREI2C_DRIVER_H

#include "main.h"
#include "delay.h"

/* I2C_SCL GPIO Write */
#define I2C_SCL_H HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET)
#define I2C_SCL_L HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_RESET)
/* I2C_SDA GPIO Write */
#define I2C_SDA_H HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET)
#define I2C_SDA_L HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_RESET)

/* I2C_SDA GPIO Read */
#define I2C_READ_SDA HAL_GPIO_ReadPin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin)

//I2C所有操作函数
void I2C_Init(void);                //初始化I2C的IO口				 
void I2C_Start(void);				//发送I2C开始信号
void I2C_Stop(void);	  			//发送I2C停止信号
void I2C_Send_Byte(uint8_t Byte);			//I2C发送一个字节
uint8_t I2C_Read_Byte(unsigned char ack);//I2C读取一个字节
uint8_t I2C_Wait_Ack(void); 				//I2C等待ACK信号
void I2C_Ack(void);					//I2C发送ACK信号
void I2C_NAck(void);				//I2C不发送ACK信号

void I2C_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t I2C_Read_One_Byte(uint8_t daddr,uint8_t addr);



#endif	/* softwareI2C_driver! */
