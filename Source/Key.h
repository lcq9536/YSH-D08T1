#ifndef KEY_H__
#define KEY_H__

extern unsigned int KeyRead;
extern unsigned int KeyValue;
extern unsigned char KeyCount;
extern unsigned int Last_KeyCode;

void Get_KeyValue(unsigned int Key_Read);
void Key_Handle();

#endif