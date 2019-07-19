#ifndef _SI7020_H
#define _SI7020_H
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

#define SI7020_SDA_IN()  {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=(uint32_t)8<<12;}	//PC11输入模式
#define SI7020_SDA_OUT() {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=(uint32_t)3<<12;} 	//PC11输出模式

#define SI7020_SCL_Pin GPIO_PIN_12
#define SI7020_SCL_GPIO_Port GPIOA
#define SI7020_SDA_Pin GPIO_PIN_11
#define SI7020_SDA_GPIO_Port GPIOA

#define SI7020_ADDR    0x80
#define SI7020_READ 	0X01
#define SI7020_TEMPERATURE      0xF3
#define SI7020_HUMIDITY         0xF5
#define SI7020_SOFT_RESET       0xFE

#define SI7020_CTRL_GET_TH      0x7F
#define SI7020_CTRL_RESET       0x7E
#define SI7020_CTRL_TEMP_CMD   0X7D
#define SI7020_CTRL_GET_TEMP    0X7C
#define SI7020_CTRL_HUMI_CMD    0X7B
#define SI7020_CTRL_GET_HUMI    0X7A


//IIC所有操作函数
void SI7020_IIC_Init(void);                //初始化IIC的IO口				 
void SI7020_IIC_Start(void);				//发送IIC开始信号
void SI7020_IIC_Stop(void);	  			//发送IIC停止信号
void SI7020_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t SI7020_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t I7020_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void SI7020_IIC_Ack(void);					//IIC发送ACK信号
void SI7020_IIC_NAck(void);				//IIC不发送ACK信号

void SI7020_IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t SI7020_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	
uint8_t si7020Measure(double *temperature, double *humidity);
#endif

