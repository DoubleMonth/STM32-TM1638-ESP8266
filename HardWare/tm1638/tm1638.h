#ifndef _TM1638_H
#define _TM1638_H
#include "stm32f1xx_hal.h"

//uint8_t tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
//                           0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

#define TM1638_DIO_Pin GPIO_PIN_4
#define TM1638_DIO_GPIO_Port GPIOB
#define TM1638_CLK_Pin GPIO_PIN_5
#define TM1638_CLK_GPIO_Port GPIOB
#define TM1638_STB_Pin GPIO_PIN_6
#define TM1638_STB_GPIO_Port GPIOB

#define	DATA_COMMAND	0X40
#define	DISP_COMMAND	0x80
#define	ADDR_COMMAND	0XC0

void TM1638_Init(void);
void TM1638_Write(uint8_t data);
void writeCMD(uint8_t cmd);
void writeData(uint8_t add,uint8_t data);
void writeAllLED(uint8_t LED_Flag);
void display(uint8_t *number,uint8_t pointFlash);//number为要显示的数字，依次从1到10。

#endif
