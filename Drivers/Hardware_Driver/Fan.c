/*---------ͷ�ļ�--------------------------------------*/
#include "Fan.h"

/*---------���ú�--------------------------------------*/
extern TIM_HandleTypeDef htim3;
#define FAN_HTIM htim3
#define FAN_CHL  TIM_CHANNEL_3

/*---------ȫ�ֱ���------------------------------------*/
uint32_t last_tick,present_tick;
uint32_t spd;

/*---------�����ӿ�-------------------------------------*/
/**
 @name:Fan_Start
 @brief:��������
**/
void Fan_Start(void)
{
	HAL_TIM_PWM_Start(&FAN_HTIM,TIM_CHANNEL_3);
}
/**
 @name:Fan_Output
 @brief:���ȵ���
 @param:float duty ռ�ձ�
**/
void Fan_Output(float duty)
{
		__HAL_TIM_SET_COMPARE(&FAN_HTIM,FAN_CHL,(uint16_t)(1.0f*(FAN_HTIM.Instance->ARR+1.0f)*duty));
}
/**
 @name:Fan_Get_Freq
 @brief:���Ȼ�ȡת��
**/
uint32_t Fan_Get_Freq(void)
{
	return spd;
}

/**
 @name:Fan_Get_Rpm
 @brief:���Ȼ�ȡת��
**/
float Fan_Get_Rpm(void)
{
	return 1000.0f/((float)spd);
}
/**
 @name:HAL_GPIO_EXTI_Callback
 @brief:�ⲿ�жϻص�����
**/
void Fan_Exti_Callback(void)
{
	present_tick=HAL_GetTick();
	spd=present_tick-last_tick;
	last_tick=present_tick;
}
