#include <Arduino.h>
#include <Ticker.h>

struct Timers {
	double enc_vel[3];
	volatile int enc_pos[3];
	int enc_cache[3];
	int enc_ms = 50;
	//void encoder();
};
