#include "Oled_Wave.h"

/**
* @name:Wave_Add
* @brief:��һ��չʾ�����ʾ����
* @param:Wave_Handler_t* ʾ�������
* @param:int16_t* arr չʾ��������
**/
void Wave_Add(Wave_Handler_t* w,int16_t* arr)
{
    uint8_t channel;
    uint8_t pixel;

    for(channel=0;channel<w->wave_chl_len;channel++)
    {
        //����������ƽ��һ��
        for(pixel=0;pixel<(w->end_pixel_x-w->start_pixel_x);pixel++)
        {
            w->wave_data[pixel][channel]=w->wave_data[pixel+1][channel];
        }
        //���Ҳ�����������
        w->wave_data[pixel][channel]=arr[channel];
    }
}

/**
* @name:Wave_Add
* @brief:��һ��չʾ�����ʾ����
* @param:int16_t* arr չʾ��������
* @param:uint8_t len  ͨ������
**/
void Wave_Display(Wave_Handler_t* w)
{
    uint8_t channel;
    uint8_t pixel;
		float   disp_y;

    for(channel=0;channel<w->wave_chl_len;channel++)
    {
        for(pixel=0;pixel<=(w->end_pixel_x-w->start_pixel_x);pixel++)
        {
						disp_y =((float)(w->wave_data[pixel][channel] - w->wave_center_val)) / ((float)(w->wave_max_val - w->wave_center_val));
						disp_y =0.5f*(w->end_pixel_y + w->start_pixel_y) + disp_y*(0.5f*(w->end_pixel_y - w->start_pixel_y));

						OLED_DrawPoint(pixel+ w->start_pixel_x , (uint8_t)disp_y, 1);
        }
    }
}

/**
* @name:Wave_Process
* @brief:����ʾ��������,�����������ͳ����
* @param:Wave_Handler_t* w ʾ�������
* @param:Wave_Param_t*   p ͳ�������
**/
void Wave_Process(Wave_Handler_t* w,Wave_Param_t* p)
{
	uint8_t i,j;
	float t_mean,t_min,t_max,t_var;

	for(j=0;j<MAX_CHL_LEN ;j++)
	{
		t_mean=0;
		t_var=0;
		t_min=w->wave_data[0][j];
		t_max=t_min;
		
		for(i=w->start_pixel_x;i<w->end_pixel_x;i++)
		{
			t_mean+=w->wave_data[i][j];
			if (w->wave_data[i][j]>t_max) t_max=w->wave_data[i][j];
			if (w->wave_data[i][j]<t_min) t_min=w->wave_data[i][j];
		}
		
		for(i=w->start_pixel_x;i<w->end_pixel_x;i++)
		{
			t_var+=sqrt((w->wave_data[i][j]-t_mean)*(w->wave_data[i][j]-t_mean))/(w->end_pixel_x-w->start_pixel_x);
		}
		
		p->mean[j]=t_mean/(w->end_pixel_x-w->start_pixel_x);
		p->max[j]=t_max;
		p->min[j]=t_min;
		p->var[j]=t_var;
	}
}

