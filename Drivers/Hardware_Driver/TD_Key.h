#ifndef TD_KEY_H_
#define TD_KEY_H_

#include "main.h"
#define TD_KEY_TIMER htim13
extern TIM_HandleTypeDef TD_KEY_TIMER;

void TD_Key_Init(void);
void TD_Key_Callback(void);
uint32_t TD_Get_Key0_Count(void);
uint8_t TD_Get_Key0_Flag(void);
uint32_t TD_Get_Key1_Count(void);
uint8_t TD_Get_Key1_Flag(void);
uint8_t TD_Get_Key0_Trigger(void);
uint8_t TD_Get_Key1_Trigger(void);
#endif