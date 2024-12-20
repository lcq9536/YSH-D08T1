#include <SH79F3212.h>

#include "LED.h"
#include "Digital.h"

unsigned int 	LED_Count = 0;	// 闪烁计数
unsigned char 	LED_State = 0;
unsigned int	LED_Value = 0;

/**
  * @Brief	LED显示函数
  * @Param  State:三个显示状态：熄灭、闪烁、全亮
			Value:LED数组选择
			Time:闪烁时间
  * @Note   和数码管分时，需要/2
  * @RetVal	None
  */
void Led_Display(unsigned char State,unsigned int Value)
{
	COM0 = 1;
	COM1 = 1;
	COM2 = 1;
	COM3 = 1;
	
	LED2 = Value & 0x01 ? 1 : 0;
	LED3 = Value & 0x02 ? 1 : 0;
	LED4 = Value & 0x04 ? 1 : 0;
	LED5 = Value & 0x08 ? 1 : 0;
	LED6 = Value & 0x10 ? 1 : 0;
	LED7 = Value & 0x20 ? 1 : 0;
	
	if(!State)	COM0 = 1;
	else if(State == 1)
	{
		LED_Count ++;
		if(LED_Count >= 4000) LED_Count = 0;
			
		if(LED_Count <= 2000) COM0 = 0;
		else if(LED_Count > 2000) COM0 = 1;
	}
	else if(State == 2)	COM0 = 0;
}

/**
  * @Brief	LED显示设置函数
  * @Param  State:三个显示状态：熄灭、闪烁、全亮
			Value:LED数组选择
			Time:闪烁时间
  * @Note   None
  * @RetVal	None
  */
void LedSet(unsigned char State,unsigned int Value)
{
	LED_Count = 0;
	
	LED_State = State;
	LED_Value = Value;
}