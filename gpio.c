/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PB13   ------> SPI2_SCK
     PB14   ------> SPI2_MISO
     PB15   ------> SPI2_MOSI
*/

void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, SW2_6_Pin|SW2_5_Pin|SW1_4_Pin|SW1_3_Pin 
                          |SW1_2_Pin|SW1_1_Pin|SW5_10_Pin|SW2_7_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SW2_1_Pin|SW1_6_Pin|SW4_5_Pin|SW4_4_Pin 
                          |SW4_3_Pin|SW4_2_Pin|SW5_4_Pin|SW5_5_Pin 
                          |SW5_6_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SW1_8_Pin|SW1_7_Pin|SW4_1_Pin|SW5_2_Pin 
                          |SW5_3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, KA1_Pin|KA2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SW1_5_Pin|SW5_9_Pin|FIRE_Pin|SW2_8_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MCU_GPIO_Port, MCU_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, SW5_1_Pin|SW5_8_Pin|SW4_10_Pin|SW4_9_Pin 
                          |SW4_8_Pin|SW4_7_Pin|SW4_6_Pin|SW5_7_Pin 
                          |SW2_2_Pin|SW2_3_Pin|SW2_4_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin 
                           PEPin PEPin PEPin PEPin 
                           PEPin PEPin PEPin PEPin 
                           PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = K16_Pin|SW2_6_Pin|K15_Pin|SW2_5_Pin 
                          |K14_Pin|SW1_4_Pin|K4_Pin|SW1_3_Pin 
                          |K3_Pin|SW1_2_Pin|K2_Pin|SW1_1_Pin 
                          |K1_Pin|SW5_10_Pin|K17_Pin|SW2_7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin 
                           PCPin PCPin PCPin PCPin 
                           PCPin PCPin PCPin PCPin 
                           PCPin */
  GPIO_InitStruct.Pin = K13_Pin|K12_Pin|K11_Pin|SW2_1_Pin 
                          |K7_Pin|SW1_6_Pin|SW4_5_Pin|SW4_4_Pin 
                          |SW4_3_Pin|SW4_2_Pin|SW5_4_Pin|SW5_5_Pin 
                          |SW5_6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin 
                           PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = K10_Pin|K9_Pin|SW1_8_Pin|K8_Pin 
                          |SW1_7_Pin|SW4_1_Pin|SW5_2_Pin|SW5_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin 
                           PBPin PBPin PBPin PBPin 
                           PBPin */
  GPIO_InitStruct.Pin = K6_Pin|SW1_5_Pin|K5_Pin|SW5_9_Pin 
                          |KA1_Pin|KA2_Pin|FIRE_Pin|K18_Pin 
                          |SW2_8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = SPI_SCK_Pin|SPI_MISO_Pin|SPI_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin PDPin PDPin 
                           PDPin PDPin PDPin PDPin 
                           PDPin PDPin PDPin PDPin */
  GPIO_InitStruct.Pin = MCU_Pin|SW5_1_Pin|SW5_8_Pin|SW4_10_Pin 
                          |SW4_9_Pin|SW4_8_Pin|SW4_7_Pin|SW4_6_Pin 
                          |SW5_7_Pin|SW2_2_Pin|SW2_3_Pin|SW2_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
//void MX_GPIO_Init(void)
//{

//  GPIO_InitTypeDef GPIO_InitStruct;

//  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOE_CLK_ENABLE();
//  __HAL_RCC_GPIOC_CLK_ENABLE();
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//  __HAL_RCC_GPIOD_CLK_ENABLE();

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOE, K16_Pin|K15_Pin|K14_Pin|K4_Pin 
//                          |K3_Pin|K2_Pin|K1_Pin|K17_Pin, GPIO_PIN_RESET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOE, SW2_6_Pin|SW2_5_Pin|SW1_4_Pin|SW1_3_Pin 
//                          |SW1_2_Pin|SW1_1_Pin|SW5_10_Pin|SW2_7_Pin, GPIO_PIN_SET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOC, K13_Pin|K12_Pin|K11_Pin|K7_Pin, GPIO_PIN_RESET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOC, SW2_1_Pin|SW1_6_Pin|SW4_5_Pin|SW4_4_Pin 
//                          |SW4_3_Pin|SW4_2_Pin|SW5_4_Pin|SW5_5_Pin 
//                          |SW5_6_Pin, GPIO_PIN_SET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOA, K10_Pin|K9_Pin|K8_Pin, GPIO_PIN_RESET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOA, SW1_8_Pin|SW1_7_Pin|SW4_1_Pin|SW5_2_Pin 
//                          |SW5_3_Pin, GPIO_PIN_SET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOB, K6_Pin|K5_Pin|KA1_Pin|KA2_Pin 
//                          |K18_Pin, GPIO_PIN_RESET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOB, SW1_5_Pin|SW5_9_Pin|FIRE_Pin|SW2_8_Pin, GPIO_PIN_SET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(MCU_GPIO_Port, MCU_Pin, GPIO_PIN_RESET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOD, SW5_1_Pin|SW5_8_Pin|SW4_10_Pin|SW4_9_Pin 
//                          |SW4_8_Pin|SW4_7_Pin|SW4_6_Pin|SW5_7_Pin 
//                          |SW2_2_Pin|SW2_3_Pin|SW2_4_Pin, GPIO_PIN_SET);

//  /*Configure GPIO pins : PEPin PEPin PEPin PEPin 
//                           PEPin PEPin PEPin PEPin 
//                           PEPin PEPin PEPin PEPin 
//                           PEPin PEPin PEPin PEPin */
//  GPIO_InitStruct.Pin = K16_Pin|SW2_6_Pin|K15_Pin|SW2_5_Pin 
//                          |K14_Pin|SW1_4_Pin|K4_Pin|SW1_3_Pin 
//                          |K3_Pin|SW1_2_Pin|K2_Pin|SW1_1_Pin 
//                          |K1_Pin|SW5_10_Pin|K17_Pin|SW2_7_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

//  /*Configure GPIO pins : PCPin PCPin PCPin PCPin 
//                           PCPin PCPin PCPin PCPin 
//                           PCPin PCPin PCPin PCPin 
//                           PCPin */
//  GPIO_InitStruct.Pin = K13_Pin|K12_Pin|K11_Pin|SW2_1_Pin 
//                          |K7_Pin|SW1_6_Pin|SW4_5_Pin|SW4_4_Pin 
//                          |SW4_3_Pin|SW4_2_Pin|SW5_4_Pin|SW5_5_Pin 
//                          |SW5_6_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//  /*Configure GPIO pins : PAPin PAPin PAPin PAPin 
//                           PAPin PAPin PAPin PAPin */
//  GPIO_InitStruct.Pin = K10_Pin|K9_Pin|SW1_8_Pin|K8_Pin 
//                          |SW1_7_Pin|SW4_1_Pin|SW5_2_Pin|SW5_3_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//  /*Configure GPIO pins : PBPin PBPin PBPin PBPin 
//                           PBPin PBPin PBPin PBPin 
//                           PBPin */
//  GPIO_InitStruct.Pin = K6_Pin|SW1_5_Pin|K5_Pin|SW5_9_Pin 
//                          |KA1_Pin|KA2_Pin|FIRE_Pin|K18_Pin 
//                          |SW2_8_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//  /*Configure GPIO pins : PBPin PBPin PBPin */
//  GPIO_InitStruct.Pin = SPI_SCK_Pin|SPI_MISO_Pin|SPI_MOSI_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//  /*Configure GPIO pins : PDPin PDPin PDPin PDPin 
//                           PDPin PDPin PDPin PDPin 
//                           PDPin PDPin PDPin PDPin */
//  GPIO_InitStruct.Pin = MCU_Pin|SW5_1_Pin|SW5_8_Pin|SW4_10_Pin 
//                          |SW4_9_Pin|SW4_8_Pin|SW4_7_Pin|SW4_6_Pin 
//                          |SW5_7_Pin|SW2_2_Pin|SW2_3_Pin|SW2_4_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

//}

void MX_SWx_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = SW5_1_Pin|SW5_8_Pin|SW4_10_Pin 
							|SW4_9_Pin|SW4_8_Pin|SW4_7_Pin|SW4_6_Pin 
							|SW5_7_Pin|SW2_2_Pin|SW2_3_Pin|SW2_4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULL_MODE;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = SW1_5_Pin|SW5_9_Pin 
							|FIRE_Pin|GPIO_PIN_13
							|SW2_8_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULL_MODE;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = SW1_8_Pin|GPIO_PIN_10|GPIO_PIN_9
							|SW1_7_Pin|SW4_1_Pin|SW5_2_Pin|SW5_3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULL_MODE;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = SW2_1_Pin 
							|SW1_6_Pin|SW4_5_Pin|SW4_4_Pin 
							|SW4_3_Pin|SW4_2_Pin|SW5_4_Pin|SW5_5_Pin 
							|SW5_6_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULL_MODE;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin = SW2_6_Pin|SW2_5_Pin 
							|SW1_4_Pin|SW1_3_Pin 
							|SW1_2_Pin|SW1_1_Pin 
							|SW5_10_Pin|SW2_7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULL_MODE;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
}

void MX_Kx_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = K16_Pin|K15_Pin|
						  K14_Pin|K4_Pin|
						  K3_Pin|K2_Pin|
						  K1_Pin|K17_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = K_GPIO_PULL_MODE;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = K13_Pin|K12_Pin|K11_Pin
							|K7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = K_GPIO_PULL_MODE;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = K10_Pin|K9_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = K_GPIO_PULL_MODE;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = K6_Pin|K5_Pin|K18_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = K_GPIO_PULL_MODE;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}
/* USER CODE END 2 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
