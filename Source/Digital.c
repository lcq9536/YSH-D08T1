#include <SH79F3212.h>
#include <math.h>

#include "LED.h"
#include "Digital.h"

unsigned char Digital_Num = 0;
unsigned char Digital_Num1 = 0;
unsigned char Digital_Num2 = 0;
unsigned char Digital_Num3 = 0;

// 数码管显示数组
unsigned char code Digital_Table[] = {
	0x3f,0x06,0x5b,0x4f,0x66,	// 0~4
	0x6d,0x7d,0x07,0x7f,0x6f,	// 5~9
	0xbf,0x86,0xdb,0xcf,0xe6,	// 0~4,带DP
	0xed,0xfd,0x87,0xff,0xef,	// 5~9,带DP
	0x39,0x3e,0x00,0xff			// C,U,熄灭,全亮
}; 

void Digital_Clear()
{
	COM0 = 1;
	COM1 = 1;
	COM2 = 1;
	COM3 = 1;
}   
/**
  * @Brief	数码管静态显示函数
  * @Param  Num1、Num2、Num3显示数据
  * @Note   None
  * @RetVal	None
  */
void Digital_Display(unsigned char Num1,unsigned char Num2,unsigned char Num3)
{
	Digital_Clear();
	
	Digital_Num ++;
	if(Digital_Num >= 4) Digital_Num = 0;

	if(Digital_Num == 1)
	{
		SEGA 	= Digital_Table[Num1] & 0x01 ? 1 : 0;
		SEGB 	= Digital_Table[Num1] & 0x02 ? 1 : 0;
		SEGC 	= Digital_Table[Num1] & 0x04 ? 1 : 0;
		SEGD 	= Digital_Table[Num1] & 0x08 ? 1 : 0;
		SEGE 	= Digital_Table[Num1] & 0x10 ? 1 : 0;
		SEGF 	= Digital_Table[Num1] & 0x20 ? 1 : 0;
		SEGG 	= Digital_Table[Num1] & 0x40 ? 1 : 0;
		SEGDP 	= Digital_Table[Num1] & 0x80 ? 1 : 0;
		
		COM1 = 0;
		COM2 = 1;
		COM3 = 1;
	}
	else if(Digital_Num == 2)
	{
		SEGA 	= Digital_Table[Num2] & 0x01 ? 1 : 0;
		SEGB 	= Digital_Table[Num2] & 0x02 ? 1 : 0;
		SEGC 	= Digital_Table[Num2] & 0x04 ? 1 : 0;
		SEGD 	= Digital_Table[Num2] & 0x08 ? 1 : 0;
		SEGE 	= Digital_Table[Num2] & 0x10 ? 1 : 0;
		SEGF 	= Digital_Table[Num2] & 0x20 ? 1 : 0;
		SEGG 	= Digital_Table[Num2] & 0x40 ? 1 : 0;
		SEGDP 	= Digital_Table[Num2] & 0x80 ? 1 : 0;
		
		COM1 = 1;
		COM2 = 0;
		COM3 = 1;
	}
	else if(Digital_Num == 3)
	{
		SEGA 	= Digital_Table[Num3] & 0x01 ? 1 : 0;
		SEGB 	= Digital_Table[Num3] & 0x02 ? 1 : 0;
		SEGC 	= Digital_Table[Num3] & 0x04 ? 1 : 0;
		SEGD 	= Digital_Table[Num3] & 0x08 ? 1 : 0;
		SEGE 	= Digital_Table[Num3] & 0x10 ? 1 : 0;
		SEGF 	= Digital_Table[Num3] & 0x20 ? 1 : 0;
		SEGG 	= Digital_Table[Num3] & 0x40 ? 1 : 0;
		SEGDP 	= Digital_Table[Num3] & 0x80 ? 1 : 0;
		
		COM1 = 1;
		COM2 = 1;
		COM3 = 0;
	}
}

/**
  * @Brief	数码管设置函数
  * @Param  Num1、Num2、Num3:显示数字
  * @Note   None
  * @RetVal	None
  */
void DigitalSet(unsigned char Num1,unsigned char Num2,unsigned char Num3)
{
	Digital_Num1 = Num1;
	Digital_Num2 = Num2;
	Digital_Num3 = Num3;
}
