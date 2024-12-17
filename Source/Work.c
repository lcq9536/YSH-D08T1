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
	PowerOnCount1 ++;
	if(PowerOnCount1 > 49 && Sys_Mode == 1)
	{
		PowerOnCount1 = 50;
		DigitalSet(10,0,0);
		LedSet(0,0x00,0);
		BuzzerSet(0,0,0);
		TriacSet(0,0,0,0);
		if(KeyValue == 0x01)
		{
			Sys_Mode = 2;
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