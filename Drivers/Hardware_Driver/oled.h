//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : 
//  生成日期   : 2018-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(STM32F4系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PG7（SCL）
//              D1   接PG6（SDA）
//              RES  接PG5
//              DC   接PG4
//              CS   接PG3               
//              ----------------------------------------------------------------
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H			  	 	    
#include "stdlib.h"
#include "main.h"

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    

//-----------------测试LED端口定义---------------- 


//-----------------OLED端口定义----------------  					   


#define OLED_SCLK_Clr() HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_RESET)//GPIO_ResetBits(GPIOG,GPIO_Pin_7)//CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_SET)//GPIO_SetBits(GPIOG,GPIO_Pin_7)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(DIN_GPIO_Port,DIN_Pin,GPIO_PIN_RESET)//GPIO_ResetBits(GPIOG,GPIO_Pin_6)//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(DIN_GPIO_Port,DIN_Pin,GPIO_PIN_SET)//GPIO_SetBits(GPIOG,GPIO_Pin_6)

#define OLED_RST_Clr() HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,GPIO_PIN_SET)
 		     
#define OLED_CS_Clr()  HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET)//GPIO_ResetBits(GPIOG,GPIO_Pin_3)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET)//GPIO_SetBits(GPIOG,GPIO_Pin_3)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



