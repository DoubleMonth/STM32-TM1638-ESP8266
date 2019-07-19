#ifndef _BH1750_H
#define _BH1750_H
#include "stm32f1xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//IIC驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/6/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//IO方向设置

#define BH1750_SDA_IN()  {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=(uint32_t)8<<20;}	//PC11输入模式
#define BH1750_SDA_OUT() {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=(uint32_t)3<<20;} 	//PC11输出模式

#define BH1750_SCL_Pin GPIO_PIN_12
#define BH1750_SCL_GPIO_Port GPIOB
#define BH1750_SDA_Pin GPIO_PIN_13
#define BH1750_SDA_GPIO_Port GPIOB

#define BH1750_ADDR    0x46
#define BH1750_READ 	0X01



//IIC所有操作函数
void BH1750_IIC_Init(void);                //初始化IIC的IO口				 
void BH1750_IIC_Start(void);				//发送IIC开始信号
void BH1750_IIC_Stop(void);	  			//发送IIC停止信号
void BH1750_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t BH1750_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t I7020_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void BH1750_IIC_Ack(void);					//IIC发送ACK信号
void BH1750_IIC_NAck(void);				//IIC不发送ACK信号

void singleWriteBH1750(uint8_t REG_Address);
void multipleReadBH1750(void);
#endif

