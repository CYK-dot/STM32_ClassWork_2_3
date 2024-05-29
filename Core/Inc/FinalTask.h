#ifndef FINALTASK_H_
#define FINALTASK_H_

#include "main.h"
#include "FinalTask_StepMotor.h"

void FinalTask_Init(void);
void FinalTask_Ctrl(void);
void FinalTask_Log(void);
void FinalTask_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#endif