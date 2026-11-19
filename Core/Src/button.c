#include "button.h"
#include <stdint.h>

#ifndef UNIT_TEST
#include "main.h"

/*
 * Active-low buttons by default (pull-up, pressed = GND).
 * If hardware test Step 3.3 shows active-high buttons, flip these two macros:
 *   #define BUTTON_IS_PRESSED  GPIO_PIN_SET
 *   #define BUTTON_IS_RELEASED GPIO_PIN_RESET
 */
#define BUTTON_IS_PRESSED  GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

static GPIO_TypeDef *BUTTON_PORTS[NO_OF_BUTTONS] = {
BTN_1_GPIO_Port,
BTN_2_GPIO_Port,
BTN_3_GPIO_Port,
BTN_4_GPIO_Port,
BTN_USER_GPIO_Port };

static uint16_t BUTTON_PINS[NO_OF_BUTTONS] = {
BTN_1_Pin,
BTN_2_Pin,
BTN_3_Pin,
BTN_4_Pin,
BTN_USER_Pin };

typedef GPIO_PinState ButtonPinState_t;
#else
#define BUTTON_IS_PRESSED  0
#define BUTTON_IS_RELEASED 1
typedef uint8_t ButtonPinState_t;

uint8_t button_mock_pins[NO_OF_BUTTONS] = {
	BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED,
	BUTTON_IS_RELEASED, BUTTON_IS_RELEASED
};
#endif

static ButtonPinState_t buttonBuffer[NO_OF_BUTTONS] = {
BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED,
BUTTON_IS_RELEASED, BUTTON_IS_RELEASED };
static ButtonPinState_t debounceButtonBuffer1[NO_OF_BUTTONS] = {
BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED,
BUTTON_IS_RELEASED, BUTTON_IS_RELEASED };
static ButtonPinState_t debounceButtonBuffer2[NO_OF_BUTTONS] = {
BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED,
BUTTON_IS_RELEASED, BUTTON_IS_RELEASED };

#define BUTTON_STATE_RELEASED  0
#define BUTTON_STATE_PRESSED   1
#define BUTTON_STATE_HOLD_1S   2

static uint8_t flagForButtonPressed[NO_OF_BUTTONS] = { 0 };
static uint8_t flagForButtonPressed1s[NO_OF_BUTTONS] = { 0 };
static uint8_t button_state[NO_OF_BUTTONS] = {
BUTTON_STATE_RELEASED, BUTTON_STATE_RELEASED, BUTTON_STATE_RELEASED,
BUTTON_STATE_RELEASED, BUTTON_STATE_RELEASED };
static uint16_t timerForKeyPress[NO_OF_BUTTONS] = {
DURATION_FOR_AUTO_INCREASING, DURATION_FOR_AUTO_INCREASING,
DURATION_FOR_AUTO_INCREASING, DURATION_FOR_AUTO_INCREASING,
DURATION_FOR_AUTO_INCREASING };

static ButtonPinState_t read_button_pin(int index) {
#ifndef UNIT_TEST
	return HAL_GPIO_ReadPin(BUTTON_PORTS[index], BUTTON_PINS[index]);
#else
	return button_mock_pins[index];
#endif
}

static void button_sample_pin(unsigned char i) {
	debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
	debounceButtonBuffer1[i] = read_button_pin(i);
	if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
		buttonBuffer[i] = debounceButtonBuffer1[i];
	}
}

static void button_fsm_step(unsigned char i) {
	switch (button_state[i]) {
	case BUTTON_STATE_RELEASED:
		if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
			button_state[i] = BUTTON_STATE_PRESSED;
			timerForKeyPress[i] = DURATION_FOR_AUTO_INCREASING;
			flagForButtonPressed[i] = 1;
			break;
		}
		break;

	case BUTTON_STATE_PRESSED:
		if (buttonBuffer[i] == BUTTON_IS_RELEASED) {
			button_state[i] = BUTTON_STATE_RELEASED;
			flagForButtonPressed1s[i] = 0;
			timerForKeyPress[i] = DURATION_FOR_AUTO_INCREASING;
			break;
		}
		if (timerForKeyPress[i] > 1) {
			timerForKeyPress[i]--;
			break;
		}
		if (timerForKeyPress[i] <= 1) {
			timerForKeyPress[i] = 0;
			flagForButtonPressed1s[i] = 1;
			button_state[i] = BUTTON_STATE_HOLD_1S;
			break;
		}
		break;

	case BUTTON_STATE_HOLD_1S:
		if (buttonBuffer[i] == BUTTON_IS_RELEASED) {
			button_state[i] = BUTTON_STATE_RELEASED;
			flagForButtonPressed1s[i] = 0;
			timerForKeyPress[i] = DURATION_FOR_AUTO_INCREASING;
			break;
		}
		break;

	default:
		button_state[i] = BUTTON_STATE_RELEASED;
		break;
	}
}

void button_reset_all(void) {
	for (int i = 0; i < NO_OF_BUTTONS; i++) {
		buttonBuffer[i] = BUTTON_IS_RELEASED;
		debounceButtonBuffer1[i] = BUTTON_IS_RELEASED;
		debounceButtonBuffer2[i] = BUTTON_IS_RELEASED;
		flagForButtonPressed[i] = 0;
		flagForButtonPressed1s[i] = 0;
		button_state[i] = BUTTON_STATE_RELEASED;
		timerForKeyPress[i] = DURATION_FOR_AUTO_INCREASING;
	}
}

void button_reading(void) {
	for (unsigned char i = 0; i < NO_OF_BUTTONS; i++) {
		button_sample_pin(i);
		button_fsm_step(i);
	}
}

unsigned char is_button_pressed(unsigned char index) {
	if (index >= NO_OF_BUTTONS) {
		return 0;
	}
	return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index) {
	if (index >= NO_OF_BUTTONS) {
		return 0;
	}
	return (flagForButtonPressed1s[index] == 1);
}

unsigned char isButtonPressed(unsigned char index) {
	if (index >= NO_OF_BUTTONS) {
		return 0;
	}
	if (flagForButtonPressed[index] == 1) {
		flagForButtonPressed[index] = 0;
		return 1;
	}
	return 0;
}
