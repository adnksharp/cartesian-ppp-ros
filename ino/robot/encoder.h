#include <Arduino.h>

struct Motor {
	volatile int pos[3];
	float vel[3], cache[3];
	byte in[3][2], out[3][2];
	void init(byte encoder[3][2], byte motor[3][2]);
	void run(byte id, short speed);
};
