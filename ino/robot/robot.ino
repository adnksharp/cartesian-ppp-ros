#include "config.h"
#include "encoder.h"
#include "pid.h"
#include "times.h"
#include "leds.h"
#include "uros.h"

extern byte LED[6];
extern byte MOTOR[3][2];
extern byte ENCODER[3][2];

LEDs led;
UROS ros;
Motor motor;
PID pid;
Ticker PIDTicker;

extern void calc();
extern void M1Update();
extern void M2Update();
extern void M3Update();
extern void err(bool rgb, bool fail);

void subscription_callback(const void * msgin)
{
	const std_msgs__msg__Int16MultiArray * msg = (const std_msgs__msg__Int16MultiArray *)msgin;
	if(int(pid.output[0] + pid.output[1] + pid.output[2]) != 0)
		return;
	for (int i = 0; i < 3; i++)
	{
		float val = msg->data.data[i];
		pid.setpoint[i] = val;
	}
}

void setup() 
{
	led.begin(LED);
	led.set(0, HIGH);

	ros.begin(NODE_NAME);
	/*
		/yoba/sensiact/info
		pos[0],
		pos[1],
		pos[2],
		err,
	*/
	led.set(1, HIGH);
	ros.make_multi_int16(TOPIC_PUB, 4, ros.topic_pub);
	led.set(2, HIGH);
	/*
		/yoba/sensiact/cmd_pos
		sp[0],
		sp[1],
		sp[2],
		sp[3]
	*/
	ros.echo_multi_int16(TOPIC_ECHO, ros.topic_sub, ros.multi_int16_msg);

	RCCHECK(rclc_executor_init(&ros.executor, &ros.support.context, 1, &ros.allocator));
	RCCHECK(rclc_executor_add_subscription(&ros.executor, &ros.topic_sub, &ros.multi_int16_msg, &subscription_callback, ON_NEW_DATA));

	led.set(0, LOW);
	led.set(1, LOW);
	led.set(2, LOW);
	motor.begin(ENCODER, MOTOR);
	pid.begin(kp, ki, kd, PPMM, motorMin);
	PIDTicker.attach_ms(pid.sample, calc);
}

void loop()
{
	led.set(3, HIGH);
	int16_t post[4] = {
		int(motor.pos[0] / pid.PPMM[0]),
		int(motor.pos[1] / pid.PPMM[1]),
		int(motor.pos[2] / pid.PPMM[2]),
		int(pid.output[0] + pid.output[1] + pid.output[2])
	};
	ros.post_multi_int16(post, 4, ros.topic_pub);
	RCCHECK(rclc_executor_spin_some(&ros.executor, RCL_MS_TO_NS(timeout)));
	led.set(3, LOW);
	delay(150);
}
