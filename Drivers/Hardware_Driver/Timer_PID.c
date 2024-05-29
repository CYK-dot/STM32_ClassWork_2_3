/*---------头文件--------------------------------------*/
#include "Timer_PID.h"
#include "math.h"
/*---------配置宏--------------------------------------*/


/*---------全局变量------------------------------------*/
Timer_PID_t* Registered_pid[15];
uint8_t      Registered_pid_count;


/*---------函数接口-------------------------------------*/
void TimerPID_Init(Timer_PID_t* ctrler,PID_Struct* pid_p,TIM_HandleTypeDef* htim_p,uint32_t ctrl_tick_ms)
{
	ctrler->pid_param=pid_p;
	ctrler->ticker_htim=htim_p;
	__HAL_TIM_SET_AUTORELOAD(htim_p,ctrl_tick_ms*10);
	__HAL_TIM_DISABLE(htim_p);
	
	Registered_pid[Registered_pid_count]=ctrler;
	Registered_pid_count++;
}
void TimerPID_Start(Timer_PID_t* ctrler)
{
	__HAL_TIM_ENABLE(ctrler->ticker_htim);
}
void TimerPID_Stop(Timer_PID_t* ctrler)
{
	__HAL_TIM_DISABLE(ctrler->ticker_htim);
}
void TimerPID_Ctrl(Timer_PID_t* ctrler,float target,float current)
{
	__HAL_TIM_DISABLE_IT(ctrler->ticker_htim,TIM_IT_UPDATE);
	ctrler->pid_target=target;
	ctrler->pid_current=current;
	__HAL_TIM_ENABLE_IT(ctrler->ticker_htim,TIM_IT_UPDATE);
}

float PID_Normal_Ctrl(float target,float current,PID_Struct *_pid)
{
		float err;
		float	different_err;
    float output;         
    
    err =target - current;														//误差
    _pid->Integral += err;														//积分误差
    different_err = err - _pid->Last_Error;						//微分误差
    
    if(_pid->Integral >= _pid->Limit_Integral)        //积分限幅
        _pid->Integral = _pid->Limit_Integral;
    if(_pid->Integral <= -_pid->Limit_Integral)
        _pid->Integral =-_pid->Limit_Integral;
    
    output = _pid->P * err + _pid->I * _pid->Integral + _pid->D * different_err;			//PID整合
    
    if(output >= _pid->Limit_Output)          			//输出限幅
        output = _pid->Limit_Output;
    if(output <= - _pid->Limit_Output)
        output = - _pid->Limit_Output;
    
		_pid->Last_Error = err;													//记录上次误差
		return output;
}


void Dace_Ctrl_Init(float dece,float acce,float start_output,Dace_t* _dace)
{
	_dace->acce=acce;
	_dace->dece=dece;
	_dace->last_value=start_output;
	_dace->current_value=start_output;
}

float Dace_Normal_Ctrl(float target,Dace_t* _dace)
{
	_dace->last_value=_dace->current_value;
	if (target > _dace->last_value)
	{
		_dace->current_value -= _dace->dece;
		if (_dace->current_value < target) _dace->current_value=target;
	}
	else if (target < _dace->last_value)
	{
		_dace->current_value += _dace->acce;
		if (_dace->current_value > target) _dace->current_value=target;
	}
	return _dace->current_value;
}


float current_output;
float PID_Dace_Ctrl(float target,float current,float dace_max,PID_Struct *_pid)
{
	current_output=PID_Normal_Ctrl(target,current,_pid);
	
	if (fabsf(current_output-_pid->Last_Output)>=dace_max)
	{
		if (current_output>=_pid->Last_Output) current_output=_pid->Last_Output+dace_max;
		else                                   current_output=_pid->Last_Output-dace_max;
	}
	
	_pid->Last_Output=current_output;
	return current_output;
}

void HAL_TIM_PeriodElapsedCallback_For_PID(TIM_HandleTypeDef *htim)
{
	uint8_t i;
	for(i=0;i<Registered_pid_count;i++)
	{
		if (htim==Registered_pid[i]->ticker_htim)
		{
			Registered_pid[i]->pid_output=PID_Normal_Ctrl(Registered_pid[i]->pid_target,Registered_pid[i]->pid_current,Registered_pid[i]->pid_param);
		}
	}
}