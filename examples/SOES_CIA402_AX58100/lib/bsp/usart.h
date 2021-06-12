#ifndef __USART_H__
#define __USART_H__

#include <stdio.h>

#include "stm32f4xx_conf.h"



// USART6, TX - PC6
#define APP_USART                        USART6
#define APP_USART_CLK                    RCC_APB2Periph_USART6
#define APP_USART_TX_PIN                 GPIO_Pin_6
#define APP_USART_TX_GPIO_PORT           GPIOC
#define APP_USART_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define APP_USART_TX_SOURCE              GPIO_PinSource6
#define APP_USART_TX_AF                  GPIO_AF_USART6
#define APP_USART_RX_PIN                 GPIO_Pin_7
#define APP_USART_RX_GPIO_PORT           GPIOC
#define APP_USART_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define APP_USART_RX_SOURCE              GPIO_PinSource7
#define APP_USART_RX_AF                  GPIO_AF_USART6
#define APP_USART_IRQn                   USART6_IRQn


void APP_USART_Init();
void APP_USART_Send(char *c);
char * STR5(uint32_t val);
char * HEX4B(uint32_t val);
char * DEC4B(uint32_t val);

#endif // __USART_H__