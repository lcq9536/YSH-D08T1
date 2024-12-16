#include <intrins.h>
#include <SH79F3212.h>

#include "Buzzer.h"
#include "Voltage.h"
#include "Triac.h"
#include "NTC.h"
#include "Key.h"
#include "LED.h"
#include "Timer.h"
#include "tk.h"
#include "MCU.h"
#include "Digital.h"

bool Flag = false;	//	LED和数码管分时共用

unsigned char Sys_Count = 0;	//	系统计时
unsigned char Sys_State = 0;	//	状态机选择位

//	系统状态:0-上电状态、1-待机状态、2-自检状态、3-报警状态、4-选择状态、5-工作状态
unsigned char Sys_Mode = 0;

unsigned int PowerOnCount = 0;	//	上电计数

void main()
{
	MCU_Init();
	Timer0_Init();
	InitKey();
	BuzzerSet(1,1000,300);
	LedSet(1,0x7f,1000);
	DigitalSet(23,23,23);
	while(1)
	{
		RSTSTAT = 0x02;
		
		if(Sys_Count >= 15)
		{
			Sys_Count = 0;
			
			Sys_State ++;
			if(Sys_State >= 10) Sys_State = 0;
			
			tkRound();
			
			switch(Sys_State)
			{
				case 1:
					ADCON = 0;	//	关闭ADC
					ADC2H = 0;	//	P3.3作为普通IO口
					Buzzer(Buzzer_Times,Buzzer_Interval,Buzzer_Duration);
				break;
				
				case 2:
					Get_KeyValue(KeyRead);	//	判断键值
					Key_Handle();	//	键值处理
				break;
				
				case 3:
					if(Buzzer_State != 1)
					{
						AD_Value = ADC_Read();
						AD_Deal(AD_Value);
						Temperature = AD_To_Temperature(AD_Value);
						Temperature_Deal(Temperature);
						//DigitalSet(Temperature/10%10,Temperature%10+10,20);
					}
				break;
					
				case 4:
					if(PowerOnCount > 7999 || Sys_Mode == 1)
					{
						DigitalSet(0+10,0,0);
						LedSet(0,0x00,0);
						BuzzerSet(0,0,0);
						TriacSet(0,0,0,0);
						if(KeyValue == 0x01)
						{
							Sys_Mode = 2;
						}
					}
				break;
					
				case 5:
//					if(PowerOnCount < 31999)
//					{
//						PowerOnCount = 32000;
//						if(KeyValue == 0x03)
//						{
//							if(KeyCount == 80 - 2)
//							{
//								BuzzerSet(2,100,100);
//							}
//						}
//					}
				break;
			}
		}
	}
}

/**
  * @Brief	中断服务函数
  * @Param  None
  * @Note   通过中断向量表进入
  * @RetVal	None
  */
void timer0() interrupt 1
{
	TL0		= 0xe3;
	TH0		= 0xf7;
	
	TF0 = 0;	// 清除中断标志位
	
	Sys_Count ++;
	
	PowerOnCount ++;
	
	// 蜂鸣部分
	if(Buzzer_State == 1)
	{
		BUZZER = !BUZZER;
	}
	
	// 可控硅部分
	Triac_SkipCycles(On_Time,Off_Time,Skip_Cycles,Total_Cycles);
	
	// 显示部分
	Flag = !Flag;
	if(!Flag)
	{
		Led_Display(LED_State,LED_Value,LED_Time);
	}
	else
	{
		Digital_Display(Digital_Num1,Digital_Num2,Digital_Num3);
	}
}

