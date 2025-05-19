#include <Ticker.h>
//Hardware config
const byte led[4] = {11, 12, 13, 14};
const byte motor[3][2] = {
	{17,18}, // 190
	{10, 9}, // 164
	{8, 3}
}, encoder[3][2] = {
	{5, 4},
	{6, 7},
	{16, 15}
};

//Encoder
volatile long position[3] = {0, 0, 0};
float setpoint[3] = {0, 0, 0};

//PID 
Ticker PIDTicker;
int sampleTime = 100;
double Kp[3] = {0.375, 0.375, 0.375},
		Ki[3] = {0.0974, 0.0974, 0.0974},
		Kd[3] = {0.01, 0.01, 0.01},
		KpError[3] = {0, 0, 0},
		KiError[3] = {0, 0, 0},
		KdError[3] = {0, 0, 0},
		CacheError[3] = {0, 0, 0},
		output[3] = {0, 0, 0};
float outMin[3] = {-255, -255, -255},
		outMax[3] = {255, 255, 255}, //+813, -970
		PPMM[3] = {12.1, 12.1, 118.7}; // pulse per millimeters
int PWM[3] = {0, 0, 0},
	minPWM[3] = {98, 102, 39},
	PPR[3] = {22, 22, 22}; // pulse per revolution
		//mPr[3] = {8, 8, 8}; // milimeters per revolution

void PID()
{
	for(byte i = 0; i < 3; i++)
	{
		double pos, error;
		pos = float(position[i]) / PPMM[i];
		error = setpoint[i] - pos;
		KpError[i] = Kp[i] * error;
		KiError[i] += Ki[i] * error * (sampleTime / 1000.0);
		KiError[i] = constrain(KiError[i], outMin[i], outMax[i]);
		KdError[i] = Kd[i] * (error - CacheError[i]) / (sampleTime / 1000.0);
		CacheError[i] = error;

		output[i] = KpError[i] + KiError[i] + KdError[i];
		output[i] = constrain(output[i], outMin[i], outMax[i]);

		if(output[i] > outMax[i])
			output[i] = outMax[i];
		else if(output[i] < outMin[i])
			output[i] = outMin[i];

		PWM[i] = output[i];
		if(PWM[i] > 0 && PWM[i] < minPWM[i])
			PWM[i] += minPWM[i];
		else if(PWM[i] < 0 && PWM[i] > -minPWM[i])
			PWM[i] -= minPWM[i];
		analogWrite(motor[i][0], PWM[i] > 0 ? PWM[i] : 0);
		analogWrite(motor[i][1], PWM[i] < 0 ? -PWM[i] : 0);
		digitalWrite(led[i], PWM[i] != 0);
	}
}

void encoderA()
{
	if(digitalRead(encoder[0][0]) == digitalRead(encoder[0][1]))
		position[0]++;
	else
		position[0]--;
}

void encoderB()
{
	if(digitalRead(encoder[1][0]) == digitalRead(encoder[1][1]))
		position[1]++;
	else
		position[1]--;
}

void encoderC()
{
	if(digitalRead(encoder[2][0]) == digitalRead(encoder[2][1]))
		position[2]++;
	else
		position[2]--;
}

void serialEvent()
{
	if (Serial.available() > 0)
	{
		String command = Serial.readStringUntil('\n');
		int separatorIndex = command.indexOf(':');
		if (separatorIndex != -1)
		{
			int motorIndex = command.substring(0, separatorIndex).toInt();
			int value = command.substring(separatorIndex + 1).toInt();
			Serial.println("Motor: " + String(motorIndex) + ", Value: " + String(value));
			if (motorIndex >= 0 && motorIndex < 3)
				setpoint[motorIndex] = value;
			else
				Serial.println("Invalid motor index");
		}
		else
		{
			Serial.println("Invalid command format");
		}
	}
}

void setup()
{
	Serial.begin(115200);

	for(size_t i = 0; i < sizeof(encoder) / sizeof(encoder[0]); i++)
		for(byte j = 0; j < 2; j++)
			pinMode(encoder[i][j], INPUT);
	for(size_t i = 0; i < sizeof(motor) / sizeof(motor[0]); i++)
		for(byte j = 0; j < 2; j++)
			pinMode(motor[i][j], OUTPUT);
	for(byte i: led)
		pinMode(i, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(encoder[0][0]), encoderA, CHANGE);
	attachInterrupt(digitalPinToInterrupt(encoder[1][0]), encoderB, CHANGE);
	attachInterrupt(digitalPinToInterrupt(encoder[2][0]), encoderC, CHANGE);

	PIDTicker.attach_ms(sampleTime, PID);
	digitalWrite(RGB_BUILTIN, LOW);
}

void loop()
{
	Serial.println(
		"[ " + String(setpoint[0]) +
		" | " + String(position[0] / PPMM[0]) + " | " +
		String(PWM[0]) + " ] [ " + String(setpoint[1]) +
		" | " + String(position[1] / PPMM[1]) + " | " +
		String(PWM[1]) + " ] [ " + String(setpoint[2]) +
		" | " + String(position[2] / PPMM[2]) + " | " +
		String(PWM[2]) + " ]"
	);
	delay(50);
}
