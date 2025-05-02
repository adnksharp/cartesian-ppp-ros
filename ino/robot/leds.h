#include <Arduino.h>

struct LEDs {
	byte led[6];
	void begin(byte PINS[6])
	{
		byte show = 20;
		for (int i = 0; i < 6; i++)
		{
			led[i] = PINS[i];
			pinMode(led[i], OUTPUT);
			digitalWrite(led[i], HIGH);
			delay(show);
			digitalWrite(led[i], LOW);
		}
#ifdef RGB_BUILTIN
		for(byte i = 0; i < 4; i++)
		{
			neopixelWrite(led[5], i == 0 ? 255 : 0, i == 1 ? 255 : 0, i == 2 ? 255 : 0);
			delay(show);
		}
#endif
	}
	void set(byte led_num, byte state)
	{
		digitalWrite(led[led_num], state);
	}
	void all(byte state)
	{
		for (byte i: led)
			digitalWrite(i, state);
	}
	void rgb(byte r, byte g, byte b)
	{
#ifdef RGB_BUILTIN
		neopixelWrite(led[5], r, g, b);
#endif
	}
};
