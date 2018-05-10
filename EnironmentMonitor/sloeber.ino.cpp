#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-05-09 12:51:58

#include "Arduino.h"
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include "DS18B20_S.h"
#include <DS3231.h>
#include <SoftwareSerial.h>
#include <dht11.h>
#include "C:\Users\I327500\MyWork_PXQ\other project\OurHome\EnvironmentMonitor\EnironmentMonitor\hz\hz.c"
#include "LCD.h"
#include "Light.h"
#include "AQM300.h"
#include "BME280.h"
#include "GP2Y10.h"

void setup() ;
void loop() ;
String displayAndGetStoreageString(unsigned long int storeageCounter) ;
String getTime() ;
String getDateFlieName(void)   ;
void writeToSD(char *databuf, char *filename) ;
void writeToSD(String str, char *filename) ;
float Get18B20(void) ;
float GetPressure(void) ;
float GetHumidity(void)   ;
float GetTemperature(void)   ;
float getOxymethylene(void)   ;
void printDirectory(File dir, int numTabs) ;
void setClock() ;
void getAllInfo(char *buf) ;


#include "EnironmentMonitor.ino"

#endif
