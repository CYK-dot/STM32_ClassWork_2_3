#ifndef OLED_WAVE_H_
#define OLED_WAVE_H_

#include "main.h"

#define MAX_WAVE_LEN 128
#define MAX_CHL_LEN  4

typedef struct oled_wave{
    uint8_t start_pixel_x;
    uint8_t start_pixel_y;
    uint8_t end_pixel_x;
    uint8_t end_pixel_y;

    int16_t wave_data[MAX_WAVE_LEN][MAX_CHL_LEN];
    int16_t wave_center_val;
    int16_t wave_max_val;
		uint8_t wave_chl_len;
}Wave_Handler_t;

typedef struct wave_param{
		float var[MAX_CHL_LEN];
		float mean[MAX_CHL_LEN];
		float max[MAX_CHL_LEN];
		float min[MAX_CHL_LEN];
}Wave_Param_t;

void Wave_Add(Wave_Handler_t* w,int16_t* arr);
void Wave_Display(Wave_Handler_t* w);
void Wave_Process(Wave_Handler_t* w,Wave_Param_t* p);
#endif