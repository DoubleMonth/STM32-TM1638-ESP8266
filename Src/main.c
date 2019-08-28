
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "si7020/si7020_iic.h"
#include "pcf8563/pcf8563.h"
#include "bh1750/bh1750.h"
#include "tm1638/tm1638.h"
#include "hal_dev_ctrl.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
//uint8_t aTxStartMessages[] = "\r\n******UART commucition using IT******\r\nPlease enter 10 characters:\r\n";
//uint8_t aRxBuffer[20];

/* USER CODE BEGIN PV */
/* 重定向printf函数	begin*/
#include "stdio.h"
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
__IO ITStatus USART1Ready = RESET;
 
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit_IT(&huart1 , (uint8_t *)&ch, 1);
	while (USART1Ready != SET)
  {
  }
	USART1Ready = RESET;
	return ch;
}
 /**
  * @brief  Tx Transfer completed callbacks.
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */ 
	USART1Ready = SET;
}
/* 重定向printf函数	end*/
/* USER CODE END PV */



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
void delay(uint32_t  t)
{
	uint32_t i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<1000;j++)
		;
	}
}
double si7020_temperature;
double si7020_humidity;
uint8_t time_buffer1[8];
uint8_t time_buffer[8];
extern uint8_t duan_tab[];
uint8_t displayNumber[10]={0};
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  SI7020_IIC_Init();
  PCF8563_IIC_Init();
  PCF8563_WriteTime();
  BH1750_IIC_Init();
  singleWriteBH1750(0x01);
  TM1638_Init();
  /* USER CODE BEGIN 2 */
	uint16_t d=32;
//	uint8_t i;
//	for(i=0;i<8;i++)	//全部显示0
//	{
//		writeData(i<<1,tab[0]);
//	}
//	writeCMD(0x44);//普通模式，固定地址
//	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port,TM1638_STB_Pin,GPIO_PIN_RESET);
//	TM1638_Write(0x02);
//	TM1638_Write(0x3f);
//	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port,TM1638_STB_Pin,GPIO_PIN_SET);
//	HAL_UART_Transmit_IT(&huart1 ,(uint8_t*)aTxStartMessages,sizeof(aTxStartMessages)); //sizeof()可读取目标长度
//	HAL_UART_Receive_IT(&huart1,(uint8_t*)aRxBuffer,10);

	printf ("Hello STM%d!\r\n",d);
	printf ("Hello STM%dF103!\r\n",d);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	  uint8_t j;
	  delay(5000);
	  si7020Measure( &si7020_temperature, &si7020_humidity);
	  printf("temperature = %d\r\n",((int)(si7020_temperature*10)));
	  PCF8563_ReadTime(time_buffer1);
	  printf("%d%d-%d%d-%d%d  %d%d:%d%d:%d%d \r\n",time_buffer[1]/10,time_buffer[1]%10,time_buffer[2]/10,time_buffer[2]%10,time_buffer[3]/10,time_buffer[3]%10,time_buffer[4]/10,time_buffer[4]%10,time_buffer[5]/10,time_buffer[5]%10,time_buffer[6]/10,time_buffer[6]%10);
	  singleWriteBH1750(0x01);
	  singleWriteBH1750(0x10);
	  delay(500);
	  multipleReadBH1750();
	displayNumber[0]=time_buffer[4]/10;
	displayNumber[1]=time_buffer[4]%10;
	displayNumber[2]=time_buffer[5]/10;
	displayNumber[3]=time_buffer[5]%10;
	displayNumber[4]=(uint16_t)(si7020_temperature*10)/100;
	displayNumber[5]=(uint16_t)(si7020_temperature*10)%100/10;
	displayNumber[6]=(uint16_t)(si7020_temperature*10)%10;
	displayNumber[7]=7;
	displayNumber[8]=8;
	displayNumber[9]=9;
	
	display(displayNumber,(time_buffer[6]%10)&0x01);
	task_handle();
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
