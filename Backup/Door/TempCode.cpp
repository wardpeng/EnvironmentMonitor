#include "TempCode.h"


//根据输入的密码，判断是否合法
boolean LegaAccess(String strCode)
{
  int i = 0;
  for(i = 0; i < PasswordNum; i++)
  {
    if(strCode.indexOf(tempCode[i])!=-1)
       break;
  }
  if(i<12)
    return true;
  else 
    return false;


}



