#include <SH79F3212.h>

#include "LED.h"
#include "Digital.h"

unsigned int 	LED_Count1 = 0;	// 闪烁计数
unsigned int 	LED_Count2 = 0;	// 闪烁计数
unsigned char 	LED_State = 0;
unsigned int	LED_Value = 0;
unsigned int 	LED_Time = 0;

void LED_Clear()
{
	COM0 = 1;
	COM1 = 1;
	COM2 = 1;
	COM3 = 1;
}

/**
  * @Brief	LED显示函数
  * @Param  State:三个显示状态：熄灭、闪烁、全亮
			Value:LED数组选择
			Time:闪烁时间
  * @Note   None
  * @RetVal	None
  */
void Led_Display(unsigned char State,unsigned int Value,unsigned int Time)
{
	LED_Clear();
	
	LED2 = Value & 0x01 ? 1 : 0;
	LED3 = Value & 0x02 ? 1 : 0;
	LED4 = Value & 0x04 ? 1 : 0;
	LED5 = Value & 0x08 ? 1 : 0;
	LED6 = Value & 0x10 ? 1 : 0;
	LED7 = Value & 0x20 ? 1 : 0;
	
	if(!State)
	{
		COM0 = 1;
	}
	else if(State == 1)
	{
		LED_Count2 ++;
		if(LED_Count2 < Time * 8 - 1)
		{
			LED_Count1 ++;
			if(LED_Count1 >= 7999) LED_Count1 = 0;
			
			if(LED_Count1 < 3999) COM0 = 0;
			else if(LED_Count1 >= 3999) COM0 = 1;
		}
		else
		{
			LED_Count2 = Time * 8 - 1;
			COM0 = 1;
		}
	}
	else if(State == 2)
	{
		LED_Count2 ++;
		if(Time == 0xff) COM0 = 0;
		else if(LED_Count2 < Time * 8 - 1)
		{
			COM0 = 0;
		}
		else
		{
			LED_Count2 = Time * 8 - 1;
			COM0 = 1;
		}
	}
}

/**
  * @Brief	LED显示设置函数
  * @Param  State:三个显示状态：熄灭、闪烁、全亮
			Value:LED数组选择
			Time:闪烁时间
  * @Note   None
  * @RetVal	None
  */
void LedSet(unsigned char State,unsigned int Value,unsigned int Time)
{
	LED_Count1 = 0;
	
	LED_State = State;
	LED_Value = Value;
	LED_Time = Time;
}