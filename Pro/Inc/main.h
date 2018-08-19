/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define K16_Pin GPIO_PIN_2
#define K16_GPIO_Port GPIOE
#define SW2_6_Pin GPIO_PIN_3
#define SW2_6_GPIO_Port GPIOE
#define K15_Pin GPIO_PIN_4
#define K15_GPIO_Port GPIOE
#define SW2_5_Pin GPIO_PIN_5
#define SW2_5_GPIO_Port GPIOE
#define K14_Pin GPIO_PIN_6
#define K14_GPIO_Port GPIOE
#define K13_Pin GPIO_PIN_0
#define K13_GPIO_Port GPIOC
#define K12_Pin GPIO_PIN_1
#define K12_GPIO_Port GPIOC
#define K11_Pin GPIO_PIN_2
#define K11_GPIO_Port GPIOC
#define SW2_1_Pin GPIO_PIN_3
#define SW2_1_GPIO_Port GPIOC
#define K10_Pin GPIO_PIN_1
#define K10_GPIO_Port GPIOA
#define K9_Pin GPIO_PIN_4
#define K9_GPIO_Port GPIOA
#define SW1_8_Pin GPIO_PIN_5
#define SW1_8_GPIO_Port GPIOA
#define K8_Pin GPIO_PIN_6
#define K8_GPIO_Port GPIOA
#define SW1_7_Pin GPIO_PIN_7
#define SW1_7_GPIO_Port GPIOA
#define K7_Pin GPIO_PIN_4
#define K7_GPIO_Port GPIOC
#define SW1_6_Pin GPIO_PIN_5
#define SW1_6_GPIO_Port GPIOC
#define K6_Pin GPIO_PIN_0
#define K6_GPIO_Port GPIOB
#define SW1_5_Pin GPIO_PIN_1
#define SW1_5_GPIO_Port GPIOB
#define K5_Pin GPIO_PIN_3		//K5 ÐÞ¸ÄÎªPA3
#define K5_GPIO_Port GPIOA
#define SW1_4_Pin GPIO_PIN_7
#define SW1_4_GPIO_Port GPIOE
#define K4_Pin GPIO_PIN_8
#define K4_GPIO_Port GPIOE
#define SW1_3_Pin GPIO_PIN_9
#define SW1_3_GPIO_Port GPIOE
#define K3_Pin GPIO_PIN_10
#define K3_GPIO_Port GPIOE
#define SW1_2_Pin GPIO_PIN_11
#define SW1_2_GPIO_Port GPIOE
#define K2_Pin GPIO_PIN_12
#define K2_GPIO_Port GPIOE
#define SW1_1_Pin GPIO_PIN_13
#define SW1_1_GPIO_Port GPIOE
#define K1_Pin GPIO_PIN_14
#define K1_GPIO_Port GPIOE
#define SW5_10_Pin GPIO_PIN_15
#define SW5_10_GPIO_Port GPIOE
#define RS485A_TXD_Pin GPIO_PIN_10
#define RS485A_TXD_GPIO_Port GPIOB
#define RS485A_RXD_Pin GPIO_PIN_11
#define RS485A_RXD_GPIO_Port GPIOB
#define SW5_9_Pin GPIO_PIN_12
#define SW5_9_GPIO_Port GPIOB
#define SPI_SCK_Pin GPIO_PIN_13
#define SPI_SCK_GPIO_Port GPIOB
#define SPI_MISO_Pin GPIO_PIN_14
#define SPI_MISO_GPIO_Port GPIOB
#define SPI_MOSI_Pin GPIO_PIN_15
#define SPI_MOSI_GPIO_Port GPIOB
#define MCU_Pin GPIO_PIN_8
#define MCU_GPIO_Port GPIOD
#define SW5_1_Pin GPIO_PIN_9
#define SW5_1_GPIO_Port GPIOD
#define SW5_8_Pin GPIO_PIN_10
#define SW5_8_GPIO_Port GPIOD
#define SW4_10_Pin GPIO_PIN_11
#define SW4_10_GPIO_Port GPIOD
#define SW4_9_Pin GPIO_PIN_12
#define SW4_9_GPIO_Port GPIOD
#define SW4_8_Pin GPIO_PIN_13
#define SW4_8_GPIO_Port GPIOD
#define SW4_7_Pin GPIO_PIN_14
#define SW4_7_GPIO_Port GPIOD
#define SW4_6_Pin GPIO_PIN_15
#define SW4_6_GPIO_Port GPIOD
#define SW4_5_Pin GPIO_PIN_6
#define SW4_5_GPIO_Port GPIOC
#define SW4_4_Pin GPIO_PIN_7
#define SW4_4_GPIO_Port GPIOC
#define SW4_3_Pin GPIO_PIN_8
#define SW4_3_GPIO_Port GPIOC
#define SW4_2_Pin GPIO_PIN_9
#define SW4_2_GPIO_Port GPIOC
#define SW4_1_Pin GPIO_PIN_8
#define SW4_1_GPIO_Port GPIOA
#define SW5_2_Pin GPIO_PIN_11
#define SW5_2_GPIO_Port GPIOA
#define SW5_3_Pin GPIO_PIN_12
#define SW5_3_GPIO_Port GPIOA
#define SW5_4_Pin GPIO_PIN_10
#define SW5_4_GPIO_Port GPIOC
#define SW5_5_Pin GPIO_PIN_11
#define SW5_5_GPIO_Port GPIOC
#define SW5_6_Pin GPIO_PIN_12
#define SW5_6_GPIO_Port GPIOC
#define SW5_7_Pin GPIO_PIN_0
#define SW5_7_GPIO_Port GPIOD
#define SW2_2_Pin GPIO_PIN_3
#define SW2_2_GPIO_Port GPIOD
#define SW2_3_Pin GPIO_PIN_4
#define SW2_3_GPIO_Port GPIOD
#define RS485B_TXD_Pin GPIO_PIN_5
#define RS485B_TXD_GPIO_Port GPIOD
#define RS485B_RXD_Pin GPIO_PIN_6
#define RS485B_RXD_GPIO_Port GPIOD
#define SW2_4_Pin GPIO_PIN_7
#define SW2_4_GPIO_Port GPIOD
#define KA1_Pin GPIO_PIN_3
#define KA1_GPIO_Port GPIOB
#define KA2_Pin GPIO_PIN_4
#define KA2_GPIO_Port GPIOB
#define FIRE_Pin GPIO_PIN_5
#define FIRE_GPIO_Port GPIOB
#define RS485C_TXD_Pin GPIO_PIN_6
#define RS485C_TXD_GPIO_Port GPIOB
#define RS485C_RXD_Pin GPIO_PIN_7
#define RS485C_RXD_GPIO_Port GPIOB
#define K18_Pin GPIO_PIN_8
#define K18_GPIO_Port GPIOB
#define SW2_8_Pin GPIO_PIN_9
#define SW2_8_GPIO_Port GPIOB
#define K17_Pin GPIO_PIN_0
#define K17_GPIO_Port GPIOE
#define SW2_7_Pin GPIO_PIN_1
#define SW2_7_GPIO_Port GPIOE

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
