#include <SH79F3212.h>

#include "NTC.h"
#include "Buzzer.h"

unsigned char 	Buzzer_State = 0;			// 蜂鸣器分为三种状态:开始，蜂鸣，间隔
unsigned int 	Buzzer_IntervalCount = 0; 	// 用于蜂鸣器空隔时间计时
unsigned int 	Buzzer_DurationCount = 0; 	// 用于蜂鸣器持续时间计时
unsigned char 	Buzzer_Count = 0;			// 记录蜂鸣器响的次数

// 蜂鸣器预设值
unsigned char 	Buzzer_Times = 0;
unsigned int 	Buzzer_Interval = 0;
unsigned int 	Buzzer_Duration = 0;

/**
  * @Brief	蜂鸣函数
  * @Param  Buzzer_Times:蜂鸣次数
			Interval:间隔时间,单位ms
			Duration:蜂鸣时间,单位ms
  * @Note   时基:20ms
  * @RetVal	None
  */
void Buzzer(unsigned char beep_times,unsigned int interval,unsigned int duration)
{
	if(Buzzer_State == 0 && Buzzer_Count < beep_times) // 如果还没开始并且蜂鸣次数没到
	{
		Buzzer_State = 1;
	}
	else if(Buzzer_State == 1)	// 蜂鸣状态
	{	
		P3CR |= 0x08;
		Buzzer_DurationCount ++;
		if(Buzzer_DurationCount >= (duration / 20 - 1))	// 蜂鸣时间结束
		{
			Buzzer_DurationCount = 0;
			Buzzer_State = 2;
			P3CR &= 0xf7;
		}
	}
	else if(Buzzer_State == 2)	// 间隔状态
	{
		Buzzer_IntervalCount ++;
		if(Buzzer_IntervalCount >=  (interval / 20 - 1))	// 间隔时间结束
		{
			Buzzer_IntervalCount = 0;
			Buzzer_Count ++;	// 记录蜂鸣次数
			if(Buzzer_Count < beep_times)	Buzzer_State = 1;	//	蜂鸣次数没完毕
			else 
			{
				Buzzer_State = 0;	// 蜂鸣次数完毕
			}
		}
	}
}

/**
  * @Brief	蜂鸣器函数刷新参数
  * @Param  Buzzer_Times:蜂鸣次数
			interval:间隔时间
			duration:发声时间
  * @Note   None
  * @RetVal	None
  */
void BuzzerSet(unsigned char beep_times,unsigned int interval,unsigned int duration)
{
	Buzzer_Count = 0;
	Buzzer_State = 0;
	Buzzer_DurationCount = 0;
	Buzzer_IntervalCount = 0;
	
	Buzzer_Times = beep_times;
	Buzzer_Duration = duration;
	Buzzer_Interval = interval;
}
