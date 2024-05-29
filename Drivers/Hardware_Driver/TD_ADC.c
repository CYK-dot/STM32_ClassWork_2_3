#include "TD_ADC.h"

extern ADC_HandleTypeDef hadc1;

void TD_ADC_Init(void)
{
	//HAL_ADCEx_Calibration_Start(&hadc1);
}

int16_t Get_TD_AIN10(void)
{
 HAL_ADC_Start(&hadc1);     
 HAL_ADC_PollForConversion(&hadc1, 100);  
 
 if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
 {
  return HAL_ADC_GetValue(&hadc1);  
 }
 else 
	return 0;
}

float Get_TD_AIN10_Percent(void)
{
	return (float)((float)Get_TD_AIN10()-2048.0f)/2048.0f;
}