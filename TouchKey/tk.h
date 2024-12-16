#ifndef	_TK_H
#define	_TK_H
//===============================================================================================
//								 			������ V2
//							-----------------------------
//										���������ӳ���ӿ�
//
//===============================================================================================

#define	MCU_SH79F3212_SERIES	//3212ϵ��MCU				(�޸���Ҫ����3212��Ӧ��LIB)	
//#define	MCU_SH79F326_SERIES						//326ϵ��MCU				(�޸���Ҫ����326��Ӧ��LIB)
#define	MAX_KEY	8          //���֧�ֶ��ٸ�����		 (�޸���Ҫ������Ӧ��LIB)

//��***���Ը�

//��װ:
#define	SH79F3212_20PIN	0
#define	SH79F3212_28PIN	1


#define	PACKAGE	SH79F3212_28PIN			//***



  #define	TK_SCAN_PERIOD	12
//����
//#define	TEST_TK_DATA									 //(***)
//��ѡһ
#define	TEST_HIGH_DATA									 //(***)
//#define	TEST_LOW_DATA								 //(***)


#define	bt0	0x01
#define	bt1	0x02
#define	bt2	0x04
#define	bt3	0x08
#define	bt4	0x10
#define	bt5	0x20
#define	bt6	0x40
#define	bt7	0x80

//-------------------------------------------------------------------------------------
//								����
//-------------------------------------------------------------------------------------
//��Ҫ�ֹ���Ķ���
extern	unsigned char	xdata KEY_NUMBER;					//��������(�û�����)
extern	unsigned char	xdata SHARE_NUMBER;					//���ø���
extern	unsigned char	xdata FLAG_COUNT;      	        	//����ȷ�ϴ���
extern	unsigned char	xdata HF_VALUE;						//��Ƶ
extern	unsigned char	xdata LF_VALUE;						//��Ƶ
extern	unsigned char	code  HF_DIV4[MAX_KEY];				//��Ƶ�Ŵ���
extern	unsigned char	code  LF_DIV4[MAX_KEY];				//��Ƶ�Ŵ���
extern	unsigned char	code  Tab_TKChn[MAX_KEY];
extern	unsigned int	code  Tab_TKMax[MAX_KEY*2];
extern	unsigned char	xdata	gBPatch1;					
extern	unsigned char	xdata	gBPatch2;
extern	unsigned char	xdata	gBPatch3;
extern	unsigned char	xdata	gBTouchReason;

//-------------------------------------------------------------------------------------
//								Ӧ�ó���ӿ�
//-------------------------------------------------------------------------------------
//�ӿ�
extern	void InitKey(void);			//��ʼ��
extern	void stop_scan_tk(void);		//һ��ɨ�����ڿ�ʼ��ʱ�����һ��
extern	void stop_scan_tk_1(void);		//��ʼɨ��LED��ʱ�����һ��(�����ͨ����LED���õ����)
extern	unsigned char SanTk(void); 	//��һ�����������ٵ���N�� (N=������������)

//OTA�жϽӿ�
extern	void TK_Isr (void);			//void TK_Interrupt (void) interrupt  X

//���
extern	unsigned int 	xdata key_value;      		//���ذ���ֵ(ԭʼ���)
//extern	unsigned int	Key_DataInput(void);	//���ذ���ֵ(����Ч�����жϹ���)
extern	unsigned int	catchKey(void);				//���ذ���ֵ(������������)

//�ֶ�������Ӧ��������
extern	void reset_TK(void);						//note1:(�緢�ְ������º�һֱû���ͷţ����Ե���һ��)
	
//������
#ifdef	TEST_TK_DATA
	extern	unsigned char xdata	gBUartSendFg;
	extern	void UART_INIT(void);
	extern	void UART_Txd_TKDEBUG(void);	 
#endif

extern void tkRound(void);

#endif
