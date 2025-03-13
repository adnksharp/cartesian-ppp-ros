#include <Arduino.h>

void err(byte pin)
{
	while (1)
	{
		digitalWrite(pin, !digitalRead(pin));
		delay(50);
	}
}

