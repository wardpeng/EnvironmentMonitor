#ifndef GP2Y10_H
#define GP2Y10_H

#include "Arduino.h"

#define dustPin 15  //A15 Pin
#define ledPower 2

void initGP2Y10();
float getDustDensity();//  ug/m3
float getPpmpercf(void);

/**

空气质量指数PM2.5（单位：μg/m3）表示每立方米空气中可入肺颗粒物的含量，这个值越高，就代表空气污染越严重：
PM2.5
日均浓度	0～35	35～75	75～115	115～150	150～250	＞250
空气质量级别	一级	二级	三级	四级	五级	六级
空气质量类型	优	良	轻度污染	中度污染	重度污染	严重污染
 */
 /*
 Interface to Sharp GP2Y1010AU0F Particle Sensor
 Program by Christopher Nafis 
 Written April 2012
 
 http://www.sparkfun.com/datasheets/Sensors/gp2y1010au_e.pdf
 http://sensorapp.net/?p=479
 
 Sharp pin 1 (V-LED)   => 5V (connected to 150ohm resister)
 Sharp pin 2 (LED-GND) => Arduino GND pin
 Sharp pin 3 (LED)     => Arduino pin 2
 Sharp pin 4 (S-GND)   => Arduino GND pin
 Sharp pin 5 (Vo)      => Arduino A0 pin
 Sharp pin 6 (Vcc)     => 5V
 */


#endif
