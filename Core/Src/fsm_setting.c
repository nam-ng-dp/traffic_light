#include "fsm_setting.h"
#include "fsm_automatic.h"
#include "traffic_light.h"
#include "led7seg.h"
#include "software_timer.h"
#include "button.h"

void fsm_setting(void) {
	switch (status) {
	case STATUS_SET_RED:
		// Transition 1: Button 1 cycles to SET_AMBER
		if (isButtonPressed(0)) {
			status = STATUS_SET_AMBER;
			temp_time = amber_time;
			turnOffAllTrafficLights();
			toggleAmber();
			setTimer(1, 250);
			updateMode7SEG(temp_time, 3);
			break;
		}
		// Transition 2: Button 2 increments value (1 - 99)
		if (isButtonPressed(1)) {
			temp_time++;
			if (temp_time > 99) {
				temp_time = 1;
			}
			updateMode7SEG(temp_time, 2);
			break;
		}
		// Transition 3: Button 3 saves red_time
		if (isButtonPressed(2)) {
			red_time = temp_time;
			break;
		}
		// Transition 4 (Self-loop): 2Hz blinking Red LEDs
		if (timer_flag[1] == 1) {
			setTimer(1, 250);
			toggleRed();
			break;
		}
		break;

	case STATUS_SET_AMBER:
		// Transition 1: Button 1 cycles to SET_GREEN
		if (isButtonPressed(0)) {
			status = STATUS_SET_GREEN;
			temp_time = green_time;
			turnOffAllTrafficLights();
			toggleGreen();
			setTimer(1, 250);
			updateMode7SEG(temp_time, 4);
			break;
		}
		// Transition 2: Button 2 increments value (1 - 99)
		if (isButtonPressed(1)) {
			temp_time++;
			if (temp_time > 99) {
				temp_time = 1;
			}
			updateMode7SEG(temp_time, 3);
			break;
		}
		// Transition 3: Button 3 saves amber_time
		if (isButtonPressed(2)) {
			amber_time = temp_time;
			break;
		}
		// Transition 4 (Self-loop): 2Hz blinking Amber LEDs
		if (timer_flag[1] == 1) {
			setTimer(1, 250);
			toggleAmber();
			break;
		}
		break;

	case STATUS_SET_GREEN:
		// Transition 1: Button 1 finishes setting and returns to Automatic mode
		if (isButtonPressed(0)) {
			turnOffAllTrafficLights();
			if (red_time <= amber_time) {
				red_time = amber_time + green_time;
			} else {
				green_time = red_time - amber_time;
			}
			fsm_automatic_init();
			break;
		}
		// Transition 2: Button 2 increments value (1 - 99)
		if (isButtonPressed(1)) {
			temp_time++;
			if (temp_time > 99) {
				temp_time = 1;
			}
			updateMode7SEG(temp_time, 4);
			break;
		}
		// Transition 3: Button 3 saves green_time
		if (isButtonPressed(2)) {
			green_time = temp_time;
			break;
		}
		// Transition 4 (Self-loop): 2Hz blinking Green LEDs
		if (timer_flag[1] == 1) {
			setTimer(1, 250);
			toggleGreen();
			break;
		}
		break;

	default:
		break;
	}
}
