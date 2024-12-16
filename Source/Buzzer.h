#ifndef BUZZER_H__
#define BUZZER_H__

#define BUZZER P3_3

extern unsigned char 	Buzzer_State;
extern unsigned char 	Buzzer_Times;
extern unsigned int 	Buzzer_Interval;
extern unsigned int 	Buzzer_Duration;

void Buzzer(unsigned char beep_times,unsigned int interval,unsigned int duration);
void BuzzerSet(unsigned char beep_times,unsigned int interval,unsigned int duration);

#endif