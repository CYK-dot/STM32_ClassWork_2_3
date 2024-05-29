#ifndef TIMER_PID_H_
#define TIMER_PID_H_

#include "stm32f4xx_hal.h"

//定义PID结构体
typedef struct {
float P;
float I;
float D;
float Last_Error;			//上次误差	
float Last_Output;
float Limit_Output;  	//输出上限
float Limit_Integral; //积分项上限
float Integral; 			//积分项,存储积分误差	
}PID_Struct;


typedef struct {
	PID_Struct*        pid_param;
	float             pid_output;
	float             pid_target;
	float             pid_current;
	
	TIM_HandleTypeDef* ticker_htim;
	uint32_t           ctrl_tick_us;
}Timer_PID_t;


typedef struct{
	float dece;
	float acce;
	float last_value;
	float current_value;
}Dace_t;


void PID_Init(Timer_PID_t* ctrler,PID_Struct* pid_p,TIM_HandleTypeDef* htim_p,uint32_t ctrl_tick_ms);
void PID_Start(Timer_PID_t* ctrler);
void PID_Stop(Timer_PID_t* ctrler);
void PID_Ctrl(Timer_PID_t* ctrler,float target,float current);
float PID_Normal_Ctrl(float target,float current,PID_Struct *_pid);
void  Dace_Ctrl_Init(float dece,float acce,float start_output,Dace_t* _dace);
float Dace_Normal_Ctrl(float target,Dace_t* _dace);
float PID_Dace_Ctrl(float target,float current,float dace_max,PID_Struct *_pid);
void HAL_TIM_PeriodElapsedCallback_For_PID(TIM_HandleTypeDef *htim);
#endif