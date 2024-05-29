#include "TD_Key.h"

//count:current press count
//flag:current voltage
//trigger:last key state

uint32_t key0_count,key0_flag,key0_trigger;
uint32_t key1_count,key1_flag,key1_trigger;

void TD_Key_Init(void)
{
	HAL_TIM_Base_Start_IT(&TD_KEY_TIMER);
}

void TD_Key_Callback(void)
{	
	//KEY 0------------------------------------------------------------
	if (HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)==GPIO_PIN_RESET)
	{
		if (key0_flag == 0) 
		{
			key0_count++;
			key0_trigger=1;
		}
		key0_flag=1;
	}
	else
	{
		if (key0_flag == 1) 
		{
			key0_trigger=0;
		}
		key0_flag=0;
	}
	//KEY 1------------------------------------------------------------
	if (HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==GPIO_PIN_RESET)
	{
		if (key1_flag == 0) 
		{
			key1_count++;
			key1_trigger=1;
		}
		key1_flag=1;
	}
	else
	{
		if (key1_flag == 1) 
		{
			key1_trigger=0;
		}
		key1_flag=0;
	}
}

uint32_t TD_Get_Key0_Count(void)
{
	return key0_count;
}
uint8_t TD_Get_Key0_Flag(void)
{
	return key0_flag;
}
uint32_t TD_Get_Key1_Count(void)
{
	return key1_count;
}
uint8_t TD_Get_Key1_Flag(void)
{
	return key1_flag;
}
uint8_t TD_Get_Key0_Trigger(void)
{
	return key0_trigger;
}
uint8_t TD_Get_Key1_Trigger(void)
{
	return key1_trigger;
}






