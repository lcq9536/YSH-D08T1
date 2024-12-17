#include <intrins.h>
#include <SH79F3212.h>

#include "NTC.h"

unsigned int AD_Value = 0;
unsigned char Fileter_Cnt = 0;
unsigned int Temperature = 0;	//	储存最终温度值
unsigned int Last_Temperature = 0; // 上一次的温度值

unsigned int code AD_Data[] = //上拉30k 100k的12位ntc
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
	unsigned int Temp = 0;	//	储存临时AD值
	
	P3CR &= 0xf7;	//	配置输入模式
	
	ADCON = 0x00;	//	关闭AD,清AD中断标志位,关闭比较中断,清AD转换结束标志位
	ADT = 0xef;		//	ADC采样周期为32*(1/16.6M)，采样时间为15个采样周期，总转换时间为27个采样周期，约52us一次转换
	
	ADC1H = 0x00;
	ADC2H = 0x88;	//	P3.3作为ADC输入口,开启1.25V基准源

	ADCON =  0x97;	//	第11个通道并启用ADC和ADC转换
	
	while(ADCON & 0x01 && i --)	// 等待ADC转换结束同时如果ADC转换卡了100次就释放
	{
		_nop_();
		RSTSTAT = 0x02;
	}
	
	Temp = (unsigned int)ADDH;	//	数据整合12位
	Temp <<= 4;
	Temp += (unsigned int)ADDL;
	
	return Temp;
}

/**
  * @Brief	AD滤波
  * @Param  None
  * @Note   None
  * @RetVal	None
  */
void AD_Deal(unsigned int Temp)
{
	unsigned char Cnt = 0;	// 通道采集数据个数
	
	unsigned int Temp_Max = 0;	//	最大AD值
	unsigned int Temp_Min = 0;
	unsigned int Temp_Sum = 0;
	
	if(!Cnt)	//	刚开始采集
	{
		Temp_Max = Temp;
		Temp_Min = Temp;
	}
	if(Temp > Temp_Max)	Temp_Max = Temp;	//	后面采集到的数据大于之前的最大值		
	else if(Temp < Temp_Min) Temp_Min = Temp;
	
	Temp_Sum += Temp;	//	统计采集数据之和
	
	Cnt ++;
	if(Cnt >= 10)	//	采集50个数据
	{
		Cnt = 0;
		AD_Value = (Temp_Sum - Temp_Max - Temp_Min) / 8;	//	滑动平均算法
		Temp_Sum = 0;
	}
}

/**
  * @Brief	查表法将AD值转化成对应的温度
  * @Param  None
  * @Note   None
  * @RetVal	放大十倍的温度
  */
unsigned int AD_To_Temperature(unsigned int Value)
{
	unsigned char i = 0;	//	索引
	unsigned int Ret_Temp = 0;
	unsigned int Temp = AD_Data[0];	//	最大AD值
	
	if(Value < AD_Data[55])	i = 55;	//	取中值比较,Value索引起点为55
	else i = 0;						//	Value索引最小值为0
	
	while(Value <= Temp)	//	寻找第一个小于Value的值,此时i指向下一个数据
	{
		Temp = AD_Data[i];
		i ++;
	}
	
	if(i > 101) Ret_Temp = 100;	//	最大温度值为100°
	else if(i == 0) Ret_Temp = 0;	//	最小温度值为0°
	else Ret_Temp = i - 1 + (Value - AD_Data[i - 1]) / (AD_Data[i - 2] - AD_Data[i - 1]);
	
	if(Ret_Temp > 100)	Ret_Temp = 100;
	
	return Ret_Temp;
}

/**
  * @Brief	迟滞滤波,更新滤波数据
  * @Param  Value:传感器温度数据
  * @Note   确保温度上升是真实有效的，而不是由于噪声引起
  * @RetVal	滤波后数据
  */
void Temperature_Deal(unsigned int Value)
{
	if(Value == Last_Temperature)	Fileter_Cnt = 0;
	else if(Value >= Last_Temperature)	//	当前传感器数据大于上次存储的数据
	{
		Fileter_Cnt &= 0x0f;	//	低位记录大于次数
		Fileter_Cnt ++;
		if(Fileter_Cnt > 10)	//	大于次数超过10次
		{
			Fileter_Cnt = 0;
			Temperature = Value;	//	更新储存传感器数据
		}
	}
	else	//	当前传感器数据小于上次存储的数据
	{
		Fileter_Cnt &= 0xf0;	//	高位记录小于次数
		Fileter_Cnt += 0x10;
		if(Fileter_Cnt > 0xa0)	//	大于次数超过10次
		{
			Fileter_Cnt = 0;
			Temperature = Value;	//	更新储存传感器数据
		}
	}
	Last_Temperature = Value;	//	保存上一次温度值
}