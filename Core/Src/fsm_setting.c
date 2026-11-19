#include "fsm_setting.h"
#include "fsm_automatic.h"
#include "traffic_light.h"
#include "lcd.h"
#include "software_timer.h"
#include "button.h"

typedef struct {
	int state;
	int *target_time;
	void (*toggle_led)(void);
} SettingConfig;

static const SettingConfig setting_configs[] = {
	{ STATUS_SET_RED,   &red_time,   toggleRed   },
	{ STATUS_SET_AMBER, &amber_time, toggleAmber },
	{ STATUS_SET_GREEN, &green_time, toggleGreen }
};

void fsm_setting(void) {
	int idx = -1;
	int num_configs = (int)(sizeof(setting_configs) / sizeof(setting_configs[0]));
	for (int i = 0; i < num_configs; i++) {
		if (setting_configs[i].state == status) {
			idx = i;
			break;
		}
	}
	if (idx < 0) {
		return;
	}

	const SettingConfig *cfg = &setting_configs[idx];

	if (isButtonPressed(0)) {
		if (idx == num_configs - 1) {
			turnOffAllTrafficLights();
			fsm_automatic_init();
			return;
		}
		const SettingConfig *next = &setting_configs[idx + 1];
		status = next->state;
		temp_time = *(next->target_time);
		turnOffAllTrafficLights();
		next->toggle_led();
		setTimer(1, 250);
		lcd_update_setting(next->state, temp_time);
		return;
	}

	if (isButtonPressed(1)) {
		temp_time++;
		if (temp_time > 99) {
			temp_time = 1;
		}
		lcd_update_setting(cfg->state, temp_time);
		return;
	}

	if (isButtonPressed(2)) {
		temp_time--;
		if (temp_time < 1) {
			temp_time = 99;
		}
		lcd_update_setting(cfg->state, temp_time);
		return;
	}

	if (isButtonPressed(3) || isButtonPressed(4)) {
		*(cfg->target_time) = temp_time;
		lcd_update_setting(cfg->state, temp_time);
		return;
	}

	if (timer_flag[1] == 1) {
		setTimer(1, 250);
		cfg->toggle_led();
	}
}
