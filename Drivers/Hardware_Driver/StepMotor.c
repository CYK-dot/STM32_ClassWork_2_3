/*---------头文件--------------------------------------*/
#include "StepMotor.h"

/*---------配置宏--------------------------------------*/
extern TIM_HandleTypeDef htim4;
#define SMOTOR_HTIM         htim4
#define SMOTOR_HTIM_TICK_US TIM4_TICK_US

/*---------全局变量------------------------------------*/
uint8_t  motor_phase;
int64_t motor_total_phase;
int32_t motor_total_cycle;
int8_t   motor_direction;

/*---------函数接口-------------------------------------*/
void StepMotor_Init(void)
{
	HAL_TIM_Base_Stop_IT(&SMOTOR_HTIM);
	motor_phase=0;
	HAL_GPIO_WritePin(DIO0_GPIO_Port,DIO0_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIO1_GPIO_Port,DIO1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIO2_GPIO_Port,DIO2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIO3_GPIO_Port,DIO3_Pin,GPIO_PIN_RESET);
	__HAL_TIM_SET_AUTORELOAD(&SMOTOR_HTIM,500);
}


void StepMotor_Action(uint8_t status)
{
	if (status)
	{
		HAL_TIM_Base_Start_IT(&SMOTOR_HTIM);
	}
	else
	{
		HAL_TIM_Base_Stop_IT(&SMOTOR_HTIM);
	}
}

uint32_t arr;
int8_t StepMotor_Set_Spd(float ms_per_phase)
{
	if (ms_per_phase>=0)motor_direction=1;
	else                motor_direction=-1;
	
	arr =motor_direction*ms_per_phase*1000/SMOTOR_HTIM_TICK_US; //ms*1000/100=ms*10
}

int8_t StepMotor_Set_Spd_Us(uint32_t us_per_phase)
{
	uint32_t arr;
	if (us_per_phase>=0)motor_direction=1;
	else                motor_direction=-1;
	
	arr =motor_direction*us_per_phase/SMOTOR_HTIM_TICK_US;
	
	if (arr>65535) 
	{
		arr=65535;
		__HAL_TIM_SET_AUTORELOAD(&SMOTOR_HTIM,65535);
		return 1;
	}
	else
	{
		__HAL_TIM_SET_AUTORELOAD(&SMOTOR_HTIM,arr);
		return 0;
	}
}

int64_t StepMotor_Get_Pos(void)
{
	return motor_total_phase;
}

void StepMotor_Set_Zero(void)
{
	motor_phase=0;
	motor_total_phase=0;
	motor_total_cycle=0;
}

void StepMotor_TIM_Callback(TIM_HandleTypeDef *htim)
{
	if (htim==&SMOTOR_HTIM)
	{
		switch(motor_phase)
		{
			case 0:
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO0_GPIO_Port,DIO0_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO1_GPIO_Port,DIO1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO2_GPIO_Port,DIO2_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO3_GPIO_Port,DIO3_Pin,GPIO_PIN_RESET);	
				
				if (motor_direction==1)motor_phase=1;
				else                   
				{
					motor_phase=7;
					motor_total_cycle--;
				}
				motor_total_phase=motor_total_cycle*8+motor_phase;
			break;
			
			case 1:
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO0_GPIO_Port,DIO0_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO1_GPIO_Port,DIO1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO2_GPIO_Port,DIO2_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO3_GPIO_Port,DIO3_Pin,GPIO_PIN_RESET);	
				
				motor_phase+=motor_direction;
				motor_total_phase=motor_total_cycle*8+motor_phase;
			break;
			
			case 2:
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO0_GPIO_Port,DIO0_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO1_GPIO_Port,DIO1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO2_GPIO_Port,DIO2_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO3_GPIO_Port,DIO3_Pin,GPIO_PIN_RESET);	
			
				motor_phase+=motor_direction;
				motor_total_phase=motor_total_cycle*8+motor_phase;
			break;
			
			case 3:
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO0_GPIO_Port,DIO0_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO1_GPIO_Port,DIO1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO2_GPIO_Port,DIO2_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO3_GPIO_Port,DIO3_Pin,GPIO_PIN_RESET);	
			
				motor_phase+=motor_direction;
				motor_total_phase=motor_total_cycle*8+motor_phase;
			break;
			
			case 4:
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO0_GPIO_Port,DIO0_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO1_GPIO_Port,DIO1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO2_GPIO_Port,DIO2_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO3_GPIO_Port,DIO3_Pin,GPIO_PIN_RESET);	
			
				motor_phase+=motor_direction;
				motor_total_phase=motor_total_cycle*8+motor_phase;
			break;
			
			case 5:
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO0_GPIO_Port,DIO0_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO1_GPIO_Port,DIO1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO2_GPIO_Port,DIO2_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO3_GPIO_Port,DIO3_Pin,GPIO_PIN_SET);	
			
				motor_phase+=motor_direction;
				motor_total_phase=motor_total_cycle*8+motor_phase;
			break;
			
			case 6:
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO0_GPIO_Port,DIO0_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO1_GPIO_Port,DIO1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO2_GPIO_Port,DIO2_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO3_GPIO_Port,DIO3_Pin,GPIO_PIN_SET);	
			
				motor_phase+=motor_direction;
				motor_total_phase=motor_total_cycle*8+motor_phase;
			break;
			
			case 7:
				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO0_GPIO_Port,DIO0_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIO1_GPIO_Port,DIO1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO2_GPIO_Port,DIO2_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIO3_GPIO_Port,DIO3_Pin,GPIO_PIN_SET);
			
				if (motor_direction==1)
				{
					motor_phase=0;
					motor_total_cycle++;
				}
				else  
				{
					motor_phase=6;
				}
				motor_total_phase=motor_total_cycle*8+motor_phase;
			break;
		}
		
		if (arr>65535) 
		{
			arr=500;
			__HAL_TIM_SET_AUTORELOAD(&SMOTOR_HTIM,500);
		}
		else if (arr>5)
		{
			__HAL_TIM_SET_AUTORELOAD(&SMOTOR_HTIM,arr);
		}
	}

}
