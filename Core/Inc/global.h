#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"

// 4 Trạng thái Tự động
#define STATUS_AUTO_RED_GREEN   1
#define STATUS_AUTO_RED_YELLOW  2
#define STATUS_AUTO_GREEN_RED   3
#define STATUS_AUTO_YELLOW_RED  4

// 4 Trạng thái Điều khiển bằng tay (Manual)
#define STATUS_MAN_RED_GREEN    5
#define STATUS_MAN_RED_YELLOW   6
#define STATUS_MAN_GREEN_RED    7
#define STATUS_MAN_YELLOW_RED   8

// 3 Trạng thái Cài đặt thời gian (Setting)
#define STATUS_SET_RED          9
#define STATUS_SET_AMBER        10
#define STATUS_SET_GREEN        11

extern int status;

extern int red_time;
extern int amber_time;
extern int green_time;

extern int temp_time;

#endif /* INC_GLOBAL_H_ */
