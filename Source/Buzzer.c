#include <SH79F3212.h>

#include "NTC.h"
#include "Buzzer.h"

unsigned char 	Buzzer_State = 0;			// ��������Ϊ����״̬:��ʼ�����������
unsigned int 	Buzzer_IntervalCount = 0; 	// ���ڷ������ո�ʱ���ʱ
unsigned int 	Buzzer_DurationCount = 0; 	// ���ڷ���������ʱ���ʱ
unsigned char 	Buzzer_Count = 0;			// ��¼��������Ĵ���

// ������Ԥ��ֵ
unsigned char 	Buzzer_Times = 0;
unsigned int 	Buzzer_Interval = 0;
unsigned int 	Buzzer_Duration = 0;

/**
  * @Brief	��������
  * @Param  Buzzer_Times:��������
			Interval:���ʱ��,��λms
			Duration:����ʱ��,��λms
  * @Note   ʱ��:20ms
  * @RetVal	None
  */
void Buzzer(unsigned char beep_times,unsigned int interval,unsigned int duration)
{
	if(Buzzer_State == 0 && Buzzer_Count < beep_times) // �����û��ʼ���ҷ�������û��
	{
		Buzzer_State = 1;
	}
	else if(Buzzer_State == 1)	// ����״̬
	{	
		P3CR |= 0x08;
		Buzzer_DurationCount ++;
		if(Buzzer_DurationCount >= (duration / 20 - 1))	// ����ʱ�����
		{
			Buzzer_DurationCount = 0;
			Buzzer_State = 2;
			P3CR &= 0xf7;
		}
	}
	else if(Buzzer_State == 2)	// ���״̬
	{
		Buzzer_IntervalCount ++;
		if(Buzzer_IntervalCount >=  (interval / 20 - 1))	// ���ʱ�����
		{
			Buzzer_IntervalCount = 0;
			Buzzer_Count ++;	// ��¼��������
			if(Buzzer_Count < beep_times)	Buzzer_State = 1;	//	��������û���
			else 
			{
				Buzzer_State = 0;	// �����������
			}
		}
	}
}

/**
  * @Brief	����������ˢ�²���
  * @Param  Buzzer_Times:��������
			interval:���ʱ��
			duration:����ʱ��
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
