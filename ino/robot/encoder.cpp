#include "encoder.h"

extern Motor motor;
extern const byte LED[4];
extern byte MOTOR[3][2];
extern byte ENCODER[3][2];

void M1Update()
{
	if (digitalRead(ENCODER[0][0]) == digitalRead(ENCODER[0][1]))
		motor.pos[0]++;
	else
		motor.pos[0]--;
}

void M2Update()
{
	if (digitalRead(ENCODER[1][0]) == digitalRead(ENCODER[1][1]))
		motor.pos[1]++;
	else
		motor.pos[1]--;
		
}

void M3Update()
{
	if (digitalRead(ENCODER[2][0]) == digitalRead(ENCODER[2][1]))
		motor.pos[2]++;
	else
		motor.pos[2]--;
}


void Motor::init(byte encoder[3][2], byte motor[3][2])
{
	for (byte i = 0; i < sizeof(encoder) / sizeof(encoder[0]); i++)
	{
		for (byte j = 0; j < sizeof(encoder[0]) / sizeof(encoder[0][0]); j++)
		{
			in[i][j] = encoder[i][j];
			out[i][j] = motor[i][j];
			pinMode(in[i][j], INPUT);
			pinMode(out[i][j], OUTPUT);
		}
	}
	attachInterrupt(digitalPinToInterrupt(in[0][0]), M1Update, CHANGE);
	attachInterrupt(digitalPinToInterrupt(in[1][0]), M2Update, CHANGE);
	attachInterrupt(digitalPinToInterrupt(in[2][0]), M3Update, CHANGE);
}

void Motor::run(byte id, short speed)
{
	if (id < sizeof(out) / sizeof(out[0]))
	{
		analogWrite(out[id][0], speed > 0 ? speed : 0);
		analogWrite(out[id][1], speed < 0 ? -speed : 0);
	}
}

