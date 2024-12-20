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
bool SelectFlag = false;
bool AdjustFlag = false;	//	���ڼ���־

unsigned char FlashCount1 = 0;	//	�������˸����,���ڱ���״̬
unsigned char FlashCount2 = 0;	//	�������˸����,���ڱ���״̬
unsigned char ErrorCount = 0;	//	�������
unsigned char PressCount = 0;	//	���ܼ����´���
unsigned char NoAction_Count = 0;	//	���ڽ��빤��״̬����
unsigned char ScrollPos = 0;		//	������ֲ�����
unsigned char ScrollCounter = 0;	//	������ֲ�����
unsigned char WorkTime = 0;
unsigned int  Lase_KeyCount = 0; 	//	��¼��������ʱ�ļ���ֵ
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
			LedSet(0,0x00);
			BuzzerSet(0,0,0);
			TriacSet(0,0,0,0);
			if(Last_KeyCode == 0x01)
			{
				Sys_Mode = 4;
			}
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
	if(Sys_Mode != 3)	//	����״̬�����Լ�
	{
		PowerOnCount2 ++;
		if(PowerOnCount2 < 199)
		{
			if(KeyValue == 0x03)
			{
				if(KeyCount == 80 - 2)
				{
					Sys_Mode = 2;	//	�Լ�״̬
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
					Sys_Mode = 1;		//	�л�����״̬
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
	FlashCount1 ++;
	if(FlashCount1 >= 50) FlashCount1 = 0;
	
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
				
				if(FlashCount1 < 25) DigitalSet(21,0,1);	//	E01
				else DigitalSet(25,25,25);
				if(ErrorFlag)	//	�������ִֻ��һ��
				{
					ErrorFlag = false;
					LedSet(0,0x00);
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
				
				if(FlashCount1 < 25) DigitalSet(21,0,0);	//	E00
				else DigitalSet(25,25,25);
				if(ErrorFlag)
				{
					ErrorFlag = false;
					LedSet(0,0x00);
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
				
				if(FlashCount1 < 25) DigitalSet(10,0,0);	//	000
				else if(FlashCount1 >= 25) DigitalSet(25,25,25);
				if(ErrorFlag)
				{
					ErrorFlag = false;
					DigitalSet(10,0,0);
					LedSet(0,0x00);
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
	if(Sys_Mode == 4)
	{
		NoAction_Count ++;	//	���ڼ��ɿ�����Ϊ�޲���
		if(Last_KeyCode == 0x01)
		{
			Last_KeyCode = 0;
			PressCount ++;
			SelectFlag = true;
			if(PressCount > 7) PressCount = 1;
			BuzzerSet(1,100,100);
		}
		switch(PressCount)
		{
			case 1:
				if(SelectFlag)
				{
					NoAction_Count = 0;	//	���¼�ʱ
					SelectFlag = false;
					LedSet(1,0x01);
					DigitalSet(23,24,24);
				}
				if(NoAction_Count >= 250)
				{
					NoAction_Count = 0;
					LedSet(2,0x01);
					Sys_Mode = 5;
				}
			break;
			case 2:
				if(SelectFlag)
				{
					NoAction_Count = 0;
					SelectFlag = false;
					LedSet(1,0x02);
					WorkTime = 5;
					DigitalSet(10,0,5);
				}
				if(NoAction_Count >= 250)	//	�޲������빤��״̬
				{
					NoAction_Count = 0;
					LedSet(2,0x02);
					Sys_Mode = 5;
				}
				if(Last_KeyCode == 0x02)
				{
					Last_KeyCode = 0;
					NoAction_Count = 0;
					FlashCount2 = 0;	//	���¿�ʼ��
					AdjustFlag = true;
					BuzzerSet(1,100,100);
					WorkTime += 1;
					if(WorkTime > 10) WorkTime = 1;
				}
				if(AdjustFlag)//	���ڼ��ɿ��ٿ�ʼ��˸
				{
					FlashCount2 ++;
					if(FlashCount2 > 50) FlashCount2 = 0;
					if(FlashCount2 < 25) DigitalSet(10,WorkTime/10,WorkTime%10);
					else DigitalSet(25,25,25);
				}
			break;
			case 3:
				if(SelectFlag)
				{
					NoAction_Count = 0;
					SelectFlag = false;
					LedSet(1,0x04);
					WorkTime = 90;
					DigitalSet(11,3,0);
				}
				if(NoAction_Count >= 250)
				{
					NoAction_Count = 0;
					LedSet(2,0x04);
					Sys_Mode = 5;
				}
				if(Last_KeyCode == 0x02)
				{
					Last_KeyCode = 0;
					NoAction_Count = 0;
					FlashCount2 = 0;
					AdjustFlag = true;
					BuzzerSet(1,100,100);
					WorkTime += 5;
					if(WorkTime > 150) WorkTime = 60;
				}
				if(AdjustFlag)//	���ڼ��ɿ��ٿ�ʼ��˸
				{
					FlashCount2 ++;
					if(FlashCount2 > 50) FlashCount2 = 0;
					if(FlashCount2 < 25) DigitalSet(10+WorkTime/60,WorkTime%60/10,WorkTime%60%10);
					else DigitalSet(25,25,25);
				}
			break;
			case 4:
				if(SelectFlag)
				{
					NoAction_Count = 0;
					SelectFlag = false;
					LedSet(1,0x08);
					WorkTime = 60;
					DigitalSet(11,0,0);
				}
				if(NoAction_Count >= 250)
				{
					NoAction_Count = 0;
					LedSet(2,0x08);
					Sys_Mode = 5;
				}
				if(Last_KeyCode == 0x02)
				{
					Last_KeyCode = 0;
					NoAction_Count = 0;
					FlashCount2 = 0;
					AdjustFlag = true;
					BuzzerSet(1,100,100);
					WorkTime += 5;
					if(WorkTime > 120) WorkTime = 30;
				}
				if(AdjustFlag)//	���ڼ��ɿ��ٿ�ʼ��˸
				{
					FlashCount2 ++;
					if(FlashCount2 > 50) FlashCount2 = 0;
					if(FlashCount2 < 25) DigitalSet(10+WorkTime/60,WorkTime%60/10,WorkTime%60%10);
					else DigitalSet(25,25,25);
				}
			break;
			case 5:
				if(SelectFlag)
				{
					NoAction_Count = 0;
					SelectFlag = false;
					LedSet(1,0x10);
					WorkTime = 90;
					DigitalSet(11,3,0);
				}
				if(NoAction_Count >= 250)
				{
					NoAction_Count = 0;
					LedSet(2,0x10);
					Sys_Mode = 5;
				}
				if(Last_KeyCode == 0x02)
				{
					Last_KeyCode = 0;
					NoAction_Count = 0;
					FlashCount2 = 0;
					AdjustFlag = true;
					BuzzerSet(1,100,100);
					WorkTime += 5;
					if(WorkTime > 120) WorkTime = 30;
				}
				if(AdjustFlag)//	���ڼ��ɿ��ٿ�ʼ��˸
				{
					FlashCount2 ++;
					if(FlashCount2 > 50) FlashCount2 = 0;
					if(FlashCount2 < 25) DigitalSet(10+WorkTime/60,WorkTime%60/10,WorkTime%60%10);
					else DigitalSet(25,25,25);
				}
			break;
			case 6:
				if(SelectFlag)
				{
					NoAction_Count = 0;
					SelectFlag = false;
					LedSet(1,0x20);
					WorkTime = 55;
					DigitalSet(5,15,20);
				}
				if(NoAction_Count >= 250)
				{
					NoAction_Count = 0;
					LedSet(2,0x20);
					Sys_Mode = 5;
				}
				if(KeyValue == 0x02)	//	�����ж�
				{
					if(KeyCount == 10 - 2)	//	�̰�
					{
						NoAction_Count = 0;
						FlashCount2 = 0;
						AdjustFlag = true;
						BuzzerSet(1,100,100);
						WorkTime += 5 ;	
						if(WorkTime > 90) WorkTime = 40;
					}
					if(KeyCount > 50 - 2)	//	����
					{
						KeyCount = 35;	//	ÿ����48-35=13=260�����һ��5
						NoAction_Count = 0;
						FlashCount2 = 0;
						AdjustFlag = true;
						WorkTime += 5 ;	
						if(WorkTime > 90) WorkTime = 40;
					}
				}
				if(AdjustFlag)//	���ڼ��ɿ��ٿ�ʼ��˸
				{
					FlashCount2 ++;
					if(FlashCount2 > 50) FlashCount2 = 0;
					if(FlashCount2 < 25) DigitalSet(WorkTime/10%10,WorkTime%10+10,20);
					else DigitalSet(25,25,25);
				}
			break;
			case 7:
				if(SelectFlag)
				{
					NoAction_Count = 0;
					SelectFlag = false;
					LedSet(0,0);
					DigitalSet(10,0,0);
				}
				if(NoAction_Count >= 250)
				{
					NoAction_Count = 0;
					Sys_Mode = 1;
				}
			break;
		}
	}
}

void WorkMode()
{
	if(Sys_Mode == 5)
	{
		DigitalSet(Temperature/10%10,Temperature%10+10,20);
	}
	
}