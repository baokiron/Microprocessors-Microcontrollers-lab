/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   Pin definitions khop voi so do Proteus + CubeMX:
  *                   PA1..PA5 = task1..task5, PA6 = one_shot, PA9/PA10 = USART1
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
#define task1_Pin GPIO_PIN_1
#define task1_GPIO_Port GPIOA
#define task2_Pin GPIO_PIN_2
#define task2_GPIO_Port GPIOA
#define task3_Pin GPIO_PIN_3
#define task3_GPIO_Port GPIOA
#define task4_Pin GPIO_PIN_4
#define task4_GPIO_Port GPIOA
#define task5_Pin GPIO_PIN_5
#define task5_GPIO_Port GPIOA
#define one_shot_Pin GPIO_PIN_6
#define one_shot_GPIO_Port GPIOA
#define TXD_Pin GPIO_PIN_9
#define TXD_GPIO_Port GPIOA
#define RXD_Pin GPIO_PIN_10
#define RXD_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
