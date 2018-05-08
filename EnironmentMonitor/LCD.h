#ifndef LCD_H
#define LCD_H

#include "Arduino.h"
#define LCD_SERIAL Serial1

//For log display
#define LOGO_HZ_INDEX 0
#define LOGO_X 5
#define LOGO_Y 10

//For count display
#define COUNT_HZ_INDEX 2
#define COUNT_X 600
#define COUNT_Y 30

//For time display
#define TIME_HZ_INDEX 1
#define TIME_X 5
#define TIME_Y 60

//For Oxy display
#define OXY_HZ_INDEX 3
#define OXY_X 5
#define OXY_Y 110

//For Temperature display
#define TEMP_HZ_INDEX 5
#define TEMP_X 5
#define TEMP_Y 160

//For huminity display
#define HUMI_HZ_INDEX 7
#define HUMI_X 5
#define HUMI_Y 210

//For Light display
#define LIGHT_HZ_INDEX 9
#define LIGHT_X 5
#define LIGHT_Y 260

//For VOC display
#define VOC_HZ_INDEX 11
#define VOC_X 5
#define VOC_Y 310

//For BME display
#define BME_HZ_INDEX 16
#define BME_X 5
#define BME_Y 360

//For HCHO security note display
#define NOTE_HZ_INDEX 18
#define NOTE_X 450
#define NOTE_Y 135

void clearDisp(void);
void LcdDisplay(int posX, int posY, int size, char *d, int iColor);
void LcdDisplay(int iPosX, int iPosY, int iSize, String strDisp, int iColor);
void setLcdDirection();

#endif
