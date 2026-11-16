#include "scheduler.h"
#include "main.h"
#include <string.h>

sTask SCH_tasks_G[SCH_MAX_TASKS];
uint8_t SCH_count = 0;
static uint32_t new_task_id = 0;

static inline uint32_t SCH_Lock(void) {
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	return primask;
}

static inline void SCH_Unlock(uint32_t primask) {
	__set_PRIMASK(primask);
}

static uint32_t Get_New_Task_ID(void) {
	new_task_id++;
	if (new_task_id == NO_TASK_ID) {
		new_task_id++;
	}
	return new_task_id;
}

static uint32_t Insert(void (*pF)(void), uint32_t delay, uint32_t period, uint32_t id) {
	uint32_t m = SCH_Lock();
	if (SCH_count >= SCH_MAX_TASKS) {
		SCH_Unlock(m);
		return NO_TASK_ID;
	}
	uint8_t idx = 0;
	uint32_t sum = 0;
	while (idx < SCH_count && sum + SCH_tasks_G[idx].Delay <= delay) {
		sum += SCH_tasks_G[idx++].Delay;
	}
	for (uint8_t i = SCH_count; i > idx; i--) {
		SCH_tasks_G[i] = SCH_tasks_G[i - 1];
	}
	sTask *t = &SCH_tasks_G[idx];
	t->pTask = pF;
	t->Delay = delay - sum;
	t->Period = period;
	t->TaskID = id;
	t->RunMe = (idx == 0 && t->Delay == 0);
	if (idx < SCH_count) {
		SCH_tasks_G[idx + 1].Delay -= t->Delay;
	}
	SCH_count++;
	SCH_Unlock(m);
	return id;
}

void SCH_Init(void) {
	uint32_t m = SCH_Lock();
	memset(SCH_tasks_G, 0, sizeof(SCH_tasks_G));
	SCH_count = 0;
	new_task_id = 0;
	SCH_Unlock(m);
}

void SCH_Update(void) {
	if (SCH_count == 0) return;
	if (SCH_tasks_G[0].Delay > 0) {
		SCH_tasks_G[0].Delay--;
	}
	if (SCH_tasks_G[0].Delay == 0) {
		SCH_tasks_G[0].RunMe = 1;
	}
}

uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD) {
	if (pFunction == 0) return NO_TASK_ID;
	uint32_t delay = DELAY / TICK;
	uint32_t period = PERIOD / TICK;
	if (DELAY > 0 && delay == 0) delay = 1;
	if (PERIOD > 0 && period == 0) period = 1;
	uint32_t id = Get_New_Task_ID();
	return Insert(pFunction, delay, period, id);
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
	if (taskID == NO_TASK_ID) return 0;
	uint32_t m = SCH_Lock();
	for (uint8_t i = 0; i < SCH_count; i++) {
		if (SCH_tasks_G[i].TaskID == taskID) {
			if (i + 1 < SCH_count) {
				SCH_tasks_G[i + 1].Delay += SCH_tasks_G[i].Delay;
				SCH_tasks_G[i + 1].RunMe = (i == 0 && SCH_tasks_G[i + 1].Delay == 0);
			}
			for (uint8_t j = i; j + 1 < SCH_count; j++) {
				SCH_tasks_G[j] = SCH_tasks_G[j + 1];
			}
			SCH_count--;
			memset(&SCH_tasks_G[SCH_count], 0, sizeof(sTask));
			SCH_Unlock(m);
			return 1;
		}
	}
	SCH_Unlock(m);
	return 0;
}

void SCH_Dispatch_Tasks(void) {
	while (SCH_count > 0 && SCH_tasks_G[0].RunMe) {
		sTask t = SCH_tasks_G[0];
		t.pTask();
		if (SCH_Delete_Task(t.TaskID) && t.Period > 0) {
			Insert(t.pTask, t.Period, t.Period, t.TaskID);
		}
	}
}
