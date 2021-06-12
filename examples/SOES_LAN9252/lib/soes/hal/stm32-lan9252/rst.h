#ifndef __HAL_NRST_H__
#define __HAL_NRST_H__

#include <stdint.h>
#include "stm32f4xx_conf.h"

#define ESC_RCC_APB1PERIPH_GPIOX_RSTN   RCC_AHB1Periph_GPIOA
#define ESC_GPIOX_RSTN                  GPIOA
#define ESC_GPIO_Pin_RSTN               GPIO_Pin_2


void rst_setup(void);
void rst_low(void);
void rst_high(void);

void rst_check_start(void);
uint8_t is_esc_reset(void);

#endif /* __HAL_NRST_H__ */
