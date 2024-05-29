#ifndef FAN_H_
#define FAN_H_

#include "main.h"

void Fan_Start(void);
void Fan_Output(float duty);
uint32_t Fan_Get_Spd(void);
float Fan_Get_Rpm(void);
void Fan_Exti_Callback(void);
#endif