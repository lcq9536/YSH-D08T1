#include <intrins.h>
#include <SH79F3212.h>

#include "NTC.h"

unsigned int AD_Value = 0;
unsigned char Fileter_Cnt = 0;
unsigned int Temperature = 0;	//	���������¶�ֵ
unsigned int Last_Temperature = 0; // ��һ�ε��¶�ֵ

unsigned int code AD_Data[] = //����30k 100k��12λntc
{
	3731,3714,3696,3677,3658,3638,3618,3597,3575,3553,//0-9
	3530,3506,3482,3457,3431,3405,3378,3350,3322,3293,//10-19
	3263,3233,3202,3171,3139,3106,3073,3039,3005,2970,//20-29
	2935,2900,2864,2827,2790,2753,2716,2678,2640,2602,//30-39
	2563,2524,2486,2447,2408,2369,2329,2290,2251,2212,//40-49
	2173,2134,2096,2057,2019,1981,1943,1905,1868,1831,//50-59
	1794,1758,1722,1686,1651,1616,1582,1548,1515,1482,//60-69
	1449,1417,1386,1355,1324,1294,1265,1236,1208,1180,//70-79
	1152,1125,1099,1073,1048,1023,999 ,975 ,952 ,929 ,//80-89
	906 ,885 ,863 ,842 ,822 ,802 ,783 ,764 ,745 ,727 ,//90-99
	709 ,692 ,675 ,659 ,642 ,627 ,611 ,597 ,582 ,568  //100-110
};

unsigned int ADC_Read()
{
	unsigned char i = 100;
	unsigned int Temp = 0;	//	������ʱADֵ
	
	P3CR &= 0xf7;	//	��������ģʽ
	
	ADCON = 0x00;	//	�ر�AD,��AD�жϱ�־λ,�رձȽ��ж�,��ADת��������־λ
	ADT = 0xef;		//	ADC��������Ϊ32*(1/16.6M)������ʱ��Ϊ15���������ڣ���ת��ʱ��Ϊ27���������ڣ�Լ52usһ��ת��
	
	ADC1H = 0x00;
	ADC2H = 0x88;	//	P3.3��ΪADC�����,����1.25V��׼Դ

	ADCON =  0x97;	//	��11��ͨ��������ADC��ADCת��
	
	while(ADCON & 0x01 && i --)	// �ȴ�ADCת������ͬʱ���ADCת������100�ξ��ͷ�
	{
		_nop_();
		RSTSTAT = 0x02;
	}
	
	Temp = (unsigned int)ADDH;	//	��������12λ
	Temp <<= 4;
	Temp += (unsigned int)ADDL;
	
	return Temp;
}

/**
  * @Brief	AD�˲�
  * @Param  None
  * @Note   None
  * @RetVal	None
  */
void AD_Deal(unsigned int Temp)
{
	unsigned char Cnt = 0;	// ͨ���ɼ����ݸ���
	
	unsigned int Temp_Max = 0;	//	���ADֵ
	unsigned int Temp_Min = 0;
	unsigned int Temp_Sum = 0;
	
	if(!Cnt)	//	�տ�ʼ�ɼ�
	{
		Temp_Max = Temp;
		Temp_Min = Temp;
	}
	if(Temp > Temp_Max)	Temp_Max = Temp;	//	����ɼ��������ݴ���֮ǰ�����ֵ		
	else if(Temp < Temp_Min) Temp_Min = Temp;
	
	Temp_Sum += Temp;	//	ͳ�Ʋɼ�����֮��
	
	Cnt ++;
	if(Cnt >= 10)	//	�ɼ�50������
	{
		Cnt = 0;
		AD_Value = (Temp_Sum - Temp_Max - Temp_Min) / 8;	//	����ƽ���㷨
		Temp_Sum = 0;
	}
}

/**
  * @Brief	�����ADֵת���ɶ�Ӧ���¶�
  * @Param  None
  * @Note   None
  * @RetVal	�Ŵ�ʮ�����¶�
  */
unsigned int AD_To_Temperature(unsigned int Value)
{
	unsigned char i = 0;	//	����
	unsigned int Ret_Temp = 0;
	unsigned int Temp = AD_Data[0];	//	���ADֵ
	
	if(Value < AD_Data[55])	i = 55;	//	ȡ��ֵ�Ƚ�,Value�������Ϊ55
	else i = 0;						//	Value������СֵΪ0
	
	while(Value <= Temp)	//	Ѱ�ҵ�һ��С��Value��ֵ,��ʱiָ����һ������
	{
		Temp = AD_Data[i];
		i ++;
	}
	
	if(i > 101) Ret_Temp = 100;	//	����¶�ֵΪ100��
	else if(i == 0) Ret_Temp = 0;	//	��С�¶�ֵΪ0��
	else Ret_Temp = i - 1 + (Value - AD_Data[i - 1]) / (AD_Data[i - 2] - AD_Data[i - 1]);
	
	if(Ret_Temp > 100)	Ret_Temp = 100;
	
	return Ret_Temp;
}

/**
  * @Brief	�����˲�,�����˲�����
  * @Param  Value:�������¶�����
  * @Note   ȷ���¶���������ʵ��Ч�ģ�������������������
  * @RetVal	�˲�������
  */
void Temperature_Deal(unsigned int Value)
{
	if(Value == Last_Temperature)	Fileter_Cnt = 0;
	else if(Value >= Last_Temperature)	//	��ǰ���������ݴ����ϴδ洢������
	{
		Fileter_Cnt &= 0x0f;	//	��λ��¼���ڴ���
		Fileter_Cnt ++;
		if(Fileter_Cnt > 10)	//	���ڴ�������10��
		{
			Fileter_Cnt = 0;
			Temperature = Value;	//	���´��洫��������
		}
	}
	else	//	��ǰ����������С���ϴδ洢������
	{
		Fileter_Cnt &= 0xf0;	//	��λ��¼С�ڴ���
		Fileter_Cnt += 0x10;
		if(Fileter_Cnt > 0xa0)	//	���ڴ�������10��
		{
			Fileter_Cnt = 0;
			Temperature = Value;	//	���´��洫��������
		}
	}
	Last_Temperature = Value;	//	������һ���¶�ֵ
}