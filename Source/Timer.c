#include <SH79F3212.h>

#include "Timer.h"

/**
  * @Brief	定时器0初始化
  * @Param  None
  * @Note   定时125us
  * @RetVal	None
  */
void Timer0_Init()
{
	TMOD |= 0x01;	// 16位向上计数定时器
	TCON1 = 0x04;	// 系统时钟源
	
	TL0	= 0xe3;	// 125 / (1 / 16.6M) = 2075
	TH0	= 0xf7;	// 65535 - 2075
	
	TF0	= 0;	// 清除溢出标志位
	
	TR0 = 1;	// 启动定时器
	
	ET0	= 1;	// 开定时器溢出中断
	
	EA = 1;		// 开全局中断
	
	IPL0	= 0x02;
	IPH0	= 0x02;	// 定时器0优先等级1，作为最高优先级
	IPL1	= 0x00;
	IPH1	= 0x00;
}