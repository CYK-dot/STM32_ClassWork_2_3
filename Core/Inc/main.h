/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_ll_adc.h"

#include "TD_ADC.h"
#include "TD_Key.h"

#include "Fan.h"
#include "Timer_PID.h"
#include "Timetick_us.h"
#include "oled.h"
#include "Oled_Wave.h"

#include "FinalTask.h"
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM4_TICK_US 100
#define EXTI2_Pin GPIO_PIN_2
#define EXTI2_GPIO_Port GPIOA
#define EXTI2_EXTI_IRQn EXTI2_IRQn
#define EXTI3_Pin GPIO_PIN_3
#define EXTI3_GPIO_Port GPIOA
#define EXTI3_EXTI_IRQn EXTI3_IRQn
#define PWM3_Pin GPIO_PIN_0
#define PWM3_GPIO_Port GPIOB
#define KEY0_Pin GPIO_PIN_0
#define KEY0_GPIO_Port GPIOG
#define KEY0_EXTI_IRQn EXTI0_IRQn
#define KEY1_Pin GPIO_PIN_1
#define KEY1_GPIO_Port GPIOG
#define KEY1_EXTI_IRQn EXTI1_IRQn
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOB
#define DIO0_Pin GPIO_PIN_8
#define DIO0_GPIO_Port GPIOD
#define DIO1_Pin GPIO_PIN_9
#define DIO1_GPIO_Port GPIOD
#define DIO2_Pin GPIO_PIN_10
#define DIO2_GPIO_Port GPIOD
#define DIO3_Pin GPIO_PIN_11
#define DIO3_GPIO_Port GPIOD
#define CS_Pin GPIO_PIN_3
#define CS_GPIO_Port GPIOG
#define DC_Pin GPIO_PIN_4
#define DC_GPIO_Port GPIOG
#define RES_Pin GPIO_PIN_5
#define RES_GPIO_Port GPIOG
#define DIN_Pin GPIO_PIN_6
#define DIN_GPIO_Port GPIOG
#define CLK_Pin GPIO_PIN_7
#define CLK_GPIO_Port GPIOG
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
