#ifndef INC_FSM_AUTOMATIC_H_
#define INC_FSM_AUTOMATIC_H_

#include "global.h"

extern int count_dir1;
extern int count_dir2;

void setCountdown(int time_dir1, int time_dir2);
void countdownTick(void);

void fsm_automatic_init(void);
void fsm_automatic(void);

#endif /* INC_FSM_AUTOMATIC_H_ */
