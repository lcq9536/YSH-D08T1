#include <SH79F3212.h>

#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "Digital.h"
#include "NTC.h"
#include "Triac.h"
#include "Work.h"

bool Scroll_Flag = false;
unsigned char PowerOnCount1 = 0;	//	�ϵ����
unsigned char PowerOnCount2 = 0;	//	�ϵ����
//	ϵͳ״̬:0-�ϵ�״̬��1-����״̬��2-�Լ�״̬��3-����״̬��4-ѡ��״̬��5-����״̬
unsigned char Sys_Mode = 1;

bool NTCShort = false;
bool NTCOpen = false;
bool DryBoil = false;
bool ErrorFlag = true;	//	�쳣��־λ

unsigned char FlashCount = 0;	//	�������˸����
unsigned char ErrorCount = 0;	//	�������
unsigned char PressCount = 0;	//	���ܼ����´���

unsigned char ScrollPos = 0;		//	������ֲ�����
unsigned char ScrollCounter = 0;	//	������ֲ�����
unsigned int xdata ScrollData[11][3] = {	//	������ֲ�����
	{10,0,0},{11,1,1},{12,2,2},{13,3,3},{14,4,4},{15,5,5},
	{16,6,6},{17,7,7},{18,8,8},{19,9,9},{22,0,0}
};

/**
  * @Brief	����ģʽ
  * @Param  None
  * @Note   None
  * @RetVal	None
  */
void StandbyMode()
{
	if(Sys_Mode == 1)
	{
		if(PowerOnCount1 <= 49) PowerOnCount1 ++;
		else
		{
			PowerOnCount1 = 50;
			DigitalSet(10,0,0);
			LedSet(0,0x00,0);
			BuzzerSet(0,0,0);
			TriacSet(0,0,0,0);
			//	���ִ���
		}
	}
}

/**
  * @Brief	�Լ�ģʽ
  * @Param  None
  * @Note   None
  * @RetVal	None
  */
void CheckMode()
{
	if(Sys_Mode != 3)
	{
		PowerOnCount2 ++;
		if(PowerOnCount2 < 199)
		{
			if(KeyValue == 0x03)
			{
				Sys_Mode = 2;	//	�Լ�״̬
				if(KeyCount == 80 - 2)
				{
					BuzzerSet(2,100,100);
					TriacSet(4,6,0,10);
					DigitalSet(10,0,0);
					Scroll_Flag = true;	//	����������ֲ�
				}
			}
		}
		else PowerOnCount2 = 200;	//	ǯס���ϵ�4����
	
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
					Sys_Mode = 1;	//	�л�����״̬
					Scroll_Flag = 0;	//	�ر��ֲ�
				}
			}
		}
	}
}

/**
  * @Brief	����״̬
  * @Param  None
  * @Note   None
  * @RetVal	None
  */
void AlarmMode()
{
	FlashCount ++;
	if(FlashCount >= 50) FlashCount = 0;
	
	if(Buzzer_State != 1)
	{
		if(AD_Value < 80 || //	��·195��
			(NTCShort && AD_Value < (DryBoil?100:709)))	//	�ָ�����
		{
			ErrorCount ++;
			if(ErrorCount > 25)
			{
				ErrorCount = 0;
				NTCShort = true;
				Sys_Mode = 3;
				
				if(FlashCount < 25) DigitalSet(21,0,1);	//	E01
				else DigitalSet(23,23,23);
				if(ErrorFlag)	//	�������ִֻ��һ��
				{
					ErrorFlag = false;
					LedSet(0,0x00,0);
					BuzzerSet(3,300,300);
					TriacSet(0,0,0,0);
				}
			}
		}
		else if((Sys_Mode == 5 && AD_Value < 100 && AD_Value > 80) //	��������185��
			|| (DryBoil && AD_Value > 80 && AD_Value < 709))	//	�ָ�����
		{
			ErrorCount ++;
			if(ErrorCount > 25)
			{
				ErrorCount = 0;
				DryBoil = true;
				Sys_Mode = 3;
				
				if(FlashCount < 25) DigitalSet(21,0,0);	//	E00
				else DigitalSet(23,23,23);
				if(ErrorFlag)
				{
					ErrorFlag = false;
					LedSet(0,0x00,0);
					BuzzerSet(3,300,300);
					TriacSet(0,0,0,0);
				}
			}
		}
		else if(AD_Value > 3965 || //��·-20��
			(NTCOpen && AD_Value > 3530 && AD_Value < 3965))	//	�ָ�����
		{
			ErrorCount ++;
			if(ErrorCount > 25)
			{
				ErrorCount = 0;
				NTCOpen = true;
				Sys_Mode = 3;
				
				if(FlashCount < 25) DigitalSet(10,0,0);	//	000
				else if(FlashCount >= 25) DigitalSet(23,23,23);
				if(ErrorFlag)
				{
					ErrorFlag = false;
					DigitalSet(10,0,0);
					LedSet(0,0x00,0);
					BuzzerSet(3,300,300);
					TriacSet(0,0,0,0);
				}
			}
		}
		else if(Sys_Mode == 3 && AD_Value > 709 && AD_Value < 3530)
		{
			NTCOpen = false;
			NTCShort = false;
			DryBoil = false;
			ErrorFlag = true;
			Sys_Mode = 1;	//	�л�����״̬
		}
	}
}

void SelectMode()
{
	
}

void WorkMode()
{
	
}