#include <SH79F3212.h>

#include "MCU.h"

/**
  * @Brief	SH79F3212芯片初始化
  * @Param  None
  * @Note   根据芯片手册设置各个端口
  * @RetVal	None
  */
void MCU_Init()
{
	// 配置寄存器时，首先要切换到寄存器所在页
	INSCON	= 0x00;	// 分页0
	
	EA 		= 0;	// 关闭全局中断
	
	CLKCON	= 0x00;	// 不分频，32.768khz作为OSCSLK
	
	RSTSTAT = 0x02;
	
	//	P0.0:SegA/LED5	P0.1:COM2	P0.2:COM3	P0.3:SegE
	P0CR	= 0xff;
	P0PCR	= 0x00;
	P0		= 0x00;
	P0_1	= 1;
	P0_2	= 1;
	
	//	P1.0:SegD/LED7	P1.1:SegDP/LED4	P1.2:COM0	P1.7:K1
	P1CR 	= 0xff;
	P1PCR 	= 0x00;
	P1 		= 0x00;
	P1_2 	= 1;
	
	DISPCON = 0x05;	// 共阴极，P1.0作为IO口
	P1OS 	= 0x02;	// 开漏输出，内部上拉电阻
	
	//	P2.0:K2			P2.1:SegC/LED6	P2.2:SegG/LED3
	//	P2.3:SegF/LED2	P2.4:SegB		P2.5:K3		P2.6:GVChk
	P2CR 	= 0x9f;
	P2PCR 	= 0x00;
	P2 		= 0x00;
	
	//	P3.3:NTC and Beep	P3.4:COM1	P3.6:Triac
	P3CR 	= 0xff;
	P3PCR 	= 0x00;
	P3 		= 0x00;
	P3_4	= 1;
	
	EA 		= 1;
}