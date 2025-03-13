#include <Arduino.h>

struct Encoder {
	int phaseA;
	int phaseB;
	volatile long count;
	int id;
	void read();
	void init(int encA, int encB, int encID);
};

extern Encoder encoder;
