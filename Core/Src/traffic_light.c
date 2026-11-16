#include "traffic_light.h"

/**
 * @brief Semantic control: Phase 1 - Road 1 RED, Road 2 GREEN.
 */
void setTraffic_RedGreen(void) {
	setDir1Light(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET);
	setDir2Light(GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET);
}

/**
 * @brief Semantic control: Phase 2 - Road 1 RED, Road 2 YELLOW.
 */
void setTraffic_RedYellow(void) {
	setDir1Light(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET);
	setDir2Light(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET);
}

/**
 * @brief Semantic control: Phase 3 - Road 1 GREEN, Road 2 RED.
 */
void setTraffic_GreenRed(void) {
	setDir1Light(GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET);
	setDir2Light(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET);
}

/**
 * @brief Semantic control: Phase 4 - Road 1 YELLOW, Road 2 RED.
 */
void setTraffic_YellowRed(void) {
	setDir1Light(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET);
	setDir2Light(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET);
}

/**
 * @brief Control Road 1 (North-South) traffic lights (Active-low: RESET = ON, SET = OFF).
 */
void setDir1Light(GPIO_PinState red, GPIO_PinState yellow, GPIO_PinState green) {
#ifdef LAB4_6PIN_HARDWARE
	HAL_GPIO_WritePin(GPIOA, RED1_Pin, red);
	HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin, yellow);
	HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, green);
#else
	(void)red;
	(void)yellow;
	(void)green;
	// TODO: Map Road 1 traffic light to XNUCLEO-F103RB 2-bit pins (TL1_A_GPIO_Port/Pin, TL1_B_GPIO_Port/Pin)
#endif
}

/**
 * @brief Control Road 2 (East-West) traffic lights (Active-low: RESET = ON, SET = OFF).
 */
void setDir2Light(GPIO_PinState red, GPIO_PinState yellow, GPIO_PinState green) {
#ifdef LAB4_6PIN_HARDWARE
	HAL_GPIO_WritePin(GPIOA, RED2_Pin, red);
	HAL_GPIO_WritePin(GPIOA, YELLOW2_Pin, yellow);
	HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, green);
#else
	(void)red;
	(void)yellow;
	(void)green;
	// TODO: Map Road 2 traffic light to XNUCLEO-F103RB 2-bit pins (TL2_A_GPIO_Port/Pin, TL2_B_GPIO_Port/Pin)
#endif
}

/**
 * @brief Turn off all traffic light LEDs in both directions.
 */
void turnOffAllTrafficLights(void) {
#ifdef LAB4_6PIN_HARDWARE
	HAL_GPIO_WritePin(GPIOA,
			RED1_Pin | YELLOW1_Pin | GREEN1_Pin | RED2_Pin | YELLOW2_Pin | GREEN2_Pin,
			GPIO_PIN_SET);
#else
	// TODO: Turn off TL1_A/B and TL2_A/B on XNUCLEO-F103RB hardware
#endif
}

/**
 * @brief Toggle Red LEDs in both directions (2Hz blinking mode), others OFF.
 */
void toggleRed(void) {
#ifdef LAB4_6PIN_HARDWARE
	HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin | GREEN1_Pin | YELLOW2_Pin | GREEN2_Pin, GPIO_PIN_SET);
	HAL_GPIO_TogglePin(GPIOA, RED1_Pin | RED2_Pin);
#else
	// TODO: Toggle Red state on TL1_A/B and TL2_A/B on XNUCLEO-F103RB hardware
#endif
}

/**
 * @brief Toggle Amber/Yellow LEDs in both directions (2Hz blinking mode), others OFF.
 */
void toggleAmber(void) {
#ifdef LAB4_6PIN_HARDWARE
	HAL_GPIO_WritePin(GPIOA, RED1_Pin | GREEN1_Pin | RED2_Pin | GREEN2_Pin, GPIO_PIN_SET);
	HAL_GPIO_TogglePin(GPIOA, YELLOW1_Pin | YELLOW2_Pin);
#else
	// TODO: Toggle Amber state on TL1_A/B and TL2_A/B on XNUCLEO-F103RB hardware
#endif
}

/**
 * @brief Toggle Green LEDs in both directions (2Hz blinking mode), others OFF.
 */
void toggleGreen(void) {
#ifdef LAB4_6PIN_HARDWARE
	HAL_GPIO_WritePin(GPIOA, RED1_Pin | YELLOW1_Pin | RED2_Pin | YELLOW2_Pin, GPIO_PIN_SET);
	HAL_GPIO_TogglePin(GPIOA, GREEN1_Pin | GREEN2_Pin);
#else
	// TODO: Toggle Green state on TL1_A/B and TL2_A/B on XNUCLEO-F103RB hardware
#endif
}
