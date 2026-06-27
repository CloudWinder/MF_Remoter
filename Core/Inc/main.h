/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NRF_IRQ_Pin GPIO_PIN_13
#define NRF_IRQ_GPIO_Port GPIOC
#define NRF_IRQ_EXTI_IRQn EXTI15_10_IRQn
#define BEEP_Pin GPIO_PIN_14
#define BEEP_GPIO_Port GPIOC
#define KEY_R_Pin GPIO_PIN_15
#define KEY_R_GPIO_Port GPIOC
#define NRF_CE_Pin GPIO_PIN_3
#define NRF_CE_GPIO_Port GPIOA
#define SPI_CSN_Pin GPIO_PIN_4
#define SPI_CSN_GPIO_Port GPIOA
#define KEY_S1_Pin GPIO_PIN_10
#define KEY_S1_GPIO_Port GPIOB
#define KEY_L_Pin GPIO_PIN_11
#define KEY_L_GPIO_Port GPIOB
#define OLED_RS_Pin GPIO_PIN_12
#define OLED_RS_GPIO_Port GPIOB
#define OLED_RST_Pin GPIO_PIN_14
#define OLED_RST_GPIO_Port GPIOB
#define KEY_S2_Pin GPIO_PIN_8
#define KEY_S2_GPIO_Port GPIOA
#define LED_BULE_Pin GPIO_PIN_3
#define LED_BULE_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_7
#define LED_RED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
