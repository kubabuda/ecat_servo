/**
  ******************************************************************************
  * @file    stm32f4_discovery.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    28-October-2011
  * @brief   This file contains definitions for STM32F4-Discovery Kit's Leds and
  *          push-button hardware resources.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_H
#define __STM32F4_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
 #include "stdbool.h"
 #include "stm32f4xx.h"
 #include "stm32f4xx_conf.h"

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Types
  * @{
  */

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;
/**
  * @}
  */

typedef enum 
{
  COM1 = 0,
  COM2 = 1,
  COM3 = 2,
  COM4 = 3,
  COM5 = 4,
  COM6 = 5,
} COM_TypeDef; 


/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL_LED
  * @{
  */

typedef enum
{
  LED4 = 0,
  LED3 = 1,
  LED5 = 2,
  LED6 = 3
} Led_TypeDef;


#define LEDn                             4

#define LED4_PIN                         GPIO_Pin_13
#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK                    RCC_AHB1Periph_GPIOD

#define LED3_PIN                         GPIO_Pin_12
#define LED3_GPIO_PORT                   GPIOD
#define LED3_GPIO_CLK                    RCC_AHB1Periph_GPIOD

#define LED5_PIN                         GPIO_Pin_14
#define LED5_GPIO_PORT                   GPIOD
#define LED5_GPIO_CLK                    RCC_AHB1Periph_GPIOD

#define LED6_PIN                         GPIO_Pin_15
#define LED6_GPIO_PORT                   GPIOD
#define LED6_GPIO_CLK                    RCC_AHB1Periph_GPIOD

#define OUTPUT_BITS_COUNT    LEDn


extern const Led_TypeDef outPins[OUTPUT_BITS_COUNT];

/**
  * @}
  */

/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL_BUTTON
  * @{
  */
#define BUTTONn                          1

/**
 * @brief Wakeup push-button
 */

#define USER_BUTTON_PIN                GPIO_Pin_0
#define USER_BUTTON_GPIO_PORT          GPIOA
#define USER_BUTTON_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define USER_BUTTON_EXTI_LINE          EXTI_Line0
#define USER_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOA
#define USER_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource0
#define USER_BUTTON_EXTI_IRQn          EXTI0_IRQn
#define USER_BUTTON_PRESSED            1

/**
  * @}
  */

/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL_USART
  * @{
  */
#define COMn                             2
/**
 * @brief Definition for COM port3, connected to USART1
 */ 
#define EVAL_COM3                        USART1
#define EVAL_COM3_CLK                    RCC_APB2Periph_USART1
#define EVAL_COM3_TX_PIN                 GPIO_Pin_9
#define EVAL_COM3_TX_GPIO_PORT           GPIOA
#define EVAL_COM3_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define EVAL_COM3_TX_SOURCE              GPIO_PinSource9
#define EVAL_COM3_TX_AF                  GPIO_AF_USART1
#define EVAL_COM3_RX_PIN                 GPIO_Pin_10
#define EVAL_COM3_RX_GPIO_PORT           GPIOA
#define EVAL_COM3_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define EVAL_COM3_RX_SOURCE              GPIO_PinSource10
#define EVAL_COM3_RX_AF                  GPIO_AF_USART1
#define EVAL_COM3_IRQn                   USART1_IRQn

/**
 * @brief Definition for COM port2, connected to USART3
 */
#define EVAL_COM2                        USART3
#define EVAL_COM2_CLK                    RCC_APB1Periph_USART3
#define EVAL_COM2_TX_PIN                 GPIO_Pin_10
#define EVAL_COM2_TX_GPIO_PORT           GPIOC
#define EVAL_COM2_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define EVAL_COM2_TX_SOURCE              GPIO_PinSource10
#define EVAL_COM2_TX_AF                  GPIO_AF_USART3
#define EVAL_COM2_RX_PIN                 GPIO_Pin_11
#define EVAL_COM2_RX_GPIO_PORT           GPIOC
#define EVAL_COM2_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define EVAL_COM2_RX_SOURCE              GPIO_PinSource11
#define EVAL_COM2_RX_AF                  GPIO_AF_USART3
#define EVAL_COM2_IRQn                   USART3_IRQn

/**
 * @brief Definition for COM port1, connected to USART6
 */
#define EVAL_COM1                        USART6
#define EVAL_COM1_CLK                    RCC_APB2Periph_USART6
#define EVAL_COM1_TX_PIN                 GPIO_Pin_6
#define EVAL_COM1_TX_GPIO_PORT           GPIOC
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource6
#define EVAL_COM1_TX_AF                  GPIO_AF_USART6
#define EVAL_COM1_RX_PIN                 GPIO_Pin_7
#define EVAL_COM1_RX_GPIO_PORT           GPIOC
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource7
#define EVAL_COM1_RX_AF                  GPIO_AF_USART6
#define EVAL_COM1_IRQn                   USART6_IRQn

/**
  * @}
  */ 	  

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(ButtonMode_TypeDef Button_Mode);
bool STM_EVAL_IsPressed();

void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_H */
