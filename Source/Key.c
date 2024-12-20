#include <SH79F3212.h>

#include "Key.h"
#include "Buzzer.h"
#include "tk.h"

unsigned char Delay_Cnt = 0;		//	������ʱ
unsigned int KeyValue = 0;			//	��ֵ
unsigned char KeyCount = 0;		//	��������ʱ�����
unsigned char LongKey_Flag = 0;	//	�����̰���־λ
unsigned int Last_KeyValue = 0;	//	һ��ɨ�������ڵľɼ�ֵ
unsigned int Last_KeyCode = 0;		//	һ���������������ľɼ�ֵ

/**
  * @Brief	�жϼ�ֵ
  * @Param  Key_Read:��⵽�ļ�ֵ
  * @Note   None
  * @RetVal	None
  */
void Get_KeyValue(unsigned int Key_Read)
{
	KeyValue = 0;
	
	if(Key_Read & 0x01)	KeyValue |= 0x01;	//	K1
	if(Key_Read & 0x02)	KeyValue |= 0x02;	//	K2
	if(Key_Read & 0x04)	KeyValue |= 0x04;	//	K3
}

/**
  * @Brief	��ֵ����
  * @Param  None
  * @Note   None
  * @RetVal	None
  */
void Key_Handle()
{
	if(KeyValue && !Last_KeyValue)	//	ɨ�赽�������²������°���
	{
		KeyCount = 0;
		Last_KeyValue = KeyValue;
	}
	else if(KeyValue == Last_KeyValue)	//	ɨ�赽������������
	{
		if(KeyCount < 254) KeyCount ++;	//	20ms+1
	}
	else if(!KeyValue)	//	ɨ�赽�����ɿ�
	{
		KeyCount = 0;
		if(Last_KeyValue)//	��һ���м�ֵ���ж�Ϊ��һ���ɿ�
		{
			Last_KeyCode = Last_KeyValue;	//	�����ϴ��ɿ�ʱ��ֵ
			Last_KeyValue = 0;
		}
	}
}