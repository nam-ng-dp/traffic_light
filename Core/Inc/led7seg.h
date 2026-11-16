#ifndef INC_LED7SEG_H_
#define INC_LED7SEG_H_

#include "main.h"

extern const uint16_t SEG_PINS[7];
extern const int MAX_LED;
extern int index_led;
extern int led_buffer[4];

extern int hour;
extern int minute;
extern int second;

void display7SEG(int num);
void update7SEG(int index);
void updateClockBuffer(void);
void updateMode7SEG(int value, int mode);

#endif /* INC_LED7SEG_H_ */
