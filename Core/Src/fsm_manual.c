#include "fsm_manual.h"
#include "fsm_automatic.h"
#include "traffic_light.h"
#include "lcd.h"
#include "button.h"
#include "software_timer.h"

void fsm_manual(void) {
	switch (status) {
	case STATUS_MAN_RED_GREEN:
		if (isButtonPressed(0)) {
			status = STATUS_SET_RED;
			temp_time = red_time;
			turnOffAllTrafficLights();
			toggleRed();
			setTimer(1, 250);
			lcd_update_setting(STATUS_SET_RED, temp_time);
			break;
		}
		if (isButtonPressed(1)) {
			status = STATUS_MAN_RED_YELLOW;
			setTraffic_RedYellow();
			lcd_update_manual(2);
			break;
		}
		if (isButtonPressed(2)) {
			status = STATUS_MAN_YELLOW_RED;
			setTraffic_YellowRed();
			lcd_update_manual(4);
			break;
		}
		if (isButtonPressed(3)) {
			fsm_automatic_init();
			break;
		}
		break;

	case STATUS_MAN_RED_YELLOW:
		if (isButtonPressed(0)) {
			status = STATUS_SET_RED;
			temp_time = red_time;
			turnOffAllTrafficLights();
			toggleRed();
			setTimer(1, 250);
			lcd_update_setting(STATUS_SET_RED, temp_time);
			break;
		}
		if (isButtonPressed(1)) {
			status = STATUS_MAN_GREEN_RED;
			setTraffic_GreenRed();
			lcd_update_manual(3);
			break;
		}
		if (isButtonPressed(2)) {
			status = STATUS_MAN_RED_GREEN;
			setTraffic_RedGreen();
			lcd_update_manual(1);
			break;
		}
		if (isButtonPressed(3)) {
			fsm_automatic_init();
			break;
		}
		break;

	case STATUS_MAN_GREEN_RED:
		if (isButtonPressed(0)) {
			status = STATUS_SET_RED;
			temp_time = red_time;
			turnOffAllTrafficLights();
			toggleRed();
			setTimer(1, 250);
			lcd_update_setting(STATUS_SET_RED, temp_time);
			break;
		}
		if (isButtonPressed(1)) {
			status = STATUS_MAN_YELLOW_RED;
			setTraffic_YellowRed();
			lcd_update_manual(4);
			break;
		}
		if (isButtonPressed(2)) {
			status = STATUS_MAN_RED_YELLOW;
			setTraffic_RedYellow();
			lcd_update_manual(2);
			break;
		}
		if (isButtonPressed(3)) {
			fsm_automatic_init();
			break;
		}
		break;

	case STATUS_MAN_YELLOW_RED:
		if (isButtonPressed(0)) {
			status = STATUS_SET_RED;
			temp_time = red_time;
			turnOffAllTrafficLights();
			toggleRed();
			setTimer(1, 250);
			lcd_update_setting(STATUS_SET_RED, temp_time);
			break;
		}
		if (isButtonPressed(1)) {
			status = STATUS_MAN_RED_GREEN;
			setTraffic_RedGreen();
			lcd_update_manual(1);
			break;
		}
		if (isButtonPressed(2)) {
			status = STATUS_MAN_GREEN_RED;
			setTraffic_GreenRed();
			lcd_update_manual(3);
			break;
		}
		if (isButtonPressed(3)) {
			fsm_automatic_init();
			break;
		}
		break;

	default:
		break;
	}
}
