#include <Arduino.h>

void err(bool rgb)
{
	while (!rgb)
	{
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		delay(50);
	}
}
