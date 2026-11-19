#include "fsm_automatic.h"
#include "traffic_light.h"
#include "lcd.h"
#include "software_timer.h"
#include "button.h"

int count_dir1 = 0;
int count_dir2 = 0;

void setCountdown(int time_dir1, int time_dir2) {
	count_dir1 = time_dir1;
	count_dir2 = time_dir2;
	lcd_update_auto(count_dir1, count_dir2);
}

void countdownTick(void) {
	count_dir1 = (count_dir1 > 0) ? (count_dir1 - 1) : 0;
	count_dir2 = (count_dir2 > 0) ? (count_dir2 - 1) : 0;
	lcd_update_auto(count_dir1, count_dir2);
	setTimer(3, 1000);
}

void fsm_automatic_init(void) {
	status = STATUS_AUTO_RED_GREEN;
	setTraffic_RedGreen();
	setCountdown(red_time, green_time);
	setTimer(0, green_time * 1000);
	setTimer(3, 1000);
}

void fsm_automatic(void) {
	switch (status) {
	case STATUS_AUTO_RED_GREEN:
		if (isButtonPressed(0)) {
			status = STATUS_MAN_RED_GREEN;
			setTraffic_RedGreen();
			lcd_update_manual(1);
			break;
		}
		if (timer_flag[0] == 1) {
			status = STATUS_AUTO_RED_YELLOW;
			setTraffic_RedYellow();
			setCountdown(amber_time, amber_time);
			setTimer(0, amber_time * 1000);
			setTimer(3, 1000);
			break;
		}
		if (timer_flag[3] == 1) {
			countdownTick();
			break;
		}
		break;

	case STATUS_AUTO_RED_YELLOW:
		if (isButtonPressed(0)) {
			status = STATUS_MAN_RED_YELLOW;
			setTraffic_RedYellow();
			lcd_update_manual(2);
			break;
		}
		if (timer_flag[0] == 1) {
			status = STATUS_AUTO_GREEN_RED;
			setTraffic_GreenRed();
			setCountdown(green_time, red_time);
			setTimer(0, green_time * 1000);
			setTimer(3, 1000);
			break;
		}
		if (timer_flag[3] == 1) {
			countdownTick();
			break;
		}
		break;

	case STATUS_AUTO_GREEN_RED:
		if (isButtonPressed(0)) {
			status = STATUS_MAN_GREEN_RED;
			setTraffic_GreenRed();
			lcd_update_manual(3);
			break;
		}
		if (timer_flag[0] == 1) {
			status = STATUS_AUTO_YELLOW_RED;
			setTraffic_YellowRed();
			setCountdown(amber_time, amber_time);
			setTimer(0, amber_time * 1000);
			setTimer(3, 1000);
			break;
		}
		if (timer_flag[3] == 1) {
			countdownTick();
			break;
		}
		break;

	case STATUS_AUTO_YELLOW_RED:
		if (isButtonPressed(0)) {
			status = STATUS_MAN_YELLOW_RED;
			setTraffic_YellowRed();
			lcd_update_manual(4);
			break;
		}
		if (timer_flag[0] == 1) {
			status = STATUS_AUTO_RED_GREEN;
			setTraffic_RedGreen();
			setCountdown(red_time, green_time);
			setTimer(0, green_time * 1000);
			setTimer(3, 1000);
			break;
		}
		if (timer_flag[3] == 1) {
			countdownTick();
			break;
		}
		break;

	default:
		break;
	}
}
