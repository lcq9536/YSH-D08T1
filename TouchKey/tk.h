#ifndef	_TK_H
#define	_TK_H
//===============================================================================================
//								 			触摸库 V2
//							-----------------------------
//										触摸按键子程序接口
//
//===============================================================================================

#define	MCU_SH79F3212_SERIES	//3212系列MCU				(修改需要更换3212对应的LIB)	
//#define	MCU_SH79F326_SERIES						//326系列MCU				(修改需要更换326对应的LIB)
#define	MAX_KEY	8          //最大支持多少个按键		 (修改需要更换对应的LIB)

//打***可以改

//封装:
#define	SH79F3212_20PIN	0
#define	SH79F3212_28PIN	1


#define	PACKAGE	SH79F3212_28PIN			//***



  #define	TK_SCAN_PERIOD	12
//调试
//#define	TEST_TK_DATA									 //(***)
//二选一
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
//								配置
//-------------------------------------------------------------------------------------
//需要手工填的东西
extern	unsigned char	xdata KEY_NUMBER;					//按键个数(用户设置)
extern	unsigned char	xdata SHARE_NUMBER;					//复用个数
extern	unsigned char	xdata FLAG_COUNT;      	        	//按键确认次数
extern	unsigned char	xdata HF_VALUE;						//高频
extern	unsigned char	xdata LF_VALUE;						//低频
extern	unsigned char	code  HF_DIV4[MAX_KEY];				//高频放大倍数
extern	unsigned char	code  LF_DIV4[MAX_KEY];				//低频放大倍数
extern	unsigned char	code  Tab_TKChn[MAX_KEY];
extern	unsigned int	code  Tab_TKMax[MAX_KEY*2];
extern	unsigned char	xdata	gBPatch1;					
extern	unsigned char	xdata	gBPatch2;
extern	unsigned char	xdata	gBPatch3;
extern	unsigned char	xdata	gBTouchReason;

//-------------------------------------------------------------------------------------
//								应用程序接口
//-------------------------------------------------------------------------------------
//接口
extern	void InitKey(void);			//初始化
extern	void stop_scan_tk(void);		//一个扫描周期开始的时候调用一次
extern	void stop_scan_tk_1(void);		//开始扫描LED的时候调用一次(仅针对通道与LED复用的情况)
extern	unsigned char SanTk(void); 	//在一个周期内至少调用N次 (N=触摸按键个数)

//OTA中断接口
extern	void TK_Isr (void);			//void TK_Interrupt (void) interrupt  X

//结果
extern	unsigned int 	xdata key_value;      		//返回按键值(原始结果)
//extern	unsigned int	Key_DataInput(void);	//返回按键值(带无效按键判断功能)
extern	unsigned int	catchKey(void);				//返回按键值(带防卡键功能)

//手动重新适应环境补丁
extern	void reset_TK(void);						//note1:(如发现按键按下后一直没有释放，可以调用一次)
	
//调试用
#ifdef	TEST_TK_DATA
	extern	unsigned char xdata	gBUartSendFg;
	extern	void UART_INIT(void);
	extern	void UART_Txd_TKDEBUG(void);	 
#endif

extern void tkRound(void);

#endif
