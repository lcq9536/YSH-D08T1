#ifndef NTC_H__
#define NTC_H__

extern unsigned int AD_Value;
extern unsigned int Temperature;

unsigned int ADC_Read();
void AD_Deal(unsigned int Temp);
unsigned int AD_To_Temperature(unsigned int Value);
void Temperature_Deal(unsigned int Value);

#endif