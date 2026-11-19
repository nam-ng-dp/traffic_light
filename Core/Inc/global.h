#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include <stdint.h>

/* 4 Automatic Mode States */
#define STATUS_AUTO_RED_GREEN   1
#define STATUS_AUTO_RED_YELLOW  2
#define STATUS_AUTO_GREEN_RED   3
#define STATUS_AUTO_YELLOW_RED  4

/* 4 Manual Mode States */
#define STATUS_MAN_RED_GREEN    5
#define STATUS_MAN_RED_YELLOW   6
#define STATUS_MAN_GREEN_RED    7
#define STATUS_MAN_YELLOW_RED   8

/* 3 Setting Mode States */
#define STATUS_SET_RED          9
#define STATUS_SET_AMBER        10
#define STATUS_SET_GREEN        11

extern int status;

extern int red_time;
extern int amber_time;
extern int green_time;

extern int temp_time;

extern int temperature;
extern int humidity;

#endif /* INC_GLOBAL_H_ */
