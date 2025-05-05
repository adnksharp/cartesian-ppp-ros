#include <Arduino.h>

extern byte LED[6];
byte red = 0;

void err(bool rgb, bool fail)
{
	red = 255 - red;
	if (rgb)
	{
		if (fail)
			neopixelWrite(LED[5], red, 0, 0);
		else
			neopixelWrite(LED[5], red, 0, red);
	}
	else
		digitalWrite(LED[5], !digitalRead(LED[5]));
}
