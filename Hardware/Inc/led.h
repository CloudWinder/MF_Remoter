#ifndef __LED_H
#define __LED_H	 
#include "stm32f1xx_hal.h"


#define LED_BLUE 		PBout(3)
#define LED_RED 		PBout(7)

#define LED_BLUE_ON()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)
#define LED_BLUE_OFF()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)
#define LED_RED_ON()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)
#define LED_RED_OFF()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)

void led_key_test(void);
			    
#endif
