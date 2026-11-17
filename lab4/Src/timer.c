/*
 * timer.c
 *
 *  Created on: Oct 30, 2021
 *      Author: DELL
 */

#include "main.h"
#include "timer.h"
#include "sched.h"

static volatile uint32_t system_time_ms = 0;

uint32_t get_time(void) {
	return system_time_ms;
}

void timer_run(void) {
	system_time_ms += TIMER_CYCLE;
}

// Ngat TIM2 moi 10 ms: cap nhat thoi gian he thong va cap nhat scheduler
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		timer_run();
		SCH_Update();
	}
}
