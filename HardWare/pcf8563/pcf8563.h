#ifndef _PCF8563_H
#define _PCF8563_H
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

#define SDA_IN()  {GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=(uint32_t)8<<4;}	//PC11输入模式
#define SDA_OUT() {GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=(uint32_t)3<<4;} 	//PC11输出模式

#define PCF8563_SCL_Pin GPIO_PIN_0
#define PCF8563_SCL_GPIO_Port GPIOA
#define PCF8563_SDA_Pin GPIO_PIN_1
#define PCF8563_SDA_GPIO_Port GPIOA

#define PCF8563_ADDR    0xA2
#define PCF8563_READ 0X01	//读数据

//控制和状态寄存器
#define CONTROL_STATUS_1 (unsigned char)0X00    
#define CONTROL_STATUS_2 (unsigned char)0X01
	
//时间和日期寄存器
#define PCF8563_SECOND_ADDRESS (unsigned char)0X02				//秒地址
#define PCF8563_MINUTE_ADDRESS (unsigned char)0X03				//分地址
#define PCF8563_HOUR_ADDRESS (unsigned char)0X04				//时地址
#define PCF8563_DAY_ADDRESS (unsigned char)0X05				//日地址
#define PCF8563_WEEK_ADDRESS (unsigned char)0X06					//星期地址
#define PCF8563_MONTH_ADDRESS (unsigned char)0X07				//月地址
#define PCF8563_YEAR_ADDRESS (unsigned char)0X08				//年地址

#define PCF8563_MINUTE_ALARM_ADDRESS 0X09				//闹钟分地址
#define PCF8563_HOUR_ALARM_ADDRESS 0X0A				//闹钟时地址
#define PCF8563_DAY_ALARM_ADDRESS 0X0B				//闹钟日地址
#define PCF8563_WEEK_ALARM_ADDRESS 0X0C					//闹钟星期地址

#define PCF8563_CLKOUT_CONTROL 0X0D				//时钟输出控制地址

//IIC所有操作函数
void PCF8563_IIC_Init(void);                //初始化IIC的IO口				 
void PCF8563_IIC_Start(void);				//发送IIC开始信号
void PCF8563_IIC_Stop(void);	  			//发送IIC停止信号
void PCF8563_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t PCF8563_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t PCF8563_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void PCF8563_IIC_Ack(void);					//IIC发送ACK信号
void PCF8563_IIC_NAck(void);				//IIC不发送ACK信号
	
uint8_t PCF8563_ReadOneByte(uint16_t ReadAddr);
void PCF8563_ReadTime(uint8_t *p_time_buf);
void PCF8563_WriteTime(void);
#endif

