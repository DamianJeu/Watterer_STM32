/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
char* sprintf_v5(char *source, char *txt, uint8_t buf_size, uint8_t zeroEnable,
		unsigned int value, ...);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_PCB_Pin GPIO_PIN_13
#define LED_PCB_GPIO_Port GPIOC
#define ADC_CH0_Pin GPIO_PIN_0
#define ADC_CH0_GPIO_Port GPIOA
#define ADC_CH1_Pin GPIO_PIN_1
#define ADC_CH1_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_4
#define BUZZER_GPIO_Port GPIOA
#define ENCO_A_Pin GPIO_PIN_6
#define ENCO_A_GPIO_Port GPIOA
#define ENCO_B_Pin GPIO_PIN_7
#define ENCO_B_GPIO_Port GPIOA
#define ENCO_BTN_Pin GPIO_PIN_0
#define ENCO_BTN_GPIO_Port GPIOB
#define LED_BLUE_Pin GPIO_PIN_12
#define LED_BLUE_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_13
#define LED_GREEN_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOB
#define DS_Pin GPIO_PIN_15
#define DS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
