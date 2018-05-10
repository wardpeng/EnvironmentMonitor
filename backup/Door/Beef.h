#ifndef BEEF_H
#define BEEF_H
#include "Arduino.h"

#define pinBeef 3  // D3

void Di(void);  //短促响铃，用于按键点下音效
void Diii(void);  //长音效，用于开门成功音效
void DiDi(void);  //取消音效
void DiDiDi(void);  //拒绝音效
void Di3s(void);  //呼叫时，模拟超长音效

#endif
