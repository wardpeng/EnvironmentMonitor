#include "GP2Y10.h"


long lastConnectionTime = 0;        // last time you connected to the server, in milliseconds
boolean lastConnected = false;      // state of the connection last time through the main loop
const int postingInterval = 10000;  //delay between updates to Pachube.com


char s[32];

float dustdensity = 0;//颗粒浓度（毫克/立方米）
float ppmpercf = 0;

void initGP2Y10()
{
  pinMode(ledPower,OUTPUT);
}

float getDustDensity()
{
  int i =0;
  float ppm=0;
  float voltage = 0;
  int dustVal=0;
  while(i<10)//采样10次，每次耗时10ms=280+40+9680
  {
    i=i+1;
    digitalWrite(ledPower,LOW); // power on the LED
    delayMicroseconds(280);//280us
    dustVal=analogRead(dustPin); // read the dust value
    ppm = ppm+dustVal;          //sum
    delayMicroseconds(40);  //40us
    digitalWrite(ledPower,HIGH); // turn the LED off
    delayMicroseconds(9680);//9680um
 }
 
    voltage = ppm/10*0.0049;  //average =sum/10
    dustdensity = 0.17*voltage-0.1;
    ppmpercf = (voltage-0.0256)*120000;
    
    if (ppmpercf < 0)
      ppmpercf = 0;
    if (dustdensity < 0 )
      dustdensity = 0;
   return dustdensity*1000;
}
float getPpmpercf(void)
{
  return ppmpercf;
}
