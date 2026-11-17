/*
 * sched.h
 *
 *  Co-operative scheduler (HCMUT - Microcontroller lab).
 *  Don vi delay / period cua SCH_Add_Task la TICK (1 tick = SCH_TICK_MS ms).
 *  Dung MS_TO_TICKS(ms) de doi tu ms sang tick.
 */

#ifndef INC_SCHED_H_
#define INC_SCHED_H_

#include "main.h"
#include "timer.h"
#include <stdint.h>
#include <stddef.h>

/* ---------------- Cau hinh ---------------- */
#define SCH_MAX_TASKS   10                   // phai >= so task su dung
#define SCH_TICK_MS     TIMER_CYCLE          // 1 tick = chu ky ngat TIM2 (10 ms)
#define MS_TO_TICKS(ms) ((ms) / SCH_TICK_MS)

/* Bo comment dong duoi neu muon xuat ma loi ra cong LED (can tu dinh nghia Led_Error_Port).
 * Mach hien tai khong co LED bao loi nen mac dinh TAT. */
// #define SCH_ALLOW_REPORT_ERROR
// #define Led_Error_Port  GPIOB->ODR

/* ---------------- Ma loi ---------------- */
#define ERROR_SCH_TOO_MANY_TASKS        1
#define ERROR_SCH_CANNOT_DELETE_TASK    2

#define RETURN_CODE_ERROR_DELETE        0
#define RETURN_CODE_SUCCESS_DELETE      1

/* ---------------- Cau truc task ---------------- */
typedef struct {
	void (*funcPtr)(void);   // con tro toi task (void (void))
	uint32_t delay;          // so tick con lai den lan chay (ke tiep)
	uint32_t period;         // chu ky (tick); 0 = one-shot
	volatile uint8_t run_me; // tang boi SCH_Update khi task den han
} sTask;

extern sTask taskArr[SCH_MAX_TASKS];
extern unsigned char ERROR_CODE;

/* ---------------- API ---------------- */
void     SCH_Init(void);
uint32_t SCH_Add_Task(void (*funcPtr)(void), uint32_t delay, uint32_t period); // tra ve ID (= index), SCH_MAX_TASKS neu day
void     SCH_Update(void);          // goi trong ngat timer
void     SCH_Dispatch_Tasks(void);  // goi trong while(1)
uint8_t  SCH_Delete_Task(uint32_t taskID);
void     SCH_Sleep(void);
void     SCH_Report_Status(void);

#endif /* INC_SCHED_H_ */
