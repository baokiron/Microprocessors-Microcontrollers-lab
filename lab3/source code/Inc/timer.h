#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

#define TIMER_TICK_MS     10   // Chu kỳ ngắt TIM2 thực tế
#define LOGIC_PERIOD_MS   10   // Chu kỳ FSM

void timer_run(void);
uint8_t timer_logic_ready(void);

#endif
