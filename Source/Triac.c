#include <SH79F3212.h>

#include "Triac.h"

unsigned int Triac_Count = 0;
unsigned char Cycles = 0;
unsigned int Edge_Count = 0;

//	预设值
unsigned char On_Time = 0;
unsigned char Off_Time = 0;
unsigned char Skip_Cycles = 0;
unsigned char Total_Cycles = 0;

/**
  * @Brief	可控硅丢波输出
  * @Param  On_Time:可控硅开启时间
			Off_Time:可控硅关闭时间
			Skip_Cycles:Total_Cycles中跳过的周期
			Total_Cycles:分段的总周期
			Time:运行的时间
  * @Note  	None
  * @RetVal	None
  */
void Triac_SkipCycles(unsigned char On_Time,unsigned char Off_Time,
	unsigned char Skip_Cycles, unsigned char Total_Cycles)
{
	unsigned char Temp = On_Time + Off_Time;
	
	if(Cycles >= Total_Cycles * 2) Cycles = 0;
	else if(Cycles < Total_Cycles * 2)
	{
		Triac_Count ++;
		if(Triac_Count >= Temp * 8 - 1)
		{
			Triac_Count = 0;
			Cycles ++;	//	周期计数+1
		}
	
		if(Triac_Count < On_Time * 8 - 1)
		{
			if(Cycles < Total_Cycles * 2 - Skip_Cycles * 2) SCR = 1;
			else SCR = 0;
		}
		else if(Triac_Count < Temp * 8 - 1)	SCR = 0;
	}
}

/**
  * @Brief	可控硅输出控制函数
  * @Param  参数含义同上
  * @Note   None
  * @RetVal	None
  */
void TriacSet(unsigned char OnTime,unsigned char OffTime,
	unsigned char SkipCycles, unsigned char TotalCycles)
{
	Triac_Count = 0;
	Cycles = 0;
	Edge_Count = 0;
	
	On_Time = OnTime;
	Off_Time = OffTime;
	Skip_Cycles = SkipCycles;
	Total_Cycles = TotalCycles;
}