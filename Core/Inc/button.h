#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#define NO_OF_BUTTONS 3
#define DURATION_FOR_AUTO_INCREASING 100 // 100 * 10ms = 1 second
#define BUTTON_IS_PRESSED GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

void button_reading(void);
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_1s(unsigned char index);
unsigned char isButtonPressed(unsigned char index);

#endif /* INC_BUTTON_H_ */
