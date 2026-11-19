#ifndef INC_FSM_AUTOMATIC_H_
#define INC_FSM_AUTOMATIC_H_

#include "global.h"

extern int count_dir1;
extern int count_dir2;

/**
 * @brief Load countdown values for both directions and update the LCD.
 */
void setCountdown(int time_dir1, int time_dir2);

/**
 * @brief Decrement countdown values every 1 second (clamped at >= 0) and update the LCD.
 */
void countdownTick(void);

/**
 * @brief Initialize Automatic traffic light mode.
 */
void fsm_automatic_init(void);

/**
 * @brief Run Automatic mode state machine.
 */
void fsm_automatic(void);

#endif /* INC_FSM_AUTOMATIC_H_ */
