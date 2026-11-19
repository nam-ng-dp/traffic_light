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
	BTN_USER_GPIO_Port
};

static uint16_t BUTTON_PINS[NO_OF_BUTTONS] = {
	BTN_1_Pin,
	BTN_2_Pin,
	BTN_3_Pin,
	BTN_4_Pin,
	BTN_USER_Pin
};

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
	BUTTON_IS_RELEASED, BUTTON_IS_RELEASED
};
static ButtonPinState_t debounceButtonBuffer1[NO_OF_BUTTONS] = {
	BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED,
	BUTTON_IS_RELEASED, BUTTON_IS_RELEASED
};
static ButtonPinState_t debounceButtonBuffer2[NO_OF_BUTTONS] = {
	BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED,
	BUTTON_IS_RELEASED, BUTTON_IS_RELEASED
};

static uint8_t flagForButtonPressed[NO_OF_BUTTONS] = { 0 };
static uint8_t flagForButtonPressed1s[NO_OF_BUTTONS] = { 0 };
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS] = { 0 };

static ButtonPinState_t read_button_pin(int index) {
#ifndef UNIT_TEST
	return HAL_GPIO_ReadPin(BUTTON_PORTS[index], BUTTON_PINS[index]);
#else
	return button_mock_pins[index];
#endif
}

void button_reset_all(void) {
	for (int i = 0; i < NO_OF_BUTTONS; i++) {
		buttonBuffer[i] = BUTTON_IS_RELEASED;
		debounceButtonBuffer1[i] = BUTTON_IS_RELEASED;
		debounceButtonBuffer2[i] = BUTTON_IS_RELEASED;
		flagForButtonPressed[i] = 0;
		flagForButtonPressed1s[i] = 0;
		counterForButtonPress1s[i] = 0;
	}
}

void button_reading(void) {
	for (int i = 0; i < NO_OF_BUTTONS; i++) {
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		debounceButtonBuffer1[i] = read_button_pin(i);

		if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
			/* Edge detection for single press event */
			if (buttonBuffer[i] != debounceButtonBuffer1[i]) {
				buttonBuffer[i] = debounceButtonBuffer1[i];
				if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
					flagForButtonPressed[i] = 1;
				}
			}

			/* Long press tracking (>= 1 second) */
			if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
				if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
					counterForButtonPress1s[i]++;
				} else {
					flagForButtonPressed1s[i] = 1;
				}
			} else {
				counterForButtonPress1s[i] = 0;
				flagForButtonPressed1s[i] = 0;
			}
		}
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
