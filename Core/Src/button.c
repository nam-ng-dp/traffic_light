#include "button.h"

// TODO: Add BTN_4 (BTN_4_GPIO_Port, BTN_4_Pin) and BTN_USER (BTN_USER_GPIO_Port, BTN_USER_Pin) when BTL features require them.
static GPIO_TypeDef *BUTTON_PORTS[NO_OF_BUTTONS] = {
		BTN_1_GPIO_Port, BTN_2_GPIO_Port, BTN_3_GPIO_Port };
static uint16_t BUTTON_PINS[NO_OF_BUTTONS] = {
		BTN_1_Pin, BTN_2_Pin, BTN_3_Pin };

static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {
		BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED };
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS] = {
		BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED };
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS] = {
		BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED };

static uint8_t flagForButtonPressed[NO_OF_BUTTONS] = { 0 };
static uint8_t flagForButtonPress1s[NO_OF_BUTTONS] = { 0 };
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS] = { 0 };

void button_reading(void) {
	for (int i = 0; i < NO_OF_BUTTONS; i++) {
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BUTTON_PORTS[i], BUTTON_PINS[i]);

		if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
			// Edge detection for single press event
			if (buttonBuffer[i] != debounceButtonBuffer1[i]) {
				buttonBuffer[i] = debounceButtonBuffer1[i];
				if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
					flagForButtonPressed[i] = 1;
				}
			}

			// Long press tracking (>= 1 second)
			if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
				if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
					counterForButtonPress1s[i]++;
				} else {
					flagForButtonPress1s[i] = 1;
				}
			} else {
				counterForButtonPress1s[i] = 0;
				flagForButtonPress1s[i] = 0;
			}
		}
	}
}

unsigned char is_button_pressed(unsigned char index) {
	if (index >= NO_OF_BUTTONS) return 0;
	return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index) {
	if (index >= NO_OF_BUTTONS) return 0;
	return (flagForButtonPress1s[index] == 1);
}

unsigned char isButtonPressed(unsigned char index) {
	if (index >= NO_OF_BUTTONS) return 0;
	if (flagForButtonPressed[index] == 1) {
		flagForButtonPressed[index] = 0;
		return 1;
	}
	return 0;
}
