#ifndef BME280_H
#define BME280_H

#include "Arduino.h"
#include <Wire.h>

void initBME280();
float getPressure();
float getTemperatureBME280();

#endif
