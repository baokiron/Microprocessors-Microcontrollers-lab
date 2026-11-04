/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define bt1_Pin GPIO_PIN_13
#define bt1_GPIO_Port GPIOC
#define bt2_Pin GPIO_PIN_14
#define bt2_GPIO_Port GPIOC
#define bt3_Pin GPIO_PIN_15
#define bt3_GPIO_Port GPIOC
#define RED1_Pin GPIO_PIN_1
#define RED1_GPIO_Port GPIOA
#define RED2_Pin GPIO_PIN_2
#define RED2_GPIO_Port GPIOA
#define YELLOW1_Pin GPIO_PIN_3
#define YELLOW1_GPIO_Port GPIOA
#define YELLOW2_Pin GPIO_PIN_4
#define YELLOW2_GPIO_Port GPIOA
#define GREEN1_Pin GPIO_PIN_5
#define GREEN1_GPIO_Port GPIOA
#define GREEN2_Pin GPIO_PIN_6
#define GREEN2_GPIO_Port GPIOA

#define a1_Pin GPIO_PIN_1
#define a1_GPIO_Port GPIOB
#define b1_Pin GPIO_PIN_2
#define b1_GPIO_Port GPIOB
#define c1_Pin GPIO_PIN_3
#define c1_GPIO_Port GPIOB
#define d1_Pin GPIO_PIN_4
#define d1_GPIO_Port GPIOB
#define e1_Pin GPIO_PIN_5
#define e1_GPIO_Port GPIOB
#define f1_Pin GPIO_PIN_6
#define f1_GPIO_Port GPIOB
#define g1_Pin GPIO_PIN_7
#define g1_GPIO_Port GPIOB

#define a2_Pin GPIO_PIN_8
#define a2_GPIO_Port GPIOB
#define b2_Pin GPIO_PIN_9
#define b2_GPIO_Port GPIOB
#define c2_Pin GPIO_PIN_10
#define c2_GPIO_Port GPIOB
#define d2_Pin GPIO_PIN_11
#define d2_GPIO_Port GPIOB
#define e2_Pin GPIO_PIN_12
#define e2_GPIO_Port GPIOB
#define f2_Pin GPIO_PIN_13
#define f2_GPIO_Port GPIOB
#define g2_Pin GPIO_PIN_14
#define g2_GPIO_Port GPIOB

#define EN1_Pin GPIO_PIN_8
#define EN1_GPIO_Port GPIOA
#define EN2_Pin GPIO_PIN_9
#define EN2_GPIO_Port GPIOA
#define EN3_Pin GPIO_PIN_10
#define EN3_GPIO_Port GPIOA
#define EN4_Pin GPIO_PIN_11
#define EN4_GPIO_Port GPIOA

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
