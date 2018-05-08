#include "Light.h"

int BH1750address = 0x23; //芯片地址为16位23
byte lightBuff[2];


///////////////////////////////////////////////////////////////////////////
// get data from light sensor
// you can replace this code for your sensor
uint16_t readLight()
{
  uint16_t val = 0;
  BH1750_Init(BH1750address);
  delay(200);
  if (2 == BH1750_Read(BH1750address))
  {
    val = ((lightBuff[0] << 8) | lightBuff[1]) / 1.2;
  }

//  Serial.print("Sensor value is: ");
//  Serial.println((int)val);

  return val;
}

int BH1750_Read(int address) //
{
  int i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while (Wire.available()) //
  {
    lightBuff[i] = Wire.read();  // receive one byte
    i++;
  }
  Wire.endTransmission();
  return i;
}

void BH1750_Init(int address)
{
  Wire.beginTransmission(address);
  Wire.write(0x10);//1lx reolution 120ms
  Wire.endTransmission();
}
