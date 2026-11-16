#include "fsm_manual.h"
#include "traffic_light.h"
#include "led7seg.h"
#include "button.h"
#include "software_timer.h"

void fsm_manual(void) {
	switch (status) {
	case STATUS_MAN_RED_GREEN:
		// Transition 1: Button 1 switches to Setting Red mode
		if (isButtonPressed(0)) {
			status = STATUS_SET_RED;
			temp_time = red_time;
			turnOffAllTrafficLights();
			toggleRed();
			setTimer(1, 250);
			updateMode7SEG(temp_time, 2);
			break;
		}
		// Transition 2: Button 2 switches to Red - Yellow phase
		if (isButtonPressed(1) || isButtonPressed(2)) {
			status = STATUS_MAN_RED_YELLOW;
			setTraffic_RedYellow();
			updateMode7SEG(2, 0);
			break;
		}
		break;

	case STATUS_MAN_RED_YELLOW:
		// Transition 1: Button 1 switches to Setting Red mode
		if (isButtonPressed(0)) {
			status = STATUS_SET_RED;
			temp_time = red_time;
			turnOffAllTrafficLights();
			toggleRed();
			setTimer(1, 250);
			updateMode7SEG(temp_time, 2);
			break;
		}
		// Transition 2: Button 2 switches to Green - Red phase
		if (isButtonPressed(1) || isButtonPressed(2)) {
			status = STATUS_MAN_GREEN_RED;
			setTraffic_GreenRed();
			updateMode7SEG(3, 0);
			break;
		}
		break;

	case STATUS_MAN_GREEN_RED:
		// Transition 1: Button 1 switches to Setting Red mode
		if (isButtonPressed(0)) {
			status = STATUS_SET_RED;
			temp_time = red_time;
			turnOffAllTrafficLights();
			toggleRed();
			setTimer(1, 250);
			updateMode7SEG(temp_time, 2);
			break;
		}
		// Transition 2: Button 2 switches to Yellow - Red phase
		if (isButtonPressed(1) || isButtonPressed(2)) {
			status = STATUS_MAN_YELLOW_RED;
			setTraffic_YellowRed();
			updateMode7SEG(4, 0);
			break;
		}
		break;

	case STATUS_MAN_YELLOW_RED:
		// Transition 1: Button 1 switches to Setting Red mode
		if (isButtonPressed(0)) {
			status = STATUS_SET_RED;
			temp_time = red_time;
			turnOffAllTrafficLights();
			toggleRed();
			setTimer(1, 250);
			updateMode7SEG(temp_time, 2);
			break;
		}
		// Transition 2: Button 2 switches to Red - Green phase
		if (isButtonPressed(1) || isButtonPressed(2)) {
			status = STATUS_MAN_RED_GREEN;
			setTraffic_RedGreen();
			updateMode7SEG(1, 0);
			break;
		}
		break;

	default:
		break;
	}
}
