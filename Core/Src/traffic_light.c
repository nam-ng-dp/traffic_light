#include "traffic_light.h"
#include <stdint.h>

#ifndef UNIT_TEST
#include "main.h"
#endif

/*
 * 2-bit encoded traffic light lookup table: { pin_A, pin_B }
 * Adjust rows here if Step 3.2 hardware test reveals different wiring/polarity.
 */
typedef struct {
	uint8_t bit_a;
	uint8_t bit_b;
} TrafficPinCode_t;

static const TrafficPinCode_t TRAFFIC_LUT[4] = {
	{ 0, 0 }, /* TRAFFIC_OFF   */
	{ 0, 1 }, /* TRAFFIC_GREEN */
	{ 1, 0 }, /* TRAFFIC_AMBER */
	{ 1, 1 }  /* TRAFFIC_RED   */
};

static int lane1_color = TRAFFIC_OFF;
static int lane2_color = TRAFFIC_OFF;

static void traffic_toggle_color(int color) {
	int next = (lane1_color == color && lane2_color == color) ? TRAFFIC_OFF : color;
	traffic_set(TRAFFIC_LANE_1, next);
	traffic_set(TRAFFIC_LANE_2, next);
}

void traffic_set(int lane, int color) {
	if (color < TRAFFIC_OFF || color > TRAFFIC_RED) {
		color = TRAFFIC_OFF;
	}

#ifndef UNIT_TEST
	GPIO_PinState pin_a = TRAFFIC_LUT[color].bit_a ? GPIO_PIN_SET : GPIO_PIN_RESET;
	GPIO_PinState pin_b = TRAFFIC_LUT[color].bit_b ? GPIO_PIN_SET : GPIO_PIN_RESET;
#else
	(void)TRAFFIC_LUT;
#endif

	if (lane == TRAFFIC_LANE_1) {
		lane1_color = color;
#ifndef UNIT_TEST
		HAL_GPIO_WritePin(TL1_A_GPIO_Port, TL1_A_Pin, pin_a);
		HAL_GPIO_WritePin(TL1_B_GPIO_Port, TL1_B_Pin, pin_b);
#endif
	} else if (lane == TRAFFIC_LANE_2) {
		lane2_color = color;
#ifndef UNIT_TEST
		HAL_GPIO_WritePin(TL2_A_GPIO_Port, TL2_A_Pin, pin_a);
		HAL_GPIO_WritePin(TL2_B_GPIO_Port, TL2_B_Pin, pin_b);
#endif
	}
}

int traffic_get(int lane) {
	if (lane == TRAFFIC_LANE_1) {
		return lane1_color;
	}
	if (lane == TRAFFIC_LANE_2) {
		return lane2_color;
	}
	return TRAFFIC_OFF;
}

void setTraffic_RedGreen(void) {
	traffic_set(TRAFFIC_LANE_1, TRAFFIC_RED);
	traffic_set(TRAFFIC_LANE_2, TRAFFIC_GREEN);
}

void setTraffic_RedYellow(void) {
	traffic_set(TRAFFIC_LANE_1, TRAFFIC_RED);
	traffic_set(TRAFFIC_LANE_2, TRAFFIC_AMBER);
}

void setTraffic_GreenRed(void) {
	traffic_set(TRAFFIC_LANE_1, TRAFFIC_GREEN);
	traffic_set(TRAFFIC_LANE_2, TRAFFIC_RED);
}

void setTraffic_YellowRed(void) {
	traffic_set(TRAFFIC_LANE_1, TRAFFIC_AMBER);
	traffic_set(TRAFFIC_LANE_2, TRAFFIC_RED);
}

void turnOffAllTrafficLights(void) {
	traffic_set(TRAFFIC_LANE_1, TRAFFIC_OFF);
	traffic_set(TRAFFIC_LANE_2, TRAFFIC_OFF);
}

void toggleRed(void) {
	traffic_toggle_color(TRAFFIC_RED);
}

void toggleAmber(void) {
	traffic_toggle_color(TRAFFIC_AMBER);
}

void toggleGreen(void) {
	traffic_toggle_color(TRAFFIC_GREEN);
}
