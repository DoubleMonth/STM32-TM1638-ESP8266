#include "stdio.h" 
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "pcf8563/pcf8563.h"
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

unsigned char  time_buf1[8]={20,17,12,31,23,59,50,6};
//uint8_t time_buf1[8]={1,2,3,4,5,6,7,8};
unsigned char time_buf[8];
unsigned char time_buf2[8];
unsigned char *time_buf1_sp;


void pcf8563_delay_us(uint32_t us)
{
	uint32_t i,j;
	for(i=0;i<us;i++)
	{
		for(j=0;j<100;j++)
		{};
	}
}
//IIC初始化
void PCF8563_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();   //使能GPIOC时钟
    
    //PC11,12初始化设置
    GPIO_Initure.Pin|=PCF8563_SCL_Pin|PCF8563_SDA_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);
}

//产生IIC起始信号
void PCF8563_IIC_Start(void)
{
	SDA_OUT(); 	         //sda线输出
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);
	pcf8563_delay_us(4);
 	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	pcf8563_delay_us(4);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void PCF8563_IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	pcf8563_delay_us(4);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);//发送I2C总线结束信号
	pcf8563_delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t PCF8563_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);
	pcf8563_delay_us(1);	   
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	pcf8563_delay_us(1);	 
	while(HAL_GPIO_ReadPin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			PCF8563_IIC_Stop();
			return 1;
		}
	}
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void PCF8563_IIC_Ack(void)
{
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
	SDA_OUT();
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_RESET);
	pcf8563_delay_us(2);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	pcf8563_delay_us(2);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
}
//不产生ACK应答		    
void PCF8563_IIC_NAck(void)
{
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
	SDA_OUT();
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);
	pcf8563_delay_us(2);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	pcf8563_delay_us(2);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void PCF8563_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_RESET);
        txd<<=1; 	  
		pcf8563_delay_us(2);   //对TEA5767这三个延时都是必须的
		HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
		pcf8563_delay_us(2); 
		HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
		pcf8563_delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t PCF8563_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
        pcf8563_delay_us(2);
		HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
        receive<<=1;
        if(HAL_GPIO_ReadPin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin))receive++;   
		pcf8563_delay_us(1); 
    }					 
    if (!ack)
        PCF8563_IIC_NAck();//发送nACK
    else
        PCF8563_IIC_Ack(); //发送ACK   
    return receive;
}


//uint8_t si70xx_buff[1];
//uint8_t si70xx_data[2];
uint8_t PCF8563_ReadOneByte(uint16_t ReadAddr)
{
	uint8_t temp = 0;
	PCF8563_IIC_Start();
	PCF8563_IIC_Send_Byte(PCF8563_ADDR);
	PCF8563_IIC_Wait_Ack();
	PCF8563_IIC_Send_Byte(ReadAddr);
	PCF8563_IIC_Wait_Ack();
	PCF8563_IIC_Start();
	PCF8563_IIC_Send_Byte(PCF8563_ADDR|PCF8563_READ);
	PCF8563_IIC_Wait_Ack();
	temp = PCF8563_IIC_Read_Byte(0);
	
	PCF8563_IIC_Stop();
	return temp;
}
uint8_t PCF8563_WriteOneByte(uint8_t WriteAddr,uint8_t DataToWrite)
{
	PCF8563_IIC_Start();
	PCF8563_IIC_Send_Byte(PCF8563_ADDR);
	PCF8563_IIC_Wait_Ack();
	PCF8563_IIC_Send_Byte(WriteAddr);
	PCF8563_IIC_Wait_Ack();
	PCF8563_IIC_Send_Byte(DataToWrite);
	PCF8563_IIC_Wait_Ack();
	PCF8563_IIC_Stop();
	pcf8563_delay_us(2000);
}
void PCF8563_WriteTime(void)
{
	uint8_t i,temp;
	for(i=0;i<8;i++)
	{
		temp = time_buf1[i]/10;
		time_buf[i] = time_buf1[i]%10;
		time_buf[i] = time_buf[i]+temp*16;
	}
	PCF8563_WriteOneByte(CONTROL_STATUS_1,0X20);
	PCF8563_WriteOneByte(PCF8563_YEAR_ADDRESS,time_buf[1]);
	PCF8563_WriteOneByte(PCF8563_MONTH_ADDRESS,time_buf[2]);
	PCF8563_WriteOneByte(PCF8563_DAY_ADDRESS,time_buf[3]);
	PCF8563_WriteOneByte(PCF8563_HOUR_ADDRESS,time_buf[4]);
	PCF8563_WriteOneByte(PCF8563_MINUTE_ADDRESS,time_buf[5]);
	PCF8563_WriteOneByte(PCF8563_SECOND_ADDRESS,time_buf[6]);
	PCF8563_WriteOneByte(PCF8563_WEEK_ADDRESS,time_buf[7]);
	PCF8563_WriteOneByte(CONTROL_STATUS_1,0X00);
}
extern uint8_t time_buffer[8];
void PCF8563_ReadTime(uint8_t *p_time_buf)
{
	uint8_t i,temp;
	time_buf[1] = PCF8563_ReadOneByte(PCF8563_YEAR_ADDRESS)&0XFF;
	time_buf[2] = PCF8563_ReadOneByte(PCF8563_MONTH_ADDRESS)&0X1F;
	time_buf[3] = PCF8563_ReadOneByte(PCF8563_DAY_ADDRESS)&0X3F;
	time_buf[4] = PCF8563_ReadOneByte(PCF8563_HOUR_ADDRESS)&0X3F;
	time_buf[5] = PCF8563_ReadOneByte(PCF8563_MINUTE_ADDRESS)&0X7F;
	time_buf[6] = PCF8563_ReadOneByte(PCF8563_SECOND_ADDRESS)&0X7F;
	time_buf[7] = PCF8563_ReadOneByte(PCF8563_WEEK_ADDRESS)&0X0F;
	for(i=0;i<8;i++)
	{
		temp=time_buf[i]/16;
		time_buffer[i]=time_buf[i]%16;
		time_buffer[i]=time_buffer[i]+temp*10;
	}

}
