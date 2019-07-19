#ifndef _SI7020_H
#define _SI7020_H
#include "stm32f1xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//IIC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/6/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//IO��������

#define SI7020_SDA_IN()  {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=(uint32_t)8<<12;}	//PC11����ģʽ
#define SI7020_SDA_OUT() {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=(uint32_t)3<<12;} 	//PC11���ģʽ

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


//IIC���в�������
void SI7020_IIC_Init(void);                //��ʼ��IIC��IO��				 
void SI7020_IIC_Start(void);				//����IIC��ʼ�ź�
void SI7020_IIC_Stop(void);	  			//����IICֹͣ�ź�
void SI7020_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t SI7020_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t I7020_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void SI7020_IIC_Ack(void);					//IIC����ACK�ź�
void SI7020_IIC_NAck(void);				//IIC������ACK�ź�

void SI7020_IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t SI7020_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	
uint8_t si7020Measure(double *temperature, double *humidity);
#endif

