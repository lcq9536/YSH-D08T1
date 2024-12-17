#include <intrins.h>
#include <SH79F3212.h>

#include "Buzzer.h"
#include "Voltage.h"
#include "Triac.h"
#include "NTC.h"
#include "Key.h"
#include "LED.h"
#include "Timer.h"
#include "tk.h"
#include "MCU.h"
#include "Digital.h"

bool Flag = false;	//	LED������ܷ�ʱ����
bool Scroll_Flag = false;

unsigned char Sys_Count = 0;	//	ϵͳ��ʱ
unsigned char Sys_State = 0;	//	״̬��ѡ��λ

//	ϵͳ״̬:0-�ϵ�״̬��1-����״̬��2-�Լ�״̬��3-����״̬��4-ѡ��״̬��5-����״̬
unsigned char Sys_Mode = 1;

unsigned int PowerOnCount1 = 0;	//	�ϵ����
unsigned int PowerOnCount2 = 0;	//	�ϵ����

unsigned int xdata ScrollData[11][3] = {
	{10,0,0},{11,1,1},{12,2,2},{13,3,3},{14,4,4},{15,5,5},
	{16,6,6},{17,7,7},{18,8,8},{19,9,9},{21,0,0}
};
unsigned char ScrollPos = 0;	//	������ֲ�����
unsigned int ScrollCounter = 0;	//	������ֲ�����

void main()
{
	MCU_Init();
	Timer0_Init();
	InitKey();
	BuzzerSet(1,1000,300);
	LedSet(2,0x7f,0xff);
	DigitalSet(23,23,23);
	while(1)
	{
		RSTSTAT = 0x02;
		
		if(Sys_Count >= 15)
		{
			Sys_Count = 0;
			
			Sys_State ++;
			if(Sys_State >= 10) Sys_State = 0;
			
			tkRound();
			
			switch(Sys_State)
			{
				case 1:
					ADCON = 0;	//	�ر�ADC
					ADC2H = 0;	//	P3.3��Ϊ��ͨIO��
					Buzzer(Buzzer_Times,Buzzer_Interval,Buzzer_Duration);
				break;
				
				case 2:
					Get_KeyValue(KeyRead);	//	�жϼ�ֵ
					Key_Handle();	//	��ֵ����
				break;
				
				case 3:
					if(Buzzer_State != 1)
					{
						AD_Value = ADC_Read();
						AD_Deal(AD_Value);
						Temperature = AD_To_Temperature(AD_Value);
						Temperature_Deal(Temperature);
					}
				break;
					
				case 4:	//	����״̬
					PowerOnCount1 ++;
					if(PowerOnCount1 > 49 && Sys_Mode == 1)
					{
						PowerOnCount1 = 8000;
						DigitalSet(10,0,0);
						LedSet(0,0x00,0);
						BuzzerSet(0,0,0);
						TriacSet(0,0,0,0);
						if(KeyValue == 0x01)
						{
							Sys_Mode = 2;
						}
					}
				break;
					
				case 5:	//	�Լ�״̬
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
					else PowerOnCount2 = 200;
					
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
		Led_Display(LED_State,LED_Value,LED_Time);
	}
	else
	{
		Digital_Display(Digital_Num1,Digital_Num2,Digital_Num3);
	}
}

