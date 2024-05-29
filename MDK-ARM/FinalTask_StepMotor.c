#include "FinalTask_StepMotor.h"

int64_t phase_count;
int8_t  task_state;
int8_t  task_next_state;
int8_t  task_ongoning_flag;
int64_t phase_target;
uint64_t time_used_tick;

PID_Struct StepMotor_Pid; 
float      debug_watch;

#define CYCLE_PHASE 97
#define MAX_SPEED   31
#define MIN_SPEED   2

void Final_Init_StepMotor(void)
{
	OLED_Init();
	TD_Key_Init();
	StepMotor_Init();
	StepMotor_Pid.P=0.5f;
	StepMotor_Pid.I=0.0f;
	StepMotor_Pid.D=0.0f;
	StepMotor_Pid.Limit_Output=MAX_SPEED;

}


//todo:each time,enter new case causes motor move
void Final_Main_StepMotor(void)
{
	
	if (phase_count<96) 
		StepMotor_Action(1);
	else                   
		StepMotor_Action(0);
	
	
	switch(task_state)
	{
		/*-------------------Wait---------------------------------------------*/
		case -1:
			StepMotor_Action(0);
			if (Count_Delay(1000,0)) task_state=task_next_state;
		break;
		/*--------------------Task 1-------------------------------------------*/
		case 0://forward 1 cycle
			if (TD_Get_Key1_Trigger()&&(task_ongoning_flag!=1)) 
			{
				phase_target+=CYCLE_PHASE;
				task_ongoning_flag=1;
			}
		
			if (task_ongoning_flag)
			{
				StepMotor_Set_Spd(10);
				StepMotor_Action(1);
				phase_count=StepMotor_Get_Pos();
				
				if (phase_count>=phase_target) 
				{
					StepMotor_Action(0);
					task_ongoning_flag=0;
				}
			}
			else
			{
				StepMotor_Action(0);
				phase_count=StepMotor_Get_Pos();
			}
			
			if ((TD_Get_Key0_Trigger())&&(!task_ongoning_flag)) 
			{
				StepMotor_Action(0);
				HAL_Delay(500);
				phase_target-=1;//change direction bias
				task_state=-1;
				task_next_state=1;			
			}
		break;
		/*--------------------Task 2-------------------------------------------*/
		case 1://backward 1 cycle
			if (TD_Get_Key1_Trigger()&&(task_ongoning_flag!=1)) 
			{
				phase_target-=CYCLE_PHASE;
				task_ongoning_flag=1;
			}
		
			if (task_ongoning_flag)
			{
				StepMotor_Set_Spd(-20);
				StepMotor_Action(1);
				phase_count=StepMotor_Get_Pos();
				
				if (phase_count<=phase_target) 
				{
					StepMotor_Action(0);
					task_ongoning_flag=0;
				}
			}
			else
			{
				StepMotor_Action(0);
				phase_count=StepMotor_Get_Pos();
			}
			
			if ((TD_Get_Key0_Trigger())&&(!task_ongoning_flag)) 
			{
				StepMotor_Action(0);
				HAL_Delay(500);
				phase_target-=1;//change direction bias
				task_state=-1;
				task_next_state=2;
			}
		break;
		/*--------------------Task 3-------------------------------------------*/
		case 2://forward 45 deg
			if (TD_Get_Key1_Trigger()&&(task_ongoning_flag!=1)) 
			{
				phase_target+=CYCLE_PHASE/8;
				task_ongoning_flag=1;
			}
		
			if (task_ongoning_flag)
			{
				StepMotor_Set_Spd(20);
				StepMotor_Action(1);
				phase_count=StepMotor_Get_Pos();
				
				if (phase_count>=phase_target) 
				{
					StepMotor_Action(0);
					task_ongoning_flag=0;
				}
			}
			else
			{
				StepMotor_Action(0);
				phase_count=StepMotor_Get_Pos();
			}
			
			if ((TD_Get_Key0_Trigger())&&(!task_ongoning_flag)) 
			{
				StepMotor_Action(0);
				HAL_Delay(500);
				phase_target-=1;//change direction bias
				task_state=-1;
				task_next_state=3;
			}
		break;
		/*--------------------Task 4-------------------------------------------*/
		case 3://backward 45 deg
			if (TD_Get_Key1_Trigger()&&(task_ongoning_flag!=1)) 
			{
				phase_target-=CYCLE_PHASE/8;
				task_ongoning_flag=1;
			}
		
			if (task_ongoning_flag)
			{
				StepMotor_Set_Spd(-20);
				StepMotor_Action(1);
				phase_count=StepMotor_Get_Pos();
				
				if (phase_count<=phase_target) 
				{
					StepMotor_Action(0);
					task_ongoning_flag=0;
				}
			}
			else
			{
				StepMotor_Action(0);
				phase_count=StepMotor_Get_Pos();
			}
			
			if ((TD_Get_Key0_Trigger())&&(!task_ongoning_flag)) 
			{
				StepMotor_Action(0);
				HAL_Delay(500);
				phase_target-=1;//change direction bias
				task_state=-1;
				task_next_state=4;
			}
		break;
		/*--------------------Task 5-------------------------------------------*/
		case 4://fast forward 10 cycles with dace
			if (TD_Get_Key1_Trigger()&&(task_ongoning_flag!=1)) 
			{
				phase_target+=960;
				task_ongoning_flag=1;
				Timetick_Start();
			}
		
			if (task_ongoning_flag)
			{
				debug_watch=MAX_SPEED+MIN_SPEED-PID_Dace_Ctrl(phase_target,phase_count,0.03f,&StepMotor_Pid);
				StepMotor_Set_Spd(debug_watch);
				StepMotor_Action(1);
				phase_count=StepMotor_Get_Pos();
				
				if (phase_count>=phase_target) 
				{
					StepMotor_Action(0);
					task_ongoning_flag=0;
					time_used_tick=Timetick_Get();
					Timetick_End();
				}
			}
			else
			{
				StepMotor_Action(0);
				phase_count=StepMotor_Get_Pos();
			}
			
			if ((TD_Get_Key0_Trigger())&&(!task_ongoning_flag)) 
			{
				StepMotor_Action(0);
				HAL_Delay(500);
				phase_target+=1;//change direction bias
				task_state=-1;
				task_next_state=5;
			}
		break;
		/*--------------------Task 6-------------------------------------------*/
		case 5://fast backward 10 cycles with dace
			if (TD_Get_Key1_Trigger()&&(task_ongoning_flag!=1)) 
			{
				phase_target-=960;
				task_ongoning_flag=1;
				Timetick_Start();
			}
		
			if (task_ongoning_flag)
			{
				debug_watch=-MAX_SPEED-MIN_SPEED-PID_Dace_Ctrl(phase_target,phase_count,0.03f,&StepMotor_Pid);
				StepMotor_Set_Spd(debug_watch);
				StepMotor_Action(1);
				phase_count=StepMotor_Get_Pos();
				
				if (phase_count<=phase_target) 
				{
					StepMotor_Action(0);
					task_ongoning_flag=0;
					time_used_tick=Timetick_Get();
					Timetick_End();
				}
			}
			else
			{
				StepMotor_Action(0);
				phase_count=StepMotor_Get_Pos();
			}
			
			if ((TD_Get_Key0_Trigger())&&(!task_ongoning_flag)) 
			{
				StepMotor_Action(0);
				HAL_Delay(500);
				phase_target+=1;//change direction bias
				task_state=-1;
				task_next_state=6;
			}
		break;
		/*--------------------Task 7-------------------------------------------*/
		case 6://fast forward 5 cycles with dace
			if (TD_Get_Key1_Trigger()&&(task_ongoning_flag!=1)) 
			{
				phase_target+=96*5;
				task_ongoning_flag=1;
				Timetick_Start();
			}
		
			if (task_ongoning_flag)
			{
				debug_watch=MAX_SPEED+MIN_SPEED-PID_Dace_Ctrl(phase_target,phase_count,0.03f,&StepMotor_Pid);
				StepMotor_Set_Spd(debug_watch);
				StepMotor_Action(1);
				phase_count=StepMotor_Get_Pos();
				
				if (phase_count>=phase_target) 
				{
					phase_target+=1;//change direction bias
					phase_target-=96*5;
					task_state=7;
				}
			}
			else
			{
				StepMotor_Action(0);
				phase_count=StepMotor_Get_Pos();
			}
		break;
		
		case 7://fast backward 5 cycles with dace		
			if (task_ongoning_flag)
			{
				debug_watch=-MAX_SPEED-MIN_SPEED-PID_Dace_Ctrl(phase_target,phase_count,0.03f,&StepMotor_Pid);
				StepMotor_Set_Spd(debug_watch);
				StepMotor_Action(1);
				phase_count=StepMotor_Get_Pos();
				
				if (phase_count<=phase_target) 
				{
					StepMotor_Action(0);
					task_ongoning_flag=0;
					time_used_tick=Timetick_Get();
					Timetick_End();
				}
			}
			else
			{
				StepMotor_Action(0);
				phase_count=StepMotor_Get_Pos();
			}
			
			if ((TD_Get_Key0_Trigger())&&(!task_ongoning_flag)) 
			{
				StepMotor_Action(0);
				HAL_Delay(500);
				phase_target+=1;//change direction bias
				task_state=-1;
				task_next_state=0;
			}
		break;
	}
}


char str[40];
void Final_Disp_StepMotor(void)
{
	OLED_Clear();
	sprintf(str,"%.2f",(float)time_used_tick/1000000);
	OLED_ShowString(1,1,str);
}