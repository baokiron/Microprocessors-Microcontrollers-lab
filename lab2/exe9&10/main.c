/* USER CODE BEGIN Header */
/**
  ****************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ****************************************************************************
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
  ****************************************************************************
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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
const int MAX_LED_MATRIX = 8;
int current_matrix_col = 0;

// Mảng hiển thị chữ "A" viết bằng Binary để dễ hình dung (1 là sáng, 0 là tắt)
uint8_t matrix_buffer[8] = {
    0b00000000, // Cột 0: Trống
    0b01111100, // Cột 1: Thân trái
    0b00010010, // Cột 2: Nét gạch & móc
    0b00010001, // Cột 3: Đỉnh chữ A
    0b00010010, // Cột 4: Nét gạch & móc
    0b01111100, // Cột 5: Thân phải
    0b00000000, // Cột 6: Trống
    0b00000000  // Cột 7: Trống
};

// Định nghĩa cấu trúc Software Timer cực kỳ chuyên nghiệp
typedef struct {
    int counter;
    int flag;
} SoftTimer;

SoftTimer timer_scan;  // Timer dùng để quét LED
SoftTimer timer_shift; // Timer dùng để dịch chữ
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void updateLEDMatrix(int index);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
const int TIMER_TICK = 10; // Ngắt mỗi 10ms

// Hàm cài đặt thời gian cho bất kỳ timer nào
void timer_set(SoftTimer* timer, int duration) {
    timer->counter = duration / TIMER_TICK;
    timer->flag = 0;
}

// Hàm chạy đếm ngược (gọi trong ngắt)
void timer_run(SoftTimer* timer) {
    if (timer->counter > 0) {
        timer->counter--;
        if (timer->counter == 0) {
            timer->flag = 1;
        }
    }
}

// Hàm dịch chữ sang trái (Animation Bài 10)
void shift_matrix_left() {
    uint8_t first_column = matrix_buffer[0];
    for (int i = 0; i < 7; i++) {
        matrix_buffer[i] = matrix_buffer[i + 1];
    }
    matrix_buffer[7] = first_column;
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim2);

    // Khởi tạo 2 timer
    timer_set(&timer_scan, 10);
    timer_set(&timer_shift, 100);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        // 1. Quét hiển thị Ma trận LED (Bài 9)
        if (timer_scan.flag == 1) {
            timer_set(&timer_scan, 10);

            updateLEDMatrix(current_matrix_col);

            current_matrix_col++;
            if (current_matrix_col >= MAX_LED_MATRIX) {
                current_matrix_col = 0;
            }
        }

        // 2. Tạo hiệu ứng chữ chạy sang trái (Bài 10)
        if (timer_shift.flag == 1) {
            timer_set(&timer_shift, 100);
            shift_matrix_left();
        }
      /* USER CODE END WHILE */
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|ENM2_Pin|ENM3_Pin
                          |ENM4_Pin|ENM5_Pin|ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ROW2_Pin|ROW3_Pin|ROW4_Pin|ROW5_Pin
                          |ROW6_Pin|ROW7_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENM0_Pin ENM1_Pin ENM2_Pin ENM3_Pin
                           ENM4_Pin ENM5_Pin ENM6_Pin ENM7_Pin */
  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|ENM2_Pin|ENM3_Pin
                          |ENM4_Pin|ENM5_Pin|ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ROW2_Pin ROW3_Pin ROW4_Pin ROW5_Pin
                           ROW6_Pin ROW7_Pin ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = ROW2_Pin|ROW3_Pin|ROW4_Pin|ROW5_Pin
                          |ROW6_Pin|ROW7_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void updateLEDMatrix(int index) {
    // 1. Xóa sạch các cột (Chống hiện tượng bóng mờ)
    HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|ENM2_Pin|ENM3_Pin|
                             ENM4_Pin|ENM5_Pin|ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

    // 2. Xuất dữ liệu ra các hàng (Dùng phép dịch bit cực nhanh)
    uint8_t col_data = matrix_buffer[index];
    HAL_GPIO_WritePin(GPIOB, ROW0_Pin, (col_data >> 0) & 0x01);
    HAL_GPIO_WritePin(GPIOB, ROW1_Pin, (col_data >> 1) & 0x01);
    HAL_GPIO_WritePin(GPIOB, ROW2_Pin, (col_data >> 2) & 0x01);
    HAL_GPIO_WritePin(GPIOB, ROW3_Pin, (col_data >> 3) & 0x01);
    HAL_GPIO_WritePin(GPIOB, ROW4_Pin, (col_data >> 4) & 0x01);
    HAL_GPIO_WritePin(GPIOB, ROW5_Pin, (col_data >> 5) & 0x01);
    HAL_GPIO_WritePin(GPIOB, ROW6_Pin, (col_data >> 6) & 0x01);
    HAL_GPIO_WritePin(GPIOB, ROW7_Pin, (col_data >> 7) & 0x01);

    // 3. Kích hoạt đúng cột đang quét
    switch (index) {
        case 0: HAL_GPIO_WritePin(GPIOA, ENM0_Pin, GPIO_PIN_SET); break;
        case 1: HAL_GPIO_WritePin(GPIOA, ENM1_Pin, GPIO_PIN_SET); break;
        case 2: HAL_GPIO_WritePin(GPIOA, ENM2_Pin, GPIO_PIN_SET); break;
        case 3: HAL_GPIO_WritePin(GPIOA, ENM3_Pin, GPIO_PIN_SET); break;
        case 4: HAL_GPIO_WritePin(GPIOA, ENM4_Pin, GPIO_PIN_SET); break;
        case 5: HAL_GPIO_WritePin(GPIOA, ENM5_Pin, GPIO_PIN_SET); break;
        case 6: HAL_GPIO_WritePin(GPIOA, ENM6_Pin, GPIO_PIN_SET); break;
        case 7: HAL_GPIO_WritePin(GPIOA, ENM7_Pin, GPIO_PIN_SET); break;
        default: break;
    }
}

// Trình phục vụ ngắt Timer 2
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        // Cập nhật tất cả các Software Timer đang có
        timer_run(&timer_scan);
        timer_run(&timer_shift);
    }
}
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
