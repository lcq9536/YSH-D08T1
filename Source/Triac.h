#ifndef TRIAC_H__
#define TRIAC_H__

#define SCR P3_6

extern unsigned char On_Time;
extern unsigned char Off_Time;
extern unsigned char Skip_Cycles;
extern unsigned char Total_Cycles;

void Triac_SkipCycles(unsigned char On_Time,unsigned char Off_Time,
	unsigned char Skip_Cycles, unsigned char Total_Cycles);
void TriacSet(unsigned char OnTime,unsigned char OffTime,
	unsigned char SkipCycles, unsigned char TotalCycles);
#endif