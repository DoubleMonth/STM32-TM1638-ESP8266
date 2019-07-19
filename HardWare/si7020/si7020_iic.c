#include "stdio.h" 
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "si7020/si7020_iic.h"
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
void delay_us(uint32_t us)
{
	uint32_t i,j;
	for(i=0;i<us;i++)
	{
		for(j=0;j<100;j++)
		{};
	}
}
//IIC��ʼ��
void SI7020_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();   //ʹ��GPIOCʱ��
    
    //PC11,12��ʼ������
    GPIO_Initure.Pin|=SI7020_SCL_Pin|SI7020_SDA_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
}
//void SDA_OUT(void)
//{
//	GPIO_InitTypeDef GPIO_Initure;
//	GPIO_Initure.Pin=SI7020_SDA_Pin;
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
//    GPIO_Initure.Pull=GPIO_PULLUP;          //����
//    HAL_GPIO_Init(SI7020_SDA_GPIO_Port,&GPIO_Initure);
//}
//void SDA_IN(void)
//{
//	GPIO_InitTypeDef GPIO_Initure;
//	GPIO_Initure.Pin=SI7020_SDA_Pin;
//    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //�������
//    GPIO_Initure.Pull=GPIO_PULLUP;          //����
//    HAL_GPIO_Init(SI7020_SDA_GPIO_Port,&GPIO_Initure);
//}

//����IIC��ʼ�ź�
void SI7020_IIC_Start(void)
{
	SI7020_SDA_OUT(); 	         //sda�����
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
	delay_us(4);
 	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void SI7020_IIC_Stop(void)
{
	SI7020_SDA_OUT();//sda�����
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t SI7020_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SI7020_SDA_IN();      //SDA����Ϊ����  
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
	delay_us(1);	   
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	delay_us(1);	 
	while(HAL_GPIO_ReadPin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			SI7020_IIC_Stop();
			return 1;
		}
	}
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void SI7020_IIC_Ack(void)
{
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
	SI7020_SDA_OUT();
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_RESET);
	delay_us(2);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	delay_us(2);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
}
//������ACKӦ��		    
void SI7020_IIC_NAck(void)
{
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
	SI7020_SDA_OUT();
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
	delay_us(2);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	delay_us(2);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void SI7020_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SI7020_SDA_OUT(); 	    
    HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_RESET);
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
		delay_us(2); 
		HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t SI7020_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SI7020_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
        delay_us(2);
		HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
        receive<<=1;
        if(HAL_GPIO_ReadPin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin))receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        SI7020_IIC_NAck();//����nACK
    else
        SI7020_IIC_Ack(); //����ACK   
    return receive;
}


uint8_t si70xx_buff[1];
uint8_t si70xx_data[2];
uint8_t si7020Measure(double *temperature, double *humidity)
{
	uint16_t value=0;
	double temp;
	SI7020_IIC_Start();
	SI7020_IIC_Send_Byte(SI7020_ADDR);
	SI7020_IIC_Wait_Ack();
//	si70xx_buff[0] = 0xE3;
	SI7020_IIC_Send_Byte(0xE3);
	SI7020_IIC_Wait_Ack();
	SI7020_IIC_Start();
	SI7020_IIC_Send_Byte(SI7020_ADDR|SI7020_READ);
	SI7020_IIC_Wait_Ack();
	delay_us(500);
	si70xx_data[1] = SI7020_IIC_Read_Byte(1);
	si70xx_data[0] = SI7020_IIC_Read_Byte(0);
	SI7020_IIC_Stop();
	value = si70xx_data[0] |(si70xx_data[1]<<8);
	temp = (double)value;
	temp = (temp / 65536.0f) * 175.72f - 46.85f;
	
	*temperature = temp;
	printf("temp");
	
}

