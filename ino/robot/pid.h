#include <Arduino.h>

struct PID {
	int sample = 100;
	double Kp[3], Ki[3], Kd[3],
		KpError[3], KiError[3], KdError[3],
		CacheError[3];
	float output[3],
		PPMM[3],
		setpoint[3];
	float outMin, outMax;
	int minPWM[3], PWM[3];
	void begin(double kp[3], double ki[3], double kd[3], float ppmm[3], int min[3]);
};
