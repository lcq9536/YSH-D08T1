#ifndef DIGITAL_H__
#define DIGITAL_H__

#define SEGA	P0_0
#define SEGF	P2_3
#define SEGB	P2_4
#define SEGD	P1_0
#define SEGDP 	P1_1
#define SEGC 	P2_1
#define SEGG 	P2_2
#define SEGE 	P0_3

#define COM1 	P3_4
#define COM2 	P0_1
#define COM3 	P0_2

extern unsigned char Digital_Num1;
extern unsigned char Digital_Num2;
extern unsigned char Digital_Num3;

void Digital_Display(unsigned char Num1,unsigned char Num2,unsigned char Num3);
void DigitalSet(unsigned char Num1,unsigned char Num2,unsigned char Num3);

#endif