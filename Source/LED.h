#ifndef LED_H__
#define LED_H__

#include <SH79F3212.h>

#define LED2 	P2_3
#define LED3 	P2_2
#define LED4 	P1_1
#define LED5 	P0_0
#define LED6 	P2_1
#define LED7 	P1_0

#define COM0 	P1_2

extern unsigned char 	LED_State;
extern unsigned int 	LED_Value;
extern unsigned int 	LED_Time;

void Led_Display(unsigned char State,unsigned int Value,unsigned int Time);
void LedSet(unsigned char State,unsigned int Value,unsigned int Time);

#endif