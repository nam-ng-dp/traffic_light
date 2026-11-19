#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#define NO_OF_BUTTONS 5
#define DURATION_FOR_AUTO_INCREASING 100

void button_reading(void);
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_1s(unsigned char index);
unsigned char isButtonPressed(unsigned char index);
void button_reset_all(void);

#endif /* INC_BUTTON_H_ */
