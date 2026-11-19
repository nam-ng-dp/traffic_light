#include "fsm_automatic.h"
#include "traffic_light.h"
#include "lcd.h"
#include "software_timer.h"
#include "button.h"

int count_dir1 = 0;
int count_dir2 = 0;

typedef struct {
	int auto_state;
	int manual_state;
	int manual_phase_id;
	void (*set_manual_lights)(void);
	int next_auto_state;
	void (*set_next_lights)(void);
	int *next_dir1_time;
	int *next_dir2_time;
	int *next_phase_duration;
} AutoPhaseTransition;

static const AutoPhaseTransition auto_transitions[] = {
	{ STATUS_AUTO_RED_GREEN,  STATUS_MAN_RED_GREEN,  1, setTraffic_RedGreen,
	  STATUS_AUTO_RED_YELLOW, setTraffic_RedYellow, &amber_time, &amber_time, &amber_time },
	{ STATUS_AUTO_RED_YELLOW, STATUS_MAN_RED_YELLOW, 2, setTraffic_RedYellow,
	  STATUS_AUTO_GREEN_RED,  setTraffic_GreenRed,  &green_time, &red_time,   &green_time },
	{ STATUS_AUTO_GREEN_RED,  STATUS_MAN_GREEN_RED,  3, setTraffic_GreenRed,
	  STATUS_AUTO_YELLOW_RED, setTraffic_YellowRed, &amber_time, &amber_time, &amber_time },
	{ STATUS_AUTO_YELLOW_RED, STATUS_MAN_YELLOW_RED, 4, setTraffic_YellowRed,
	  STATUS_AUTO_RED_GREEN,  setTraffic_RedGreen,  &red_time,   &green_time, &green_time }
};

void setCountdown(int time_dir1, int time_dir2) {
	count_dir1 = time_dir1;
	count_dir2 = time_dir2;
	lcd_update_auto(count_dir1, count_dir2);
}

void countdownTick(void) {
	if (count_dir1 > 0) {
		count_dir1--;
	}
	if (count_dir2 > 0) {
		count_dir2--;
	}
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
	const AutoPhaseTransition *phase = 0;
	for (unsigned int i = 0; i < sizeof(auto_transitions) / sizeof(auto_transitions[0]); i++) {
		if (auto_transitions[i].auto_state == status) {
			phase = &auto_transitions[i];
			break;
		}
	}
	if (phase == 0) {
		return;
	}

	if (isButtonPressed(0)) {
		status = phase->manual_state;
		phase->set_manual_lights();
		lcd_update_manual(phase->manual_phase_id);
		return;
	}

	if (timer_flag[0] == 1) {
		status = phase->next_auto_state;
		phase->set_next_lights();
		setCountdown(*(phase->next_dir1_time), *(phase->next_dir2_time));
		setTimer(0, *(phase->next_phase_duration) * 1000);
		setTimer(3, 1000);
		return;
	}

	if (timer_flag[3] == 1) {
		countdownTick();
	}
}
