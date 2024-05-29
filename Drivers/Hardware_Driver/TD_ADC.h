#ifndef TD_ADC_H_
#define TD_ADC_H_

#include "main.h"
#include "stm32f4xx_hal_adc_ex.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_ll_adc.h"

int16_t Get_TD_AIN10(void);
float Get_TD_AIN10_Percent(void);
void TD_ADC_Init(void);
#endif