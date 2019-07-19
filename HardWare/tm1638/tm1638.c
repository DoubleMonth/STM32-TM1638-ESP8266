#include "stdio.h" 
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "tm1638/tm1638.h"
uint8_t tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                           0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
void TM1638_Init(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOC时钟
    
    //PC11,12初始化设置
    GPIO_Initure.Pin|=TM1638_DIO_Pin|TM1638_CLK_Pin|TM1638_STB_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
	HAL_GPIO_WritePin(TM1638_DIO_GPIO_Port,TM1638_DIO_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(TM1638_CLK_GPIO_Port,TM1638_CLK_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port,TM1638_STB_Pin,GPIO_PIN_SET);
	
	
	writeCMD(0x8f);		//设置亮度
	writeCMD(0x40);		//采用地址自动加1
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port,TM1638_STB_Pin,GPIO_PIN_RESET);  //STB=0
	TM1638_Write(0xC0);		//设置起始地址
	for(i=0;i<16;i++)	//初始全部不显示
	{
		TM1638_Write(0x00);
	}
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port,TM1638_STB_Pin,GPIO_PIN_SET);
}
void TM1638_Write(uint8_t data)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		HAL_GPIO_WritePin(TM1638_CLK_GPIO_Port,TM1638_CLK_Pin,GPIO_PIN_RESET);
		if(data&0x01)
			HAL_GPIO_WritePin(TM1638_DIO_GPIO_Port,TM1638_DIO_Pin,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(TM1638_DIO_GPIO_Port,TM1638_DIO_Pin,GPIO_PIN_RESET);
		data>>=1;
		HAL_GPIO_WritePin(TM1638_CLK_GPIO_Port,TM1638_CLK_Pin,GPIO_PIN_SET);
	}
}
void writeCMD(uint8_t cmd)
{
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port,TM1638_STB_Pin,GPIO_PIN_RESET);
	TM1638_Write(cmd);
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port,TM1638_STB_Pin,GPIO_PIN_SET);
}
void writeData(uint8_t add,uint8_t data)
{
	writeCMD(0x44);//普通模式，固定地址
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port,TM1638_STB_Pin,GPIO_PIN_RESET);
	TM1638_Write(0xc0|add);
	TM1638_Write(data);
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port,TM1638_STB_Pin,GPIO_PIN_SET);
}
void writeAllLED(uint8_t LED_Flag)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(LED_Flag&(1<<i))
			writeData(2*i+1,1);
		else
			writeData(2*i+1,0);
	}
}
void display(uint8_t *number,uint8_t pointFlash)//number为要显示的数字，依次从1到10。
{
	int i;
	uint8_t ii;
	uint8_t j=0;
	uint8_t data[8]={0};
	uint8_t data00=0,data01=0,data02=0,data03=0,data04=0,data05=0,data06=0,data07=0;
//	displayNumber[10]={4,5,6,7,8,9,0,1,2,3};
	
	for(j=0;j<8;j++)
	{
		for(i=7;i>=0;i--)
		{
			data[j] <<= 1;
			if((i==3)&&pointFlash)
			{
				if(((tab[*(number+i)]|0x80)>>j)&0x01)
				data[j] +=1;
			}
			else
			{
				if(((tab[*(number+i)])>>j)&0x01)
				data[j] +=1;
			}
			
		}
		writeData(2*j,data[j]);
	}
	
	for(j=0;j<8;j++)
	{
		for(i=7;i>=0;i--)
		{
			data[j] <<= 1;
			if((tab[*(number+i+8)]>>j)&0x01)
				data[j] +=1;
		}
		writeData(2*j+1,data[j]);
	}

//	for(ii=0;ii<10;ii++)
//	{
//		tab[ii]=duan_tab[*(number+ii)];
//	}
//	for(i=7;i>=0;i--)
//	{
//		data00 <<= 1;
//		if((tab[i])&0x01)
//			data00 +=1;
//	}
//	writeData(0x00,data00);
//	for(i=7;i>=0;i--)
//	{
//		data01 <<= 1;
//		if((tab[i]>>1)&0x01)
//			data01 +=1;
//	}
//	writeData(0x02,data01);
//	for(i=7;i>=0;i--)
//	{
//		data02 <<= 1;
//		if((tab[i]>>2)&0x01)
//			data02 +=1;
//	}
//	writeData(0x04,data02);
//	for(i=7;i>=0;i--)
//	{
//		data03 <<= 1;
//		if((tab[i]>>3)&0x01)
//			data03 +=1;
//	}
//	writeData(0x06,data03);
//	for(i=7;i>=0;i--)
//	{
//		data04 <<= 1;
//		if((tab[i]>>4)&0x01)
//			data04 +=1;
//	}
//	writeData(0x08,data04);
//	for(i=7;i>=0;i--)
//	{
//		data05 <<= 1;
//		if((tab[i]>>5)&0x01)
//			data05 +=1;
//	}
//	writeData(0x0A,data05);
//	for(i=7;i>=0;i--)
//	{
//		data06 <<= 1;
//		if((tab[i]>>6)&0x01)
//			data06 +=1;
//	}
//	writeData(0x0C,data06);
//	for(i=7;i>=0;i--)
//	{
//		data07 <<= 1;
//		if((tab[i]>>7)&0x01)
//			data07 +=1;
//	}
//	writeData(0x0E,data07);

//	writeData(0x01,0x03);
//	writeData(0x03,0x03);
//	writeData(0x05,0x03);
//	writeData(0x07,0x03);
//	writeData(0x09,0x01);
//	writeData(0x0B,0x03);
//	writeData(0x0D,0x03);
//	writeData(0x0F,0x00);
}