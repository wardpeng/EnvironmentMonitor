#include "AQM300.h"

void initVocSensor()
{
	pinMode(AQM_A_PIN, INPUT);
	pinMode(AQM_B_PIN, INPUT);
}

int getVoc()
{
	int a = digitalRead(AQM_A_PIN);
	int b = digitalRead(AQM_A_PIN);
	return 2 * a + b + 1;
}
