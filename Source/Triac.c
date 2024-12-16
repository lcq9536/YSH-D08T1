#include <SH79F3212.h>

#include "Triac.h"

unsigned int Triac_Count = 0;
unsigned char Cycles = 0;
unsigned int Edge_Count = 0;

//	Ԥ��ֵ
unsigned char On_Time = 0;
unsigned char Off_Time = 0;
unsigned char Skip_Cycles = 0;
unsigned char Total_Cycles = 0;

/**
  * @Brief	�ɿع趪�����
  * @Param  On_Time:�ɿع迪��ʱ��
			Off_Time:�ɿع�ر�ʱ��
			Skip_Cycles:Total_Cycles������������
			Total_Cycles:�ֶε�������
			Time:���е�ʱ��
  * @Note  	None
  * @RetVal	None
  */
void Triac_SkipCycles(unsigned char On_Time,unsigned char Off_Time,
	unsigned char Skip_Cycles, unsigned char Total_Cycles)
{
	unsigned char Temp = On_Time + Off_Time;
	
	if(Cycles >= Total_Cycles * 2) Cycles = 0;
	else if(Cycles < Total_Cycles * 2)
	{
		Triac_Count ++;
		if(Triac_Count >= Temp * 8 - 1)
		{
			Triac_Count = 0;
			Cycles ++;	//	���ڼ���+1
		}
	
		if(Triac_Count < On_Time * 8 - 1)
		{
			if(Cycles < Total_Cycles * 2 - Skip_Cycles * 2) SCR = 1;
			else SCR = 0;
		}
		else if(Triac_Count < Temp * 8 - 1)	SCR = 0;
	}
}

/**
  * @Brief	�ɿع�������ƺ���
  * @Param  ��������ͬ��
  * @Note   None
  * @RetVal	None
  */
void TriacSet(unsigned char OnTime,unsigned char OffTime,
	unsigned char SkipCycles, unsigned char TotalCycles)
{
	Triac_Count = 0;
	Cycles = 0;
	Edge_Count = 0;
	
	On_Time = OnTime;
	Off_Time = OffTime;
	Skip_Cycles = SkipCycles;
	Total_Cycles = TotalCycles;
}