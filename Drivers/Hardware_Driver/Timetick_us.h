#ifndef _TIMETICK_US_H_
#define _TIMETICK_US_H_

#include "main.h"

#define TD_TICK_TIMER htim11
extern TIM_HandleTypeDef TD_TICK_TIMER;

void Timetick_Reset(void);
void Timetick_Start(void);
void Timetick_End(void);
void Timetick_Callback(TIM_HandleTypeDef *htim);
uint64_t Timetick_Get(void);
int Count_Delay(int count_num,int num);
#endif