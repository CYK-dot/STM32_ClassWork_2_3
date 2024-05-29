/* =========头文件==============================================*/
#include "FinalTask.h"

/* =========全局变量============================================*/

//状态机
int8_t    FinalTask_Status=-1;

//风扇
float      Fan_Test;
float      Fan_Output_duty;
float      Fan_Output_Rpm;
float      Fan_Input_Rpm;
PID_Struct Fan_Ctrl_Pid;
float      Fan_Ctrl_Time;
float      Fan_Ctrl_Overload;
uint8_t    Fan_Ctrl_Target;
#define    FAN_MAX_SPD 65.0f

Wave_Handler_t Fan_Wave_disp;
Wave_Handler_t Fan_Wave_param;
Wave_Param_t   Fan_Wave_anylsis;
int16_t        Fan_Wave_disp_arr[2];
uint8_t        Fan_Wave_Ctrl_Direction;

//全局驱动
float    Adc_value;
uint32_t Time_tick;

float test_ffff=0.3;
uint32_t real_tick_ctrl;
int finish_flag;

/* =========函数接口============================================*/
/**
 *@name:FinalTask_Init
 *@brief:完整初始化
**/
void FinalTask_Init(void)
{
    //-------硬件层初始化---------------------
	TD_Key_Init();
	TD_ADC_Init();
	Fan_Start();
	OLED_Init();

    //-------软件层初始化---------------------
	Fan_Ctrl_Pid.P = 0.1f;
	Fan_Ctrl_Pid.I = 0.4f;
	Fan_Ctrl_Pid.D = 0.0f;
	Fan_Ctrl_Pid.Limit_Output  = 1.0f;
	Fan_Ctrl_Pid.Integral      = 0.0f;
	Fan_Ctrl_Pid.Limit_Integral= 0.5f;
	Fan_Ctrl_Pid.Last_Error    = 0.0f;

	Fan_Wave_disp.start_pixel_x=0;
	Fan_Wave_disp.end_pixel_x  =125;
	Fan_Wave_disp.start_pixel_y=0;
	Fan_Wave_disp.end_pixel_y  =50;
	Fan_Wave_disp.wave_center_val=FAN_MAX_SPD/2.0f;
	Fan_Wave_disp.wave_max_val   =FAN_MAX_SPD;
	Fan_Wave_disp.wave_chl_len   =1;

	Fan_Wave_param.start_pixel_x=0;
	Fan_Wave_param.end_pixel_x  =125;
	Fan_Wave_param.start_pixel_y=0;
	Fan_Wave_param.end_pixel_y  =50;
	Fan_Wave_param.wave_center_val=FAN_MAX_SPD/2.0f;
	Fan_Wave_param.wave_max_val   =FAN_MAX_SPD;
	Fan_Wave_param.wave_chl_len   =1;
}

/**
 *@name:FinalTask_Ctrl
 *@brief:控制任务
 *@usage:按下按键,进入测量模式,此时会停车,继续按键才会开始测量
**/
void FinalTask_Ctrl(void)
{
	switch(FinalTask_Status)
	{
		//============================任务3:MAX Speed====================================================
		case -1:
			Fan_Input_Rpm  =Fan_Get_Rpm();
			Fan_Output(1);
			Fan_Wave_disp_arr[0]=(int)Fan_Input_Rpm;
			Fan_Wave_disp_arr[1]=(int)(Fan_Output_Rpm);
			Wave_Add(&Fan_Wave_param,Fan_Wave_disp_arr);
			Wave_Process(&Fan_Wave_param,&Fan_Wave_anylsis);
		
			if (TD_Get_Key1_Count()) FinalTask_Status=0;
		break;
		//============================任务4:电位器控速====================================================
		case 0:
			Fan_Input_Rpm  =Fan_Get_Rpm();
			Adc_value      =Get_TD_AIN10_Percent();
			Fan_Output_Rpm =(1.0f+Adc_value)/2.0f*FAN_MAX_SPD;
			Fan_Output_duty=PID_Normal_Ctrl(Fan_Output_Rpm,Fan_Input_Rpm,&Fan_Ctrl_Pid);
			if (Fan_Output_Rpm>=3) Fan_Output(Fan_Output_duty);
			else                   Fan_Output(0);

		
			Fan_Wave_disp_arr[0]=(int)Fan_Input_Rpm;
			Fan_Wave_disp_arr[1]=(int)(Fan_Output_Rpm);

			Wave_Add(&Fan_Wave_param,Fan_Wave_disp_arr);
			Wave_Process(&Fan_Wave_param,&Fan_Wave_anylsis);

			if (TD_Get_Key0_Count()) FinalTask_Status=1;
		break;

		//=========================按键被按下,等待风扇停机================================================
		case 1:
			Fan_Output(0.0f);
			Fan_Input_Rpm =Fan_Get_Rpm();
		
			Fan_Wave_disp_arr[0]=(int)Fan_Input_Rpm;
			Fan_Wave_disp_arr[1]=(int)(Fan_Output_Rpm);

			Wave_Add(&Fan_Wave_param,Fan_Wave_disp_arr);
			Wave_Process(&Fan_Wave_param,&Fan_Wave_anylsis);
		
			if (Count_Delay(5000,0)) 
			{
				FinalTask_Status=2;
				
				Fan_Ctrl_Target=20;
				Fan_Wave_Ctrl_Direction=1;
				Timetick_Start();
			}
		break;

		//=============================任务5:获取性能参数=================================================
		case 2:
			//状态函数：PID控制
			Fan_Input_Rpm=Fan_Get_Rpm();
			Fan_Output_Rpm=Fan_Ctrl_Target;
			Fan_Output_duty=PID_Normal_Ctrl(Fan_Output_Rpm,Fan_Input_Rpm,&Fan_Ctrl_Pid);
			if (Fan_Output_Rpm>=3) Fan_Output(Fan_Output_duty);
			else                   Fan_Output(1);

			Fan_Wave_disp_arr[0]=(int)Fan_Input_Rpm;
			Fan_Wave_disp_arr[1]=(int)(Fan_Output_Rpm);
			Wave_Add(&Fan_Wave_param,Fan_Wave_disp_arr);
			Wave_Process(&Fan_Wave_param,&Fan_Wave_anylsis);

			//转移条件：误差的均值达到3以内
			if (fabsf(Fan_Wave_anylsis.mean[0] - Fan_Ctrl_Target)<=3)
			{
				finish_flag++;
			}
			else
			{
				finish_flag--;
				if (finish_flag<0) finish_flag=0;
			}
			
			if (finish_flag>225)
			{
				
				if (Fan_Wave_Ctrl_Direction==1)
					Fan_Ctrl_Overload=((float)Fan_Wave_anylsis.max[0]-(float)Fan_Ctrl_Target)/(float)Fan_Ctrl_Target;//up
				else if (Fan_Wave_Ctrl_Direction==2)
					Fan_Ctrl_Overload=((float)Fan_Ctrl_Target-(float)Fan_Wave_anylsis.min[0])/(float)Fan_Ctrl_Target;//down
				
				
				real_tick_ctrl=Timetick_Get();
				Fan_Ctrl_Time=((double)real_tick_ctrl)/1000.0;
				
				finish_flag=0;
				FinalTask_Status=3;
			}
		break;
		//=============================任务5:保持恒定速度运行=================================================
		case 3:
			//状态函数：PID控制
			Fan_Input_Rpm=Fan_Get_Rpm();
			Fan_Output_Rpm=Fan_Ctrl_Target;
			Fan_Output_duty=PID_Normal_Ctrl(Fan_Output_Rpm,Fan_Input_Rpm,&Fan_Ctrl_Pid);
			if (Fan_Output_Rpm>=3) Fan_Output(Fan_Output_duty);
			else                   Fan_Output(1);

			Fan_Wave_disp_arr[0]=(int)Fan_Input_Rpm;
			Fan_Wave_disp_arr[1]=(int)(Fan_Output_Rpm);
			Wave_Add(&Fan_Wave_param,Fan_Wave_disp_arr);
			Wave_Process(&Fan_Wave_param,&Fan_Wave_anylsis);

			//转移条件：按下开关，开始下一次测量
			if (TD_Get_Key0_Trigger()) 
			{
				Fan_Wave_Ctrl_Direction=1;
				if (Fan_Ctrl_Target>=FAN_MAX_SPD-20) 
					Fan_Ctrl_Target=FAN_MAX_SPD;
				else
					Fan_Ctrl_Target+=20;
				
				FinalTask_Status=2;
				Timetick_End();
				Timetick_Start();
			}
			if (TD_Get_Key1_Trigger()) 
			{
				Fan_Wave_Ctrl_Direction=2;
				if (Fan_Ctrl_Target<=20)  
					Fan_Ctrl_Target=10;
				else         
					Fan_Ctrl_Target-=20;
				
				FinalTask_Status=2;
				Timetick_End();
				Timetick_Start();
			}
		break;
	}

}


/**
 *@name:FinalTask_Log
 *@brief:显示任务
**/
void FinalTask_Log(void)
{
	char log[40];

	switch(FinalTask_Status)
	{
		//============================任务4显示====================================================
		case -1:
		case 0:
		case 1:
			sprintf(log,"now=%d tar=%d",(int)Fan_Input_Rpm,(int)(Fan_Output_Rpm));

			Fan_Wave_disp_arr[0]=(int)Fan_Input_Rpm;
			Fan_Wave_disp_arr[1]=(int)(Fan_Output_Rpm);
			Wave_Add(&Fan_Wave_disp,Fan_Wave_disp_arr);

			OLED_Clear();
			OLED_ShowString(0,0,log);
			Wave_Display(&Fan_Wave_disp);
		break;

		//=============================任务5显示===================================================
		case 2:
		case 3:
		default:
			sprintf(log,"t=%.1f ol=%.2f",Fan_Ctrl_Time,Fan_Ctrl_Overload*10.0f);

			Fan_Wave_disp_arr[0]=(int)Fan_Wave_anylsis.mean[0];//Fan_Input_Rpm;
			Fan_Wave_disp_arr[1]=(int)(100.0f*Fan_Output_duty);
			Wave_Add(&Fan_Wave_disp,Fan_Wave_disp_arr);

			OLED_Clear();
			OLED_ShowString(0,0,log);
			Wave_Display(&Fan_Wave_disp);

		break;
	}

}


/* =========回调接口============================================*/

/**
 *@name:HAL_GPIO_EXTI_Callback
 *@brief:外部中断回调
**/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	if (GPIO_Pin == EXTI3_Pin)
	{
	  //风扇霍尔
		Fan_Exti_Callback();
	}
	else if (GPIO_Pin == KEY0_Pin)
	{
		//KEY0
	}
	else if (GPIO_Pin == KEY1_Pin)
	{
		//KEY1
	}
}

/**
 *@name:FinalTask_TIM_PeriodElapsedCallback
 *@brief:定时器回调
**/
void FinalTask_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == (&TD_KEY_TIMER))
	{
		TD_Key_Callback();
	}

	Timetick_Callback(htim);
	StepMotor_TIM_Callback(htim);
}
