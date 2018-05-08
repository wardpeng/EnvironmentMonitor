#ifndef LIGHT_H
#define LIGHT_H

#include "Arduino.h"
#include <Wire.h>

int BH1750_Read(int address);
uint16_t readLight();
void BH1750_Init(int address);
#endif
