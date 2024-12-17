#ifndef WORK_H__
#define WORK_H__

#include <SH79F3212.h>
#include "MCU.h"

extern unsigned char Sys_Mode;

extern bool NTCShort;
extern bool NTCOpen;
extern bool DryBoil;

void StandbyMode();
void CheckMode();

#endif