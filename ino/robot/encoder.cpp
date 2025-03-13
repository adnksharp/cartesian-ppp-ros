#include "encoder.h"
#include "uros.h"

UROS* rosGI = nullptr;
Encoder* encoderGI = nullptr;

void globalEncoderInterrupt() 
{
	if (encoderGI != nullptr)
		encoderGI->read();
    
}

void Encoder::read() 
{
	if (digitalRead(phaseA) == digitalRead(phaseB))
		count++;
	else
		count--;

	int32_t data[3] = {id, count, millis()};
	rosGI->spin_publisher_multi_i32(data, 3, rosGI->encoder);
}

void Encoder::init(int encA, int encB, int encID) 
{
	phaseA = encA;
	phaseB = encB;
	id = encID;
	count = 0;
	pinMode(phaseA, INPUT);
	pinMode(phaseB, INPUT);
    
	encoderGI = this;

	attachInterrupt(digitalPinToInterrupt(phaseA), globalEncoderInterrupt, CHANGE);
}
