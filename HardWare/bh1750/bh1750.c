#include "stdio.h" 
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "bh1750/bh1750.h"
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
void bh1750_delay_us(uint32_t us)
{
	uint32_t i,j;
	for(i=0;i<us;i++)
	{
		for(j=0;j<100;j++)
		{};
	}
}
//IIC��ʼ��
void BH1750_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOCʱ��
    
    //PC11,12��ʼ������
    GPIO_Initure.Pin|=BH1750_SCL_Pin|BH1750_SDA_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_SET);
}
//void SDA_OUT(void)
//{
//	GPIO_InitTypeDef GPIO_Initure;
//	GPIO_Initure.Pin=BH1750_SDA_Pin;
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
//    GPIO_Initure.Pull=GPIO_PULLUP;          //����
//    HAL_GPIO_Init(BH1750_SDA_GPIO_Port,&GPIO_Initure);
//}
//void SDA_IN(void)
//{
//	GPIO_InitTypeDef GPIO_Initure;
//	GPIO_Initure.Pin=BH1750_SDA_Pin;
//    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //�������
//    GPIO_Initure.Pull=GPIO_PULLUP;          //����
//    HAL_GPIO_Init(BH1750_SDA_GPIO_Port,&GPIO_Initure);
//}

//����IIC��ʼ�ź�
void BH1750_IIC_Start(void)
{
	BH1750_SDA_OUT(); 	         //sda�����
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_SET);
	bh1750_delay_us(4);
 	HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	bh1750_delay_us(4);
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void BH1750_IIC_Stop(void)
{
	BH1750_SDA_OUT();//sda�����
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	bh1750_delay_us(4);
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_SET);//����I2C���߽����ź�
	bh1750_delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t BH1750_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	BH1750_SDA_IN();      //SDA����Ϊ����  
	HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_SET);
	bh1750_delay_us(1);	   
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_SET);
	bh1750_delay_us(1);	 
	while(HAL_GPIO_ReadPin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			BH1750_IIC_Stop();
			return 1;
		}
	}
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void BH1750_IIC_Ack(void)
{
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET);
	BH1750_SDA_OUT();
	HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_RESET);
	bh1750_delay_us(2);
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_SET);
	bh1750_delay_us(2);
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET);
}
//������ACKӦ��		    
void BH1750_IIC_NAck(void)
{
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET);
	BH1750_SDA_OUT();
	HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_SET);
	bh1750_delay_us(2);
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_SET);
	bh1750_delay_us(2);
	HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void BH1750_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	BH1750_SDA_OUT(); 	    
    HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_RESET);
        txd<<=1; 	  
		bh1750_delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_SET);
		bh1750_delay_us(2); 
		HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET);
		bh1750_delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t BH1750_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	BH1750_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET);
        bh1750_delay_us(2);
		HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_SET);
        receive<<=1;
        if(HAL_GPIO_ReadPin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin))receive++;   
		bh1750_delay_us(1); 
    }					 
    if (!ack)
        BH1750_IIC_NAck();//����nACK
    else
        BH1750_IIC_Ack(); //����ACK   
    return receive;
}

void singleWriteBH1750(uint8_t REG_Address)
{
	BH1750_IIC_Start();
	BH1750_IIC_Send_Byte(BH1750_ADDR);
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Send_Byte(REG_Address);
	BH1750_IIC_Wait_Ack();
	BH1750_IIC_Stop();
}
uint8_t luxBuff[2];
int lux_data;
void multipleReadBH1750(void)
{
	float temp;
	uint8_t i;
	BH1750_IIC_Start();
	BH1750_IIC_Send_Byte(BH1750_ADDR|BH1750_READ);
	BH1750_IIC_Wait_Ack();
	luxBuff[0] = BH1750_IIC_Read_Byte(1);
	luxBuff[1] = BH1750_IIC_Read_Byte(0);
	BH1750_IIC_Stop();
	bh1750_delay_us(1000);
	lux_data = luxBuff[0];
	lux_data = (lux_data << 8) + luxBuff[1];
	temp = (float)lux_data/1.2;
	printf ("luxBuff = %1f\r\n",temp);
}

