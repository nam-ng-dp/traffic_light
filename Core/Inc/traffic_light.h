#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

#include "main.h"

// Semantic functions for the 4 traffic phases
void setTraffic_RedGreen(void);
void setTraffic_RedYellow(void);
void setTraffic_GreenRed(void);
void setTraffic_YellowRed(void);

// Low-level driver functions
void setDir1Light(GPIO_PinState red, GPIO_PinState yellow, GPIO_PinState green);
void setDir2Light(GPIO_PinState red, GPIO_PinState yellow, GPIO_PinState green);
void turnOffAllTrafficLights(void);
void toggleRed(void);
void toggleAmber(void);
void toggleGreen(void);

#endif /* INC_TRAFFIC_LIGHT_H_ */
