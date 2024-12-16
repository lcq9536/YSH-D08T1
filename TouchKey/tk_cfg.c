#include	<intrins.h>
#include	<ABSACC.h>
#include 	<SH79F3212.h>

#include 	"tk.h" 

//===============================================================================================
//							��ӱ SH79F3212 ϵ�д����� V2.0
//							-----------------------------
//									������������								
//	
//===============================================================================================

#pragma message "����֧����ֱ����"

#ifdef	TEST_TK_DATA	
	#pragma message "�򿪴��ڵ��� ������100000bps"				
#else	
	#pragma message "�رմ��ڵ���"					
#endif
																					
																					
#define     TK_VREF_SEL_2P5V		TKVREF = (TKVREF & 0x3F);
#define     TK_VREF_SEL_2V  		TKVREF = (TKVREF & 0x3F) | 0x40;
#define     TK_VREF_SEL_1P5V  		TKVREF = (TKVREF & 0x3F) | 0x80;
#define     TK_VREF_SEL_1V  		TKVREF = (TKVREF & 0x3F) | 0xC0;

#define     TK_CMP_Delay_4SCK		TKVREF = (TKVREF & 0xCF);
#define     TK_CMP_Delay_8SCK  		TKVREF = (TKVREF & 0xCF) | 0x10;
#define     TK_CMP_Delay_16SCK 		TKVREF = (TKVREF & 0xCF) | 0x20;
#define     TK_CMP_Delay_32SCK 		TKVREF = (TKVREF & 0xCF) | 0x30;

#define     TK_OP_SEL_4V		    TKVREF = (TKVREF & 0xF3);
#define     TK_OP_SEL_3V  		    TKVREF = (TKVREF & 0xF3) | 0x04;
#define     TK_OP_SEL_2P5V  	    TKVREF = (TKVREF & 0xF3) | 0x08;
#define     TK_OP_SEL_2V  		    TKVREF = (TKVREF & 0xF3) | 0x0C;

#define     TK_Dischg_128SCK		TKVREF = (TKVREF & 0xFC);
#define     TK_Dischg_256SCK  		TKVREF = (TKVREF & 0xFC) | 0x01;
#define     TK_Dischg_384SCK 		TKVREF = (TKVREF & 0xFC) | 0x02;
#define     TK_Dischg_512SCK 		TKVREF = (TKVREF & 0xFC) | 0x03;

extern	unsigned char	xdata KEY_NUMBER;					//��������(�û�����)
extern	unsigned char	xdata HF_VALUE;						//��ƵƵ��
extern	unsigned char	xdata HF_TKVREF;					//��Ƶ��ѹ
extern	unsigned char	xdata LF_VALUE;						//��ƵƵ��
extern	unsigned char	xdata LF_TKVREF;					//��Ƶ��ѹ
extern	unsigned char	code  HF_DIV4[MAX_KEY];				  	//��Ƶ�Ŵ���
extern	unsigned char	code  LF_DIV4[MAX_KEY];				   	//��Ƶ�Ŵ���
extern	unsigned char	code  Tab_TKChn[MAX_KEY];				//ͨ����
extern	unsigned int	code  Tab_TKMax[MAX_KEY*2];				//��ֵ

#define		FREQ_4_15M			4			//2.89 -- 5.8
#define		FREQ_3_32M			5			//4  -- 7.5
#define		FREQ_2_76M			6			//5.37 -- 9.13
#define		FREQ_2_37M			7			//6.5 -- 10.6
#define		FREQ_2_075M			8			//7.21 -- 12.13
#define		FREQ_1_84M			9
#define		FREQ_1_66M			10
#define		FREQ_1_50M			11
#define		FREQ_1_38M			12
#define		FREQ_1_27M			13
#define		FREQ_1_18M			14
#define		FREQ_1_1K			15
#define		FREQ_1_0375K		16
#define		FREQ_976K			17
#define		FREQ_922K			18

#define	KEY1_OUT_L	{P1_7 = 0;	P1CR  |=  bt7;}		
#define	KEY2_OUT_L	{P2_0 = 0;	P2CR  |=  bt0;}	//<<--3
#define	KEY3_OUT_L	{P2_1 = 0;	P2CR  |=  bt1;}
#define	KEY4_OUT_L	{P2_2 = 0;	P2CR  |=  bt2;}		//<<--4
#define	KEY5_OUT_L	{P2_3 = 0;	P2CR  |=  bt3;}		//<<--2
#define	KEY6_OUT_L	{P2_4 = 0;	P2CR  |=  bt4;}
#define	KEY7_OUT_L	{P2_5 = 0;	P2CR  |=  bt5;}
#define	KEY8_OUT_L	{P2_6 = 0;	P2CR  |=  bt6;}			//<<--1

#define	CX_OUT_L	 {P2_7 = 0;	P2CR   |=  bt7;}	
					  //8������					//Ԥ��4��

#define CH0_Pin23 	0
#define CH1_Pin24 	1
#define CH2_Pin25 	2
#define CH3_Pin26 	3
#define CH4_Pin27 	4
#define CH5_Pin28 	5
#define CH6_Pin1 	6
#define CH7_Pin2 	7

unsigned char	code  Tab_TKChn[8] = 	{
	 CH0_Pin23,
	 CH1_Pin24,	//	0x02
	 CH6_Pin1	//	0x04
};									 //(***)

 
unsigned char	code  HF_DIV4[MAX_KEY] 	 = {3,3,3,3,3,3,3,3,};									 //(***�����Ŵ���������ǲ���ʱ���а����仯����350~450֮��)
unsigned char	code  LF_DIV4[MAX_KEY] 	 = {3,3,3,3,3,3,3,3,};								 //(***�����Ŵ���������ǲ���ʱ���а����仯����350~450֮��)
unsigned int	code  Tab_TKMax[MAX_KEY*2] = {500,500,500,500,500,500,500,500,			 //��������� ����ȫ��500
					      500,500,500,500,500,500,500,500,};	
unsigned int	code  Tab_HanTouch[MAX_KEY*2] = {1000,1000,1000,1000,1000,1000,1000,1000,		 //����ֱ����
                                                 1000,1000,1000,1000,1000,1000,1000,1000, };



void	InitKey(void)
{
	unsigned char i;

 	KEY_NUMBER = 3;									  	//(***)
	SHARE_NUMBER = 0;									//(***)
	FLAG_COUNT = 2;										//����ȷ�ϴ��� 8ms*2(�ߵ�˫Ƶ)*(FLAG_COUNT) = ms
		
	HF_VALUE =FREQ_2_37M;															   				  //(***)	
    HF_TKVREF = TK_VREF_SEL_2V + TK_CMP_Delay_32SCK +  TK_OP_SEL_4V + TK_Dischg_512SCK;				  //(***)

	LF_VALUE =FREQ_1_18M;																			  //(***)	
	LF_TKVREF =TK_VREF_SEL_1V + TK_CMP_Delay_32SCK +  TK_OP_SEL_4V + TK_Dischg_512SCK;					  //(***)

	gBPatch1 = 0;										//(***)																	
	gBPatch2 = 1;										//��ֱ����һ������Ȼ���ͨ5V����
	gBPatch3 = 1;										//֧����ֱ����
	
	CLKCON = 0;	
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	
	for(i=0;i<KEY_NUMBER;i++)
	{
		switch(Tab_TKChn[i])
		{
			case 0:		KEY1_OUT_L		break;		   
			case 1:		KEY2_OUT_L		break;		   
			case 2:		KEY3_OUT_L		break;		   
			case 3:		KEY4_OUT_L		break;		   
			case 4:		KEY5_OUT_L		break;		   
			case 5:		KEY6_OUT_L		break;		   
			case 6:		KEY7_OUT_L		break;		   
			case 7:		KEY8_OUT_L		break;		   
		}	
	}											 									  //(***)	

	CX_OUT_L

 //SH79f3212	���ؼĴ�������
	#if (PACKAGE==	SH79F3212_28PIN)
#pragma message "SH79F3212----28PIN"		
		P1	 &= (~(bt3+bt4+bt5+bt6));
		P1CR |=   (bt3+bt4+bt5+bt6) ;

		P3	 &= (~(bt5));
		P3CR |=   (bt5) ;

		INSCON |= 0x40;
		P4	 &= (~(bt0+bt1+bt2+bt3));
		P4CR |=   (bt0+bt1+bt2+bt3) ;
		INSCON &= 0xBF;						 
	#endif

	#if (PACKAGE==	SH79F3212_20PIN)
#pragma message "SH79F3212----20PIN"		
		P1	 &= (~(bt1+bt2+bt3+bt4+bt5+bt6));
		P1CR |=   (bt1+bt2+bt3+bt4+bt5+bt6) ;

		P2	 &= (~(bt3));
		P2CR |=   (bt3) ;

		P3	 &= (~(bt0+bt1+bt3+bt4+bt5));
		P3CR |=   (bt0+bt1+bt3+bt4+bt5) ;

		INSCON |= 0x40;
		P4	 &= (~(bt0+bt1+bt2+bt3));
		P4CR |=   (bt0+bt1+bt2+bt3) ;
		INSCON &= 0xBF;		
	#endif

	reset_TK();
}


unsigned int	KeyRead;	 
unsigned char	gBKeyValidFg;
unsigned int	gWRestT;
unsigned int	gWFailT;

unsigned int	Key_DataInput(void)
{
	unsigned int	val;
	
	gBKeyValidFg = 1;

	val = key_value;

	switch(val)															  //(***)
	{	 
		//֧�ֵİ���ֵ
		case 0x0000:	
		case 0x0001:	 //TK1
		case 0x0002:	 //TK2
		case 0x0004:	 //TK3
		case 0x0008:	 //TK4
		case 0x0010:	 //TK5
		case 0x0020:	 //TK6
		case 0x0040:	 //TK7
		case 0x0080:	 //TK8		
		case 0x0003:	 //TK1+TK2
		//֧�ֵ����ֵ
		//TBD
				return val;
		
		default:	gBKeyValidFg = 0; return 0xffff;
	}	
}

unsigned int	catchKey(void)
{
	KeyRead =  Key_DataInput();

	//10ms
	if(gBKeyValidFg == 0)
	{	  
		//�Ƿ���1.8s��λ	
		gWRestT = 0;

		if((++gWFailT)>=180)  		//1.8s		
		{
			gWFailT = 0;
			reset_TK();

			KeyRead = 0;
		}
	}
	else if(KeyRead!=0)
	{
		//����36s��λ
		gWFailT = 0;

		if((++gWRestT)>=3600)		//36s		
		{
			gWRestT = 0;
			reset_TK();	

			KeyRead = 0;
		}
	}
	else
	{
		//�޼�
		gWRestT = 0;
		gWFailT = 0;
	}
    //-----------------------------------------------------------
	return KeyRead;
}

extern	void	test(void);

void tkRound(void)
{
		static unsigned char time;
	
		if(++time>= (TK_SCAN_PERIOD/2))
		{
			time =0;
			catchKey();         				/* ��������2����ֵ����� */		// ������ȡ�����
			stop_scan_tk();						/* ��������3������ɨ�� */	
			#ifdef	TEST_TK_DATA
				UART_Txd_TKDEBUG();				/* ��������4�������������� */
			#endif /* TEST_TK_DATA */		
		}
		else
		{
			SanTk();								/* ��������5���������� */
			SanTk();
		}
		//	test();
}

void TK_Interrupt (void) interrupt  0
{
	TK_Isr ();
}


#ifdef	TEST_TK_DATA
unsigned char   xdata		gBUartSendFg;
bit		gbUartTran;
unsigned char 	xdata	gBUartLen;
unsigned char 	xdata	gBUartTotalByte;
unsigned char 	xdata	gBUartBuf[(MAX_KEY+1)*8+3];		 //���֧��12KEY,������һ����ͨ��
extern	unsigned int   xdata key_baseline[MAX_KEY*2];			//��׼ֵ
extern	unsigned int   xdata key_pre[MAX_KEY*2];			//��׼ֵ
extern	unsigned char  xdata freqForDataMsg;				//��ǰ�������ݶ�Ӧ��Ƶ��			
extern	unsigned char  xdata freqForKeyJudge;				//��ǰ�����жϰ���ʹ�õ�Ƶ��
extern	unsigned int   xdata key_value;		 				//���	
extern	unsigned int   xdata key_RawAd[MAX_KEY*2];	
extern	unsigned char	xdata key_RawTouch[MAX_KEY*2];

void UARTRxD_Interrupt(void) interrupt 4
{
	_push_(FLASHCON);
	_push_(INSCON);
	INSCON = 0;
	FLASHCON = 0;

  if(RI)
	{
		RI= 0;		
	}
	if(TI)
	{		
		TI = 0;
		if(gbUartTran)
		{
			if(gBUartLen <gBUartTotalByte	)
			{
				SBUF	=	gBUartBuf[gBUartLen];				
				gBUartLen++;
			}
			else
			{
				gbUartTran	= 0;
			}	
		}
	}
	_pop_(INSCON);
	_pop_(FLASHCON);
}



void	UART_INIT(void)
{	
	P1_0= 1;			   //TXD
	P1CR |= bt0;
	
	SFINE	=	6;							//16.6M/(10*16+6) = 50000bps
	SBRTL	=	(32768-10)&0xff;
	SBRTH	=	(((32768-10)>>8)&0xff) | 0x80;
	SCON = 0x40;
	REN = 0;			//ֻ���Ͳ���
	
	ES = 1;
}


//һ���Է���ģʽ

extern	unsigned char 	xdata key_count[MAX_KEY*2];

void UART_Txd_TKDEBUG(void)
{		
	unsigned char i,j,k,x;
	unsigned int sample,base;


#ifdef	TEST_HIGH_DATA	
	#pragma message "���ڷ��͸�Ƶֵ"				
	if( freqForDataMsg != MAX_KEY) //12��ʾ��Ƶ
	{
		return;
	}	
#endif

#ifdef	TEST_LOW_DATA	
	#pragma message "���ڷ��͵�Ƶֵ"				
	if( freqForDataMsg != 0) //12��ʾ��Ƶ
	{
		return;
	}	
#endif


	if(gbUartTran == 0)
	{
		x = 0;
		gBUartBuf[x++] = 0x69;

	//	gBUartBuf[x++] = ((KEY_NUMBER  )<<3)+1;		//1.����ģʽ
		gBUartBuf[x++] = ((KEY_NUMBER+1)<<3)+1;		//2.����һ����ͨ��������ʱ��
	//	for(k = 0;k< KEY_NUMBER;   k++)				//1.����ģʽ
		for(k = 0;k<(KEY_NUMBER+1);k++)				//2.����һ����ͨ��������ʱ��
		{		
			if(k==KEY_NUMBER)
			{
				sample =key_value;					//��ǰ������һ������������һ��Ƶ��
				base = 	freqForKeyJudge;				//��ǰ���ĸ�Ƶ��ȷ�ϰ���		
			}	
			else
			{
				sample = key_RawAd[k+freqForDataMsg] ;		 // ����ԭʼ����
			//	sample = key_pre[k+freqForDataMsg]  ;
				base   = key_baseline[k+freqForDataMsg]  ;  //������׼ֵ							
			//	sample = base   = key_RawTouch[k+freqForDataMsg];
			//	base   = key_count[k+freqForDataMsg]  ;  //������׼ֵ						
				if(base==0)
				{
					sample=0;
				}	
			}

			gBUartBuf[x++] = 0x02;
			gBUartBuf[x++] = k;		
			gBUartBuf[x++] = (base>>8)&0xff;	
			gBUartBuf[x++] = (base)&0xff;
					
			gBUartBuf[x++] = 0x12;
			gBUartBuf[x++] = k;		
			gBUartBuf[x++] = (sample>>8)&0xff;		
			gBUartBuf[x++] = (sample)&0xff;
		}
					
		for(i=0,j=0;i<x;i++)
		{
			j+= gBUartBuf[i];
		}
		gBUartBuf[x++] = j;
		gBUartLen = 0;
		gBUartTotalByte = x;
					
		gbUartTran = 1;
		
		TI = 1;
	}
}

#endif