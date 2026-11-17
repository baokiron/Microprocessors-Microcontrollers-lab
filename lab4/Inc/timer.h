/*
 * timer.h
 *
 *  He thong thoi gian (ms) dua tren TIM2, dung cho scheduler.
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"
#include <stdint.h>

/*
 * Chu ky ngat cua TIM2 (ms).
 * TIM2: Prescaler = 7999, Period = 9, clock 8 MHz  =>  8MHz / 8000 / 10 = 100 Hz = 10 ms
 * Moi khi doi cau hinh TIM2 trong .ioc thi phai sua lai so nay.
 */
#define TIMER_CYCLE 10

/* Goi trong ngat TIM2 moi TIMER_CYCLE ms */
void timer_run(void);

/* Thoi gian he thong (ms) ke tu luc bat timer, do chinh xac TIMER_CYCLE */
uint32_t get_time(void);

#endif /* INC_TIMER_H_ */
