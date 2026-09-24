/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t segmentNumber[10] = {
    0x40,  // Số 0
    0x79,  // Số 1
    0x24,  // Số 2
    0x30,  // Số 3
    0x19,  // Số 4
    0x12,  // Số 5
    0x02,  // Số 6
    0x78,  // Số 7
    0x00,  // Số 8
    0x10   // Số 9
};

// Hàm xuất hiển thị cho LED 7 đoạn hướng 1 (Chân PB1 -> PB7)
void display7SEG1(int num) {
    if (num < 0 || num > 9) return;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, ((segmentNumber[num] >> 0) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, ((segmentNumber[num] >> 1) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, ((segmentNumber[num] >> 2) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, ((segmentNumber[num] >> 3) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, ((segmentNumber[num] >> 4) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, ((segmentNumber[num] >> 5) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, ((segmentNumber[num] >> 6) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// Hàm xuất hiển thị cho LED 7 đoạn hướng 2 (Chân PB8 -> PB14)
void display7SEG2(int num) {
    if (num < 0 || num > 9) return;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, ((segmentNumber[num] >> 0) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, ((segmentNumber[num] >> 1) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, ((segmentNumber[num] >> 2) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, ((segmentNumber[num] >> 3) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, ((segmentNumber[num] >> 4) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, ((segmentNumber[num] >> 5) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, ((segmentNumber[num] >> 6) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  int time_counter = 0;   // Biến đếm thời gian chu kỳ (0 đến 9)
  int count_val1 = 0;     // Số đếm ngược hiển thị trên LED 1
  int count_val2 = 0;     // Số đếm ngược hiển thị trên LED 2
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  // 1. Xử lý trạng thái đèn giao thông và số đếm ngược
	        if (time_counter < 3) {
	            // GIAI ĐOẠN 1 (3s): Đèn 1 ĐỎ, Đèn 2 XANH
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   // RED1 Bật
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET); // YELLOW1 Tắt
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // GREEN1 Tắt

	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET); // RED2 Tắt
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // YELLOW2 Tắt
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);   // GREEN2 Bật

	            count_val1 = 5 - time_counter; // Đèn đỏ 1 đếm ngược: 5, 4, 3
	            count_val2 = 3 - time_counter; // Đèn xanh 2 đếm ngược: 3, 2, 1
	        }
	        else if (time_counter < 5) {
	            // GIAI ĐOẠN 2 (2s): Đèn 1 ĐỎ, Đèn 2 VÀNG
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // GREEN2 Tắt
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // YELLOW2 Bật

	            count_val1 = 5 - time_counter; // Đèn đỏ 1 đếm ngược: 2, 1
	            count_val2 = 5 - time_counter; // Đèn vàng 2 đếm ngược: 2, 1
	        }
	        else if (time_counter < 8) {
	            // GIAI ĐOẠN 3 (3s): Đèn 1 XANH, Đèn 2 ĐỎ
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // RED1 Tắt
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);   // GREEN1 Bật

	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // YELLOW2 Tắt
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);   // RED2 Bật

	            count_val1 = 8 - time_counter;  // Đèn xanh 1 đếm ngược: 3, 2, 1
	            count_val2 = 10 - time_counter; // Đèn đỏ 2 đếm ngược: 5, 4, 3
	        }
	        else if (time_counter < 10) {
	            // GIAI ĐOẠN 4 (2s): Đèn 1 VÀNG, Đèn 2 ĐỎ
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // GREEN1 Tắt
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);   // YELLOW1 Bật

	            count_val1 = 10 - time_counter; // Đèn vàng 1 đếm ngược: 2, 1
	            count_val2 = 10 - time_counter; // Đèn đỏ 2 đếm ngược: 2, 1
	        }

	        // 2. Xuất giá trị thời gian ra 2 cụm LED 7 đoạn
	        display7SEG1(count_val1);
	        display7SEG2(count_val2);

	        // 3. Delay 1 giây và tăng biến thời gian
	        HAL_Delay(1000);
	        time_counter++;
	        if (time_counter >= 10) {
	            time_counter = 0; // Lặp lại chu kỳ mới
	        }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RED1_Pin|RED2_Pin|YELLOW1_Pin|YELLOW2_Pin
                          |GREEN1_Pin|GREEN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, a1_Pin|b1_Pin|c2_Pin|d2_Pin
                          |e2_Pin|f2_Pin|g2_Pin|c1_Pin
                          |d1_Pin|e1_Pin|f1_Pin|g1_Pin
                          |a2_Pin|b2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RED1_Pin RED2_Pin YELLOW1_Pin YELLOW2_Pin
                           GREEN1_Pin GREEN2_Pin */
  GPIO_InitStruct.Pin = RED1_Pin|RED2_Pin|YELLOW1_Pin|YELLOW2_Pin
                          |GREEN1_Pin|GREEN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : a1_Pin b1_Pin c2_Pin d2_Pin
                           e2_Pin f2_Pin g2_Pin c1_Pin
                           d1_Pin e1_Pin f1_Pin g1_Pin
                           a2_Pin b2_Pin */
  GPIO_InitStruct.Pin = a1_Pin|b1_Pin|c2_Pin|d2_Pin
                          |e2_Pin|f2_Pin|g2_Pin|c1_Pin
                          |d1_Pin|e1_Pin|f1_Pin|g1_Pin
                          |a2_Pin|b2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
