#include "GP2Y10.h"

int measurePin = A5;
int ledPower = 12;

const unsigned int samplingTime = 280;
const unsigned int deltaTime = 40;
const unsigned int sleepTime = 9680;

void initGP2Y10()
{
	pinMode(ledPower, OUTPUT);
}

float getDustDensity()
{
	float voMeasured = 0;
	float calcVoltage = 0;
	float dustDensity = 0;

	digitalWrite(ledPower, LOW);
	delayMicroseconds(samplingTime);

	voMeasured = analogRead(measurePin);

	delayMicroseconds(deltaTime);
	digitalWrite(ledPower, HIGH);
	delayMicroseconds(sleepTime);

	calcVoltage = voMeasured * (5.0 / 1024);
	dustDensity = 0.17 * calcVoltage - 0.1;

	if (dustDensity < 0)
	{
		dustDensity = 0.00;
	}

//	  Serial.println("Raw Signal Value (0-1023):");
//	  Serial.println(voMeasured);
//
//	  Serial.println("Voltage:");
//	  Serial.println(calcVoltage);
//
//	  Serial.println("Dust Density:");
//	  Serial.println(dustDensity);

	return dustDensity;
}

