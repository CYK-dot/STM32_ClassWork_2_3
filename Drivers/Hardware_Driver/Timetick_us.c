#include "Timetick_us.h"

volatile int tick_total;
volatile int tick_overflow;

void Timetick_Reset(void)
{
	tick_total=0;
	tick_overflow=0;
	__HAL_TIM_SET_COUNTER(&TD_TICK_TIMER,0);
}

void Timetick_Start(void)
{
	Timetick_Reset();
	HAL_TIM_Base_Start_IT(&TD_TICK_TIMER);
}
void Timetick_End(void)
{
	HAL_TIM_Base_Stop_IT(&TD_TICK_TIMER);
	Timetick_Reset();
}

void Timetick_Callback(TIM_HandleTypeDef *htim)
{
	if(htim == &TD_TICK_TIMER)
	{
		tick_total=__HAL_TIM_GET_COUNTER(&TD_TICK_TIMER)+tick_overflow*65536;
		tick_overflow++;
	}
}

uint64_t Timetick_Get(void)
{
	tick_total=__HAL_TIM_GET_COUNTER(&TD_TICK_TIMER)+tick_overflow*65536;
	return tick_total;
}

/**
	@name: Count_Delay
	@brief:计数延时
	@param:int count_num            			计数数
	@return	1延时完成
**/
int Count_Delay(int count_num,int num)
{
	static int count[20] = {0};
	if (num >= 20) return 1;
	
	count[num] ++;
	if(count[num] >=count_num)
	{
		count[num] = 0;
		return 1;
	}
	else
		return 0;
}
