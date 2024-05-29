#ifndef STEPMOTOR_H_
#define STEPMOTOR_H_

#include "main.h"

void StepMotor_Init(void);
void StepMotor_Action(uint8_t status);
int8_t StepMotor_Set_Spd_Us(uint32_t us_per_phase);
void StepMotor_Set_Zero(void);
int8_t StepMotor_Set_Spd(float ms_per_phase);
int64_t StepMotor_Get_Pos(void);
void StepMotor_TIM_Callback(TIM_HandleTypeDef *htim);

#endif