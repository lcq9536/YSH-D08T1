#include <SH79F3212.h>

#include "Key.h"
#include "Buzzer.h"
#include "tk.h"

unsigned char Delay_Cnt = 0;		//	消抖计时
unsigned int KeyValue = 0;			//	键值
unsigned char KeyCount = 0;		//	按键按下时间计数
unsigned char LongKey_Flag = 0;	//	长按短按标志位
unsigned int Last_KeyValue = 0;	//	一个扫描周期内的旧键值
unsigned int Last_KeyCode = 0;		//	一个完整按键动作的旧键值

/**
  * @Brief	判断键值
  * @Param  Key_Read:检测到的键值
  * @Note   None
  * @RetVal	None
  */
void Get_KeyValue(unsigned int Key_Read)
{
	KeyValue = 0;
	
	if(Key_Read & 0x01)	KeyValue |= 0x01;	//	K1
	if(Key_Read & 0x02)	KeyValue |= 0x02;	//	K2
	if(Key_Read & 0x04)	KeyValue |= 0x04;	//	K3
}

/**
  * @Brief	键值处理
  * @Param  None
  * @Note   None
  * @RetVal	None
  */
void Key_Handle()
{
	if(KeyValue && !Last_KeyValue)	//	扫描到按键按下并且是新按键
	{
		KeyCount = 0;
		Last_KeyValue = KeyValue;
	}
	else if(KeyValue == Last_KeyValue)	//	扫描到按键继续按下
	{
		if(KeyCount < 254) KeyCount ++;	//	20ms+1
	}
	else if(!KeyValue)	//	扫描到按键松开
	{
		KeyCount = 0;
		if(Last_KeyValue)//	上一次有键值就判断为第一次松开
		{
			Last_KeyCode = Last_KeyValue;	//	保存上次松开时键值
			Last_KeyValue = 0;
		}
	}
}