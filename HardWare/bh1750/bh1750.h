#ifndef _BH1750_H
#define _BH1750_H
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

#define BH1750_SDA_IN()  {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=(uint32_t)8<<20;}	//PC11����ģʽ
#define BH1750_SDA_OUT() {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=(uint32_t)3<<20;} 	//PC11���ģʽ

#define BH1750_SCL_Pin GPIO_PIN_12
#define BH1750_SCL_GPIO_Port GPIOB
#define BH1750_SDA_Pin GPIO_PIN_13
#define BH1750_SDA_GPIO_Port GPIOB

#define BH1750_ADDR    0x46
#define BH1750_READ 	0X01



//IIC���в�������
void BH1750_IIC_Init(void);                //��ʼ��IIC��IO��				 
void BH1750_IIC_Start(void);				//����IIC��ʼ�ź�
void BH1750_IIC_Stop(void);	  			//����IICֹͣ�ź�
void BH1750_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t BH1750_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t I7020_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void BH1750_IIC_Ack(void);					//IIC����ACK�ź�
void BH1750_IIC_NAck(void);				//IIC������ACK�ź�

void singleWriteBH1750(uint8_t REG_Address);
void multipleReadBH1750(void);
#endif

