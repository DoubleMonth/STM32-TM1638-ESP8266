#include "stdio.h" 
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "si7020/si7020_iic.h"
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
void delay_us(uint32_t us)
{
	uint32_t i,j;
	for(i=0;i<us;i++)
	{
		for(j=0;j<100;j++)
		{};
	}
}
//IIC初始化
void SI7020_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();   //使能GPIOC时钟
    
    //PC11,12初始化设置
    GPIO_Initure.Pin|=SI7020_SCL_Pin|SI7020_SDA_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
}
//void SDA_OUT(void)
//{
//	GPIO_InitTypeDef GPIO_Initure;
//	GPIO_Initure.Pin=SI7020_SDA_Pin;
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
//    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
//    HAL_GPIO_Init(SI7020_SDA_GPIO_Port,&GPIO_Initure);
//}
//void SDA_IN(void)
//{
//	GPIO_InitTypeDef GPIO_Initure;
//	GPIO_Initure.Pin=SI7020_SDA_Pin;
//    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //推挽输出
//    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
//    HAL_GPIO_Init(SI7020_SDA_GPIO_Port,&GPIO_Initure);
//}

//产生IIC起始信号
void SI7020_IIC_Start(void)
{
	SI7020_SDA_OUT(); 	         //sda线输出
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
	delay_us(4);
 	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void SI7020_IIC_Stop(void)
{
	SI7020_SDA_OUT();//sda线输出
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t SI7020_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SI7020_SDA_IN();      //SDA设置为输入  
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
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void SI7020_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SI7020_SDA_OUT(); 	    
    HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_RESET);
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
		delay_us(2); 
		HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t SI7020_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SI7020_SDA_IN();//SDA设置为输入
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
        SI7020_IIC_NAck();//发送nACK
    else
        SI7020_IIC_Ack(); //发送ACK   
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

