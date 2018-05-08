#ifndef AQM300_H
#define AQM300_H

#include "Arduino.h"

#define AQM_A_PIN 5
#define AQM_B_PIN 4

//0-0.5ppm:A0B0
#define VOC_0_05_PPM 0
//0.5-1.5ppm:A0B1
#define VOC_05_15_PPM 1
//1.5-3.0ppm:A1B0
#define VOC_15_30_PPM 2
//3.0-  ppm:A1B1
#define VOC_30_00_PPM 3

void initVocSensor();
int getVoc();

#endif
