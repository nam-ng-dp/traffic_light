#include "fsm_setting.h"
#include "fsm_automatic.h"
#include "traffic_light.h"
#include "lcd.h"
#include "software_timer.h"
#include "button.h"

static void increaseTempTime(void) {
	temp_time = (temp_time >= 99) ? 1 : (temp_time + 1);
}

static void decreaseTempTime(void) {
	temp_time = (temp_time <= 1) ? 99 : (temp_time - 1);
}

void fsm_setting(void) {
	switch (status) {
	case STATUS_SET_RED:
		if (isButtonPressed(0)) {
			status = STATUS_SET_AMBER;
			temp_time = amber_time;
			turnOffAllTrafficLights();
			toggleAmber();
			setTimer(1, 250);
			lcd_update_setting(STATUS_SET_AMBER, temp_time);
			break;
		}
		if (isButtonPressed(1)) {
			increaseTempTime();
			lcd_update_setting(STATUS_SET_RED, temp_time);
			break;
		}
		if (isButtonPressed(2)) {
			decreaseTempTime();
			lcd_update_setting(STATUS_SET_RED, temp_time);
			break;
		}
		if (isButtonPressed(3) || isButtonPressed(4)) {
			red_time = temp_time;
			lcd_update_setting(STATUS_SET_RED, temp_time);
			break;
		}
		if (timer_flag[1] == 1) {
			setTimer(1, 250);
			toggleRed();
			break;
		}
		break;

	case STATUS_SET_AMBER:
		if (isButtonPressed(0)) {
			status = STATUS_SET_GREEN;
			temp_time = green_time;
			turnOffAllTrafficLights();
			toggleGreen();
			setTimer(1, 250);
			lcd_update_setting(STATUS_SET_GREEN, temp_time);
			break;
		}
		if (isButtonPressed(1)) {
			increaseTempTime();
			lcd_update_setting(STATUS_SET_AMBER, temp_time);
			break;
		}
		if (isButtonPressed(2)) {
			decreaseTempTime();
			lcd_update_setting(STATUS_SET_AMBER, temp_time);
			break;
		}
		if (isButtonPressed(3) || isButtonPressed(4)) {
			amber_time = temp_time;
			lcd_update_setting(STATUS_SET_AMBER, temp_time);
			break;
		}
		if (timer_flag[1] == 1) {
			setTimer(1, 250);
			toggleAmber();
			break;
		}
		break;

	case STATUS_SET_GREEN:
		if (isButtonPressed(0)) {
			turnOffAllTrafficLights();
			fsm_automatic_init();
			break;
		}
		if (isButtonPressed(1)) {
			increaseTempTime();
			lcd_update_setting(STATUS_SET_GREEN, temp_time);
			break;
		}
		if (isButtonPressed(2)) {
			decreaseTempTime();
			lcd_update_setting(STATUS_SET_GREEN, temp_time);
			break;
		}
		if (isButtonPressed(3) || isButtonPressed(4)) {
			green_time = temp_time;
			lcd_update_setting(STATUS_SET_GREEN, temp_time);
			break;
		}
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
