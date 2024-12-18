#include <SH79F3212.h>

#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "Digital.h"
#include "NTC.h"
#include "Triac.h"
#include "Work.h"

bool Scroll_Flag = false;
unsigned char PowerOnCount1 = 0;	//	上电计数
unsigned char PowerOnCount2 = 0;	//	上电计数
//	系统状态:0-上电状态、1-待机状态、2-自检状态、3-报警状态、4-选择状态、5-工作状态
unsigned char Sys_Mode = 1;

bool NTCShort = false;
bool NTCOpen = false;
bool DryBoil = false;
bool ErrorFlag = true;	//	异常标志位

unsigned char FlashCount = 0;	//	数码管闪烁计数
unsigned char ErrorCount = 0;	//	错误计数
unsigned char PressCount = 0;	//	功能键按下次数

unsigned char ScrollPos = 0;		//	数码管轮播索引
unsigned char ScrollCounter = 0;	//	数码管轮播计数
unsigned int xdata ScrollData[11][3] = {	//	数码管轮播数组
	{10,0,0},{11,1,1},{12,2,2},{13,3,3},{14,4,4},{15,5,5},
	{16,6,6},{17,7,7},{18,8,8},{19,9,9},{22,0,0}
};

/**
  * @Brief	待机模式
  * @Param  None
  * @Note   None
  * @RetVal	None
  */
void StandbyMode()
{
	if(Sys_Mode == 1)
	{
		if(PowerOnCount1 <= 49) PowerOnCount1 ++;
		else
		{
			PowerOnCount1 = 50;
			DigitalSet(10,0,0);
			LedSet(0,0x00,0);
			BuzzerSet(0,0,0);
			TriacSet(0,0,0,0);
			//	松手触发
		}
	}
}

/**
  * @Brief	自检模式
  * @Param  None
  * @Note   None
  * @RetVal	None
  */
void CheckMode()
{
	if(Sys_Mode != 3)
	{
		PowerOnCount2 ++;
		if(PowerOnCount2 < 199)
		{
			if(KeyValue == 0x03)
			{
				Sys_Mode = 2;	//	自检状态
				if(KeyCount == 80 - 2)
				{
					BuzzerSet(2,100,100);
					TriacSet(4,6,0,10);
					DigitalSet(10,0,0);
					Scroll_Flag = true;	//	开启数码管轮播
				}
			}
		}
		else PowerOnCount2 = 200;	//	钳住，上电4秒内
	
		if(Scroll_Flag)
		{
			ScrollCounter ++;
			if(ScrollCounter > 49)
			{
				ScrollCounter = 0;
				ScrollPos ++;
				if(ScrollPos < 11)
				{
					DigitalSet(ScrollData[ScrollPos][0],
						ScrollData[ScrollPos][1],ScrollData[ScrollPos][2]);
				}
				else if(ScrollPos == 11)
				{
					DigitalSet(Temperature/10%10,Temperature%10+10,20);
				}
				else
				{
					ScrollPos = 0;
					Sys_Mode = 1;	//	切换待机状态
					Scroll_Flag = 0;	//	关闭轮播
				}
			}
		}
	}
}

/**
  * @Brief	报警状态
  * @Param  None
  * @Note   None
  * @RetVal	None
  */
void AlarmMode()
{
	FlashCount ++;
	if(FlashCount >= 50) FlashCount = 0;
	
	if(Buzzer_State != 1)
	{
		if(AD_Value < 80 || //	短路195℃
			(NTCShort && AD_Value < (DryBoil?100:709)))	//	恢复过程
		{
			ErrorCount ++;
			if(ErrorCount > 25)
			{
				ErrorCount = 0;
				NTCShort = true;
				Sys_Mode = 3;
				
				if(FlashCount < 25) DigitalSet(21,0,1);	//	E01
				else DigitalSet(23,23,23);
				if(ErrorFlag)	//	下面操作只执行一次
				{
					ErrorFlag = false;
					LedSet(0,0x00,0);
					BuzzerSet(3,300,300);
					TriacSet(0,0,0,0);
				}
			}
		}
		else if((Sys_Mode == 5 && AD_Value < 100 && AD_Value > 80) //	工作干烧185℃
			|| (DryBoil && AD_Value > 80 && AD_Value < 709))	//	恢复过程
		{
			ErrorCount ++;
			if(ErrorCount > 25)
			{
				ErrorCount = 0;
				DryBoil = true;
				Sys_Mode = 3;
				
				if(FlashCount < 25) DigitalSet(21,0,0);	//	E00
				else DigitalSet(23,23,23);
				if(ErrorFlag)
				{
					ErrorFlag = false;
					LedSet(0,0x00,0);
					BuzzerSet(3,300,300);
					TriacSet(0,0,0,0);
				}
			}
		}
		else if(AD_Value > 3965 || //开路-20℃
			(NTCOpen && AD_Value > 3530 && AD_Value < 3965))	//	恢复过程
		{
			ErrorCount ++;
			if(ErrorCount > 25)
			{
				ErrorCount = 0;
				NTCOpen = true;
				Sys_Mode = 3;
				
				if(FlashCount < 25) DigitalSet(10,0,0);	//	000
				else if(FlashCount >= 25) DigitalSet(23,23,23);
				if(ErrorFlag)
				{
					ErrorFlag = false;
					DigitalSet(10,0,0);
					LedSet(0,0x00,0);
					BuzzerSet(3,300,300);
					TriacSet(0,0,0,0);
				}
			}
		}
		else if(Sys_Mode == 3 && AD_Value > 709 && AD_Value < 3530)
		{
			NTCOpen = false;
			NTCShort = false;
			DryBoil = false;
			ErrorFlag = true;
			Sys_Mode = 1;	//	切换待机状态
		}
	}
}

void SelectMode()
{
	
}

void WorkMode()
{
	
}