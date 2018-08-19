
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
  * COPYRIGHT(c) 2018 STMicroelectronics
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "bsp_rs485x.h"
#include "bsp_gpioControl.h"
#include "stdio.h"
#include "stm32Flash.h"
#include "string.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define DEBUG		1

#define ON_CNT		20
#define OFF_CNT		120
uint16_t gRunningLedFlag;
uint8_t gKA1Flag;
uint8_t gFlag;
uint8_t gFireTimeFlag = 1;
uint8_t gRS485BDelayCnt = 0;
uint8_t gNeedWriteFlashFlag = 0;
uint8_t gManuallyFireFlag = 0;
uint8_t gCodeFireFlag = 0;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
int fputc(int ch,FILE *f)
{
	uint8_t temp[1]= {ch};
	HAL_UART_Transmit(&huart1,temp,1,2);
	return ch;
}
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	FLASH_OBProgramInitTypeDef obData;
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
	//MX_SWx_GPIO_Init(); //将拨码设置位输入模式，而且关闭上拉
	//MX_Kx_GPIO_Init(); //将K1-K15设置为输入模式，无上拉和下拉
	/* 读取掉电前获取的刷卡模式 */
	gNeedFlashCardFlag = HAL_FLASHEx_OBGetUserData(OB_DATA_ADDRESS_DATA0);
	if(gNeedFlashCardFlag > 1)
	{
		gNeedFlashCardFlag = 1;
    __disable_irq();
		HAL_FLASH_Unlock();
		HAL_FLASH_OB_Unlock();
		HAL_FLASHEx_OBGetConfig(&obData);
		HAL_FLASHEx_OBErase();
		obData.DATAAddress = OB_DATA_ADDRESS_DATA0;
		obData.DATAData = gNeedFlashCardFlag;
		obData.OptionType = OPTIONBYTE_DATA;
		HAL_FLASHEx_OBProgram(&obData);
		HAL_FLASH_OB_Lock();
		HAL_FLASH_Lock();
	}
	gNeedFlashCardFlag = HAL_FLASHEx_OBGetUserData(OB_DATA_ADDRESS_DATA0);
  __enable_irq();
  if(1 == gNeedFlashCardFlag)
  {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
  }
#if 1 == DEBUG
	//printf("gNeedFlashCardFlag is %X \r\n",gNeedFlashCardFlag);
#endif
	bsp_RS485xVar_Init();
	bsp_RS485A_Init();
	bsp_RS485B_Init();
	bsp_RS485C_Init();
	bsp_gpioControl_Init();
	/* 开启定时器 */
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
//  HAL_Delay(10);
//  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
//  HAL_Delay(2000);
//  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
  __disable_irq();
  gNeedFlashCardFlag = HAL_FLASHEx_OBGetUserData(OB_DATA_ADDRESS_DATA0);
  //printf(" the gNeedFlashCardFlag is %d\r\n ",gNeedFlashCardFlag);
  __enable_irq();
  
	
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    /* 解析串口RS-485A的数据 */
    bsp_RS485A_PrashData();
    /* 解析串口RS-485B的数据 */
    bsp_RS485B_PrashData();
    /* 写入火警报警消息 */
    if(1 == gNeedWriteFlashFlag)
    {
      gNeedWriteFlashFlag = 0;
      //printf(" write flag to Flash\r\n");
       __disable_irq();
      HAL_FLASH_Unlock();
      HAL_FLASH_OB_Unlock();
      HAL_FLASHEx_OBGetConfig(&obData);
      HAL_FLASHEx_OBErase();
      obData.DATAAddress = OB_DATA_ADDRESS_DATA0;
      obData.DATAData = gNeedFlashCardFlag;
      obData.OptionType = OPTIONBYTE_DATA;
      HAL_FLASHEx_OBProgram(&obData);
      HAL_FLASH_OB_Lock();
      HAL_FLASH_Lock();
      __enable_irq();
    }
    
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USART3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* TIM5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM5_IRQn);
  /* TIM3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* TIM4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
	uint8_t i = 0;
	/* 10ms进入一次 */
	if(&htim3 == htim)
	{
    if(2 == gRevRS485BDelayFlag)
    {
      gRS485BDelayCnt++;
      if(gRS485BDelayCnt > 9)
      {
        gRS485BDelayCnt = 0;
        gRevRS485BDelayFlag = 0;
        for(i = 10; i < 18; i++)
        {
          gAckRS485CAskDataBuff[i] |= gRS485BData[i - 10];
        }
        for( i = 26; i < 34; i++)
        {
          gAckRS485CAskDataBuff[i] |= gRS485BData[i - 26];
        }
        gAutoRegisterFlag = 1;
        if(0 == gManuallyRegisterFlag || gClearTimeCnt < 70)
        {
          gClearTimeCnt = 80;
        }
        
        memset(gRS485BData,0x00,8);
      }
    }
		gRunningLedFlag++;
		if(gRunningLedFlag <= ON_CNT)
		{
			//点亮运行LED灯
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_RESET);
		}
		else if(gRunningLedFlag > OFF_CNT)
		{
			gRunningLedFlag = 0;
		}
		else
		{
			//熄灭运行LED灯
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
		}
		//printf("\r\n htim3 \r\n");
		if(gKA1Flag)
		{
			gKA1TimCnt--;
			if(gKA1TimCnt < 10)
			{
				gKA1TimCnt = 0;
				gKA1Flag = 0;
				/* 控制GPIO 开路继电器KA1 */
				HAL_GPIO_WritePin(KA1_GPIO_Port,KA1_Pin,GPIO_PIN_RESET);
			}
		}
		if(gRevRS485ADataFlag)
		{
			gClearTimeCnt--;
			/*
			gKA1TimCnt--;
			if(gKA1TimCnt < 10)
			{
				gKA1TimCnt = 5;
				// 控制GPIO 开路继电器KA1 
				HAL_GPIO_WritePin(KA1_GPIO_Port,KA1_Pin,GPIO_PIN_RESET);
			}
		  */
			if(gClearTimeCnt < 3)
			{
				gRevRS485ADataFlag = 0;
				gClearTimeCnt = 0;
				gFlag = 0;
				gKA2ActionFlag = 0;
				HAL_GPIO_WritePin(KA2_GPIO_Port,KA2_Pin,GPIO_PIN_RESET);
        if(1 == gManuallyRegisterFlag)
        {
          bsp_ClearAllRegisterFloorInfo();//此函数中已经包含了对拨码的查询
          gManuallyRegisterFlag = 0;
        }
        if(1 == gAutoRegisterFlag)
        {
          bsp_CLearAutoRegisterFloorInfo();
          gAutoRegisterFlag = 0;
        }
				//bsp_gpioControlGetSW45Data();
#if 2==DEBUG
				printf("\r\n gRevRS485ADataFlag = 0\r\n");
#endif
			}
		}
	}
   /* 40ms */
	if(&htim4 == htim)
	{
		bsp_gpioControlGetSW45Data();
    
    gFireFlag = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5);
    if(GPIO_PIN_RESET == gFireFlag)
    {
      HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
      if(2 == gRS485CSendGateTypeFlag)
      {
        for(i = 2; i < 34; i++)
        {
          gAckRS485CAskDataBuff[i] = 0xFF;
        }
      }
      if(1 == gRS485CSendGateTypeFlag)                          
      {
        for(i = 2; i < 18; i ++)
        {
          gAckRS485CAskDataBuff[i] = 0xFF;
        }
      }
      if(0 == gRS485CSendGateTypeFlag)
      {
        for(i = 18; i < 34; i++)
        {
          gAckRS485CAskDataBuff[i] = 0xFF;
        }
      }
      bsp_CLearAutoRegisterFloorInfo();
      if(0 == gManuallyFireFlag)
      {
        gManuallyFireFlag = 1;
      }
    }
    else
    {
      if(0 == gNeedFlashCardFlag)
      {
        if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4))
        {
          if(1 == gCodeFireFlag)
          {
            gCodeFireFlag = 0;
            gNeedWriteFlashFlag = 1;
            bsp_ClearAllRegisterFloorInfo(); 
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
          }
        }
        else
        {
          if(0 == gCodeFireFlag)
          {
            gCodeFireFlag = 1;
            gNeedWriteFlashFlag = 1;
          }
          HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
          if(2 == gRS485CSendGateTypeFlag)
          {
            for(i = 2; i < 34; i++)
            {
              gAckRS485CAskDataBuff[i] = 0xFF;
            }
          }
          if(1 == gRS485CSendGateTypeFlag)
          {
            for(i = 2; i < 18; i ++)
            {
              gAckRS485CAskDataBuff[i] = 0xFF;
            }
          }
          if(0 == gRS485CSendGateTypeFlag)
          {
            for(i = 18; i < 34; i++)
            {
              gAckRS485CAskDataBuff[i] = 0xFF;
            }
          }
          bsp_CLearAutoRegisterFloorInfo();
        }
      }
      else
      {
         if(1 == gCodeFireFlag)
         {
           gCodeFireFlag = 0;
           gNeedWriteFlashFlag = 1;
           HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
           bsp_ClearAllRegisterFloorInfo();
         }
      }
      if(1 == gManuallyFireFlag)
      {
        gManuallyFireFlag = 0;
        bsp_ClearAllRegisterFloorInfo();
      }
    }
	}
}
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
