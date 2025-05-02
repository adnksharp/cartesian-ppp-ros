#include <Arduino.h>

extern byte LED[6];

void err(bool rgb)
{
	while (!rgb)
	{
		digitalWrite(LED[4], !digitalRead(LED[4]));
		delay(50);
	}
}
