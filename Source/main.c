#include <intrins.h>
#include <SH79F3212.h>

#include "Buzzer.h"
#include "Voltage.h"
#include "Triac.h"
#include "NTC.h"
#include "Key.h"
#include "LED.h"
#include "Timer.h"
#include "Work.h"
#include "tk.h"
#include "MCU.h"
#include "Digital.h"

bool Flag = false;	//	LED������ܷ�ʱ����

unsigned char Sys_Count = 0;	//	ϵͳ��ʱ
unsigned char Sys_State = 0;	//	״̬��ѡ��λ


void main()
{
	MCU_Init();
	Timer0_Init();
	InitKey();
	BuzzerSet(1,1000,300);
	LedSet(2,0x7f);
	DigitalSet(26,26,26);
	while(1)
	{
		RSTSTAT = 0x02;
		
		if(Sys_Count >= 15)
		{
			Sys_Count = 0;
			
			Sys_State ++;
			if(Sys_State >= 10) Sys_State = 0;
			
			tkRound();
			if(Buzzer_State != 1) AD_Value = ADC_Read();
			
			switch(Sys_State)
			{
				case 1:
					ADCON = 0;	//	�ر�ADC
					ADC2H = 0;	//	P3.3��Ϊ��ͨIO��
					Buzzer(Buzzer_Times,Buzzer_Interval,Buzzer_Duration);
				break;
				
				case 2:
					Get_KeyValue(KeyRead);
					Key_Handle();
				break;
				
				case 3:
					AD_Deal(AD_Value);
					Temperature = AD_To_Temperature(AD_Value);
					Temperature_Deal(Temperature);
				break;
					
				case 4:
					StandbyMode();	//	����״̬
				break;
					
				case 5:
					CheckMode();	//	�Լ�״̬
				break;
					
				case 6:
					AlarmMode();	//	����״̬
				break;
				
				case 7:	
					SelectMode();	//	ѡ��״̬
				break;
				
				case 8:	
					WorkMode();		//	����״̬
				break;
			}
		}
	}
}

/**
  * @Brief	�жϷ�����
  * @Param  None
  * @Note   ͨ���ж����������
  * @RetVal	None
  */
void timer0() interrupt 1
{
	TL0		= 0xe3;
	TH0		= 0xf7;
	
	TF0 = 0;	// ����жϱ�־λ
	
	Sys_Count ++;
	
	// ��������
	if(Buzzer_State == 1)
	{
		BUZZER = !BUZZER;
	}
	
	// �ɿع貿��
	Triac_SkipCycles(On_Time,Off_Time,Skip_Cycles,Total_Cycles);
	
	// ��ʾ����
	Flag = !Flag;
	if(!Flag)
	{
		Led_Display(LED_State,LED_Value);
	}
	else
	{
		Digital_Display(Digital_Num1,Digital_Num2,Digital_Num3);
	}
}

