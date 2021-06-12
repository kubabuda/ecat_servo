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
 #include "stm32f4xx.h"
 #include "stm32f4xx_conf.h"

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Types
  * @{
  */

typedef enum
{
  BUTTON_USER = 0,
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;
/**
  * @}
  */

/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL_LED
  * @{
  */

#ifdef STM32DISCO_F4

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


#elif defined(F407Z_ECAT_DEVKIT_HW)

#define LEDn                             8

typedef enum
{
  LED0 = 0,
  LED1 = 1,
  LED2 = 2,
  LED3 = 3,
  LED4 = 4,
  LED5 = 5,
  LED6 = 6,
  LED7 = 7
} Led_TypeDef;


#define LED0_PIN                        GPIO_Pin_15
#define LED0_GPIO_PORT                  GPIOC
#define LED0_GPIO_CLK                   RCC_AHB1Periph_GPIOC

#define LED1_PIN                        GPIO_Pin_14
#define LED1_GPIO_PORT                  GPIOC
#define LED1_GPIO_CLK                   RCC_AHB1Periph_GPIOC

#define LED2_PIN                        GPIO_Pin_13
#define LED2_GPIO_PORT                  GPIOC
#define LED2_GPIO_CLK                   RCC_AHB1Periph_GPIOC

#define LED3_PIN                        GPIO_Pin_6
#define LED3_GPIO_PORT                  GPIOE
#define LED3_GPIO_CLK                   RCC_AHB1Periph_GPIOE

#define LED4_PIN                        GPIO_Pin_5
#define LED4_GPIO_PORT                  GPIOE
#define LED4_GPIO_CLK                   RCC_AHB1Periph_GPIOE

#define LED5_PIN                        GPIO_Pin_4
#define LED5_GPIO_PORT                  GPIOE
#define LED5_GPIO_CLK                   RCC_AHB1Periph_GPIOE

#define LED6_PIN                        GPIO_Pin_3
#define LED6_GPIO_PORT                  GPIOE
#define LED6_GPIO_CLK                   RCC_AHB1Periph_GPIOE

#define LED7_PIN                        GPIO_Pin_2
#define LED7_GPIO_PORT                  GPIOE
#define LED7_GPIO_CLK                   RCC_AHB1Periph_GPIOE


#define OUTPUT_BITS_COUNT    LEDn


extern const Led_TypeDef outPins[OUTPUT_BITS_COUNT];


#else 
  #error "LED blinkenlights for selected harware not configured"
#endif

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
#ifdef STM32DISCO_F4

#define USER_BUTTON_PIN                GPIO_Pin_0
#define USER_BUTTON_GPIO_PORT          GPIOA
#define USER_BUTTON_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define USER_BUTTON_EXTI_LINE          EXTI_Line0
#define USER_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOA
#define USER_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource0
#define USER_BUTTON_EXTI_IRQn          EXTI0_IRQn

#elif defined(F407Z_ECAT_DEVKIT_HW)

#define USER_BUTTON_PIN                GPIO_Pin_1
#define USER_BUTTON_GPIO_PORT          GPIOE
#define USER_BUTTON_GPIO_CLK           RCC_AHB1Periph_GPIOE
#define USER_BUTTON_EXTI_LINE          EXTI_Line1
#define USER_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOE
#define USER_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource1
#define USER_BUTTON_EXTI_IRQn          EXTI0_IRQn

#endif

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
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_H */
