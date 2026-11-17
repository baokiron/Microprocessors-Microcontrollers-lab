/*
 * sched.c
 *
 *  Created on: Oct 30, 2021
 *      Author: DELL
 */
#include "sched.h"
#include "main.h"

sTask taskArr[SCH_MAX_TASKS];
unsigned char ERROR_CODE = 0;

void SCH_Init(void) {
	for (uint32_t i = 0; i < SCH_MAX_TASKS; ++i) {
		taskArr[i].funcPtr = NULL;
		taskArr[i].delay = 0;
		taskArr[i].period = 0;
		taskArr[i].run_me = 0;
	}
	ERROR_CODE = 0;
}

uint32_t SCH_Add_Task(void (*funcPtr)(void), uint32_t delay, uint32_t period) {
	uint32_t idx = 0;

	// Tim o trong (kiem tra bien mang TRUOC khi doc phan tu)
	while ((idx < SCH_MAX_TASKS) && (taskArr[idx].funcPtr != NULL))
		++idx;

	if (idx == SCH_MAX_TASKS) {
		// Danh sach day
		ERROR_CODE = ERROR_SCH_TOO_MANY_TASKS;
		return SCH_MAX_TASKS;
	}

	// Ghi funcPtr CUOI CUNG de ISR khong doc phai task dang khoi tao do
	taskArr[idx].delay = delay;
	taskArr[idx].period = period;
	taskArr[idx].run_me = 0;
	taskArr[idx].funcPtr = funcPtr;
	return idx; // dung de xoa task sau nay
}

// Goi trong ngat timer (moi 1 tick)
void SCH_Update(void) {
	for (uint32_t idx = 0; idx < SCH_MAX_TASKS; ++idx) {
		if (taskArr[idx].funcPtr) {
			if (taskArr[idx].delay == 0) {
				// Task den han
				++taskArr[idx].run_me;

				// Task tuan hoan: nap lai delay.
				// Dung (period - 1) vi tick nay da tinh la 1 tick,
				// nhu vay khoang cach giua 2 lan chay dung bang period tick.
				if (taskArr[idx].period)
					taskArr[idx].delay = taskArr[idx].period - 1;
			} else {
				--taskArr[idx].delay;
			}
		}
	}
}

void SCH_Dispatch_Tasks(void) {
	for (uint32_t idx = 0; idx < SCH_MAX_TASKS; ++idx) {
		if (taskArr[idx].run_me > 0 && taskArr[idx].funcPtr != NULL) {
			(*taskArr[idx].funcPtr)(); // chay task

			// run_me duoc ISR tang -> giam trong vung bao ve
			__disable_irq();
			if (taskArr[idx].run_me > 0)
				--taskArr[idx].run_me;
			__enable_irq();

			// Task one-shot (period == 0) thi xoa khoi mang
			// (neu task tu xoa chinh no thi funcPtr da NULL, khong xoa lan nua)
			if (taskArr[idx].period == 0 && taskArr[idx].funcPtr != NULL)
				SCH_Delete_Task(idx);
		}
	}

	SCH_Report_Status();
	SCH_Sleep();
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
	if (taskID >= SCH_MAX_TASKS || taskArr[taskID].funcPtr == NULL) {
		ERROR_CODE = ERROR_SCH_CANNOT_DELETE_TASK;
		return RETURN_CODE_ERROR_DELETE;
	}

	__disable_irq();
	taskArr[taskID].funcPtr = NULL;
	taskArr[taskID].delay = 0;
	taskArr[taskID].period = 0;
	taskArr[taskID].run_me = 0;
	__enable_irq();
	return RETURN_CODE_SUCCESS_DELETE;
}

void SCH_Sleep(void) {
	// Idle mode la tinh nang tuy chon. Mo hinh Cortex-M3 cua Proteus (CM3_STM32.DLL)
	// co the bao "access violation" khi gap lenh WFI, nen mac dinh de trong.
	// Tren mach that, bo comment dong duoi de tiet kiem dien.
	// __WFI();
}

void SCH_Report_Status(void) {
#ifdef SCH_ALLOW_REPORT_ERROR // define trong sched.h
	static unsigned char lastError = 0;
	static unsigned int errorTimer_tick = 0;
	// Co loi moi?
	if (ERROR_CODE != lastError) {
		// LED active-low
		Led_Error_Port = 255 - ERROR_CODE;
		lastError = ERROR_CODE;
		errorTimer_tick = (ERROR_CODE != 0) ? 60000 : 0; // don vi: tick
	} else {
		if (errorTimer_tick == 0)
			ERROR_CODE = 0; // het thoi gian hien thi -> reset
		else
			--errorTimer_tick;
	}
#endif
}
