#include "pid.h"
#include "encoder.h"

extern Motor motor;
extern PID pid;

void calc()
{
	for (byte i = 0; i < 3; i++)
	{
		double pos, error;
		pos = float(motor.pos[i]) / pid.PPMM[i];
		error = pid.setpoint[i] - pos;
		
		pid.KpError[i] = pid.Kp[i] * error;
		pid.KiError[i] += pid.Ki[i] * error * (pid.sample / 1000.0);
		pid.KiError[i] = constrain(pid.KiError[i], pid.outMin, pid.outMax);
		pid.KdError[i] = pid.Kd[i] * (error - pid.CacheError[i]) / (pid.sample / 1000.0);
		pid.CacheError[i] = error;
		
		pid.output[i] = pid.KpError[i] + pid.KiError[i] + pid.KdError[i];
		pid.output[i] = constrain(pid.output[i], pid.outMin, pid.outMax);

		if(pid.output[i] < pid.outMin)
			pid.output[i] = pid.outMin;
		if(pid.output[i] > pid.outMax)
			pid.output[i] = pid.outMax;

		pid.PWM[i] = pid.output[i];
		if(pid.PWM[i] > 0 && pid.PWM[i] < pid.minPWM[i])
			pid.PWM[i] += pid.minPWM[i];
		else if(pid.PWM[i] < 0 && pid.PWM[i] > -pid.minPWM[i])
			pid.PWM[i] -= pid.minPWM[i];

		motor.set(i, pid.PWM[i]);
	}
}

void PID::begin(double kp[3], double ki[3], double kd[3], float ppmm[3], int min[3])
{
	outMin = -255;
	outMax = 255;
	for(byte i = 0; i < 3; i++)
	{
		Kp[i] = kp[i];
		Ki[i] = ki[i];
		Kd[i] = kd[i];
		PPMM[i] = ppmm[i];
		minPWM[i] = min[i];
	}
}
