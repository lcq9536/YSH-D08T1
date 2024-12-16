#include <SH79F3212.h>

#include "Timer.h"

/**
  * @Brief	��ʱ��0��ʼ��
  * @Param  None
  * @Note   ��ʱ125us
  * @RetVal	None
  */
void Timer0_Init()
{
	TMOD |= 0x01;	// 16λ���ϼ�����ʱ��
	TCON1 = 0x04;	// ϵͳʱ��Դ
	
	TL0	= 0xe3;	// 125 / (1 / 16.6M) = 2075
	TH0	= 0xf7;	// 65535 - 2075
	
	TF0	= 0;	// ��������־λ
	
	TR0 = 1;	// ������ʱ��
	
	ET0	= 1;	// ����ʱ������ж�
	
	EA = 1;		// ��ȫ���ж�
	
	IPL0	= 0x02;
	IPH0	= 0x02;	// ��ʱ��0���ȵȼ�1����Ϊ������ȼ�
	IPL1	= 0x00;
	IPH1	= 0x00;
}