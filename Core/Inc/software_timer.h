#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define MAX_TIMER 10

extern int TIMER_CYCLE;
extern int timer_counter[MAX_TIMER];
extern int timer_flag[MAX_TIMER];

/**
 * @brief Set timer duration in milliseconds.
 *        Ticks are automatically calculated as duration / TIMER_CYCLE.
 *        If TIMER_CYCLE changes (e.g., from 10ms to 1ms or 50ms),
 *        the duration remains correct across the system.
 */
void setTimer(int index, int duration);

/**
 * @brief Set the hardware timer interrupt cycle in milliseconds.
 */
void setTimerCycle(int cycle_ms);

/**
 * @brief Run timer countdown, called every timer interrupt.
 */
void timer_run(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
