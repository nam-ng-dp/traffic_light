#include "led7seg.h"

#ifdef LAB4_7SEG_HARDWARE
const uint16_t SEG_PINS[7] = {
		SEG0_Pin, SEG1_Pin, SEG2_Pin, SEG3_Pin, SEG4_Pin, SEG5_Pin, SEG6_Pin };
#else
const uint16_t SEG_PINS[7] = { 0 };
#endif

const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = { 0, 0, 0, 0 };

int hour = 15;
int minute = 8;
int second = 50;

/**
 * @brief hien thi led 7 doan anode chung->muc 0 la sang
 */
void display7SEG(int num) {
#ifdef LAB4_7SEG_HARDWARE
	const uint8_t SEG_CODE[10] = {
			0x3F, // 0: 0011 1111 (g off, a-f on)
			0x06, // 1: 0000 0110 (b, c on)
			0x5B, // 2: 0101 1011
			0x4F, // 3: 0100 1111
			0x66, // 4: 0110 0110
			0x6D, // 5: 0110 1101
			0x7D, // 6: 0111 1101
			0x07, // 7: 0000 0111
			0x7F, // 8: 0111 1111
			0x6F  // 9: 0110 1111
	};

	if (num >= 0 && num <= 9) {
		uint8_t code = SEG_CODE[num];
		for (int i = 0; i < 7; i++) {
			// anode chung: bit 1->xuat 0 (reset); bit 0:->xuat 1 (set)
			if ((code >> i) & 0x01) {
				HAL_GPIO_WritePin(GPIOB, SEG_PINS[i], GPIO_PIN_RESET);
			} else {
				HAL_GPIO_WritePin(GPIOB, SEG_PINS[i], GPIO_PIN_SET);
			}
		}
	}
#else
	(void)num;
	// TODO: replace 7SEG with I2C LCD module
#endif
}

void update7SEG(int index) {
#ifdef LAB4_7SEG_HARDWARE
	switch (index) {
	case 0:
		// bat led 1 (en0 = 0), tat led 2, 3, 4
		HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, EN1_Pin | EN2_Pin | EN3_Pin, GPIO_PIN_SET);
		display7SEG(led_buffer[0]);
		break;

	case 1:
		// bat led 2 (en1 = 0), tat led 1, 3, 4
		HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN2_Pin | EN3_Pin, GPIO_PIN_SET);
		display7SEG(led_buffer[1]);
		break;

	case 2:
		// bat led 3 (en2 = 0), tat led 1, 2, 4
		HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN1_Pin | EN3_Pin, GPIO_PIN_SET);
		display7SEG(led_buffer[2]);
		break;

	case 3:
		// bat led 4 (en3 = 0), tat led 1, 2, 3
		HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN1_Pin | EN2_Pin, GPIO_PIN_SET);
		display7SEG(led_buffer[3]);
		break;

	default:
		break;
	}
#else
	(void)index;
	// TODO: replace 7SEG with I2C LCD module
#endif
}

void updateClockBuffer(void) {
	led_buffer[0] = hour / 10;
	led_buffer[1] = hour % 10;
	led_buffer[2] = minute / 10;
	led_buffer[3] = minute % 10;
}

void updateMode7SEG(int value, int mode) {
	led_buffer[0] = value / 10;
	led_buffer[1] = value % 10;
	led_buffer[2] = mode / 10;
	led_buffer[3] = mode % 10;
}
