#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

#define TRAFFIC_LANE_1  1
#define TRAFFIC_LANE_2  2
#define TRAFFIC_OFF     0
#define TRAFFIC_GREEN   1
#define TRAFFIC_AMBER   2
#define TRAFFIC_RED     3

void traffic_set(int lane, int color);
int  traffic_get(int lane);
void setTraffic_RedGreen(void);
void setTraffic_RedYellow(void);
void setTraffic_GreenRed(void);
void setTraffic_YellowRed(void);
void turnOffAllTrafficLights(void);
void toggleRed(void);
void toggleAmber(void);
void toggleGreen(void);

#endif /* INC_TRAFFIC_LIGHT_H_ */
