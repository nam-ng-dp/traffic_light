#include "software_timer.h"

int TIMER_CYCLE = 10; /* Timer interrupt period in ms (default TIM2 = 10ms) */
int timer_counter[MAX_TIMER] = { 0 };
int timer_flag[MAX_TIMER] = { 0 };

void setTimerCycle(int cycle_ms) {
	if (cycle_ms > 0) {
		TIMER_CYCLE = cycle_ms;
	}
}

void setTimer(int index, int duration) {
	if (index >= 0 && index < MAX_TIMER) {
		if (TIMER_CYCLE <= 0) {
			TIMER_CYCLE = 10;
		}
		int ticks = duration / TIMER_CYCLE;
		if (duration > 0 && ticks == 0) {
			ticks = 1;
		}
		timer_counter[index] = ticks;
		timer_flag[index] = 0;
	}
}

void clearTimer(int index) {
	if (index >= 0 && index < MAX_TIMER) {
		timer_counter[index] = 0;
		timer_flag[index] = 0;
	}
}

void timer_run(void) {
	for (int i = 0; i < MAX_TIMER; i++) {
		if (timer_counter[i] > 0) {
			timer_counter[i]--;
			if (timer_counter[i] <= 0) {
				timer_flag[i] = 1;
			}
		}
	}
}
