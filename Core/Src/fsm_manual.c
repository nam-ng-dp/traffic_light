#include "fsm_manual.h"
#include "fsm_automatic.h"
#include "traffic_light.h"
#include "lcd.h"
#include "button.h"
#include "software_timer.h"

typedef struct {
	int state;
	void (*set_lights)(void);
	int phase_id;
} ManualPhase;

static const ManualPhase manual_phases[] = {
	{ STATUS_MAN_RED_GREEN,  setTraffic_RedGreen,  1 },
	{ STATUS_MAN_RED_YELLOW, setTraffic_RedYellow, 2 },
	{ STATUS_MAN_GREEN_RED,  setTraffic_GreenRed,  3 },
	{ STATUS_MAN_YELLOW_RED, setTraffic_YellowRed, 4 }
};

void fsm_manual(void) {
	int idx = -1;
	int num_phases = (int)(sizeof(manual_phases) / sizeof(manual_phases[0]));
	for (int i = 0; i < num_phases; i++) {
		if (manual_phases[i].state == status) {
			idx = i;
			break;
		}
	}
	if (idx < 0) {
		return;
	}

	if (isButtonPressed(0)) {
		status = STATUS_SET_RED;
		temp_time = red_time;
		turnOffAllTrafficLights();
		toggleRed();
		setTimer(1, 250);
		lcd_update_setting(STATUS_SET_RED, temp_time);
		return;
	}

	if (isButtonPressed(1)) {
		int next = (idx + 1) % num_phases;
		status = manual_phases[next].state;
		manual_phases[next].set_lights();
		lcd_update_manual(manual_phases[next].phase_id);
		return;
	}

	if (isButtonPressed(2)) {
		int prev = (idx + num_phases - 1) % num_phases;
		status = manual_phases[prev].state;
		manual_phases[prev].set_lights();
		lcd_update_manual(manual_phases[prev].phase_id);
		return;
	}

	if (isButtonPressed(3)) {
		fsm_automatic_init();
	}
}
