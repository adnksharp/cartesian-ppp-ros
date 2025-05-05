#include "config.h"
#include "encoder.h"
#include "times.h"
#include "leds.h"
#include "uros.h"

extern byte LED[6];
extern byte MOTOR[3][2];
extern byte ENCODER[3][2];

LEDs led;
UROS ros;
Motor motor;
Ticker alerts;
Ticker calls;

extern void M1Update();
extern void M2Update();
extern void M3Update();

extern void err(bool rgb, bool fail);

void T1encoder()
{
	for (int i = 0; i < 3; i++)
	{
		motor.vel[i] = (motor.pos[i] - motor.cache[i]) / (enc_ms / 1000.0);
		motor.cache[i] = motor.pos[i];
	}
}

void CheckROS()
{
	RCCHECK(rclc_executor_spin_some(&ros.executor, RCL_MS_TO_NS(timeout)));
}

void subscription_callback(const void * msgin)
{
	const std_msgs__msg__Int16MultiArray * msg = (const std_msgs__msg__Int16MultiArray *)msgin;
	for (int i = 0; i < 3; i++)
	{
		short val = msg->data.data[i];
		motor.run(i, val);
	}
}

void setup() 
{
	Serial.begin(115200);
	
	led.begin(LED);
	ros.begin(NODE_NAME);
	ros.make_multi_int16(TOPIC_ENCODER_POS, 3, ros.enc_pos_pub);
	ros.make_multi_int16(TOPIC_ENCODER_VEL, 3, ros.enc_vel_pub);
	ros.echo_multi_int16(TOPIC_MOTOR_VEL, ros.motor_vel_sub, ros.multi_int16_msg);

	RCCHECK(rclc_executor_init(&ros.executor, &ros.support.context, 1, &ros.allocator));
	RCCHECK(rclc_executor_add_subscription(&ros.executor, &ros.motor_vel_sub, &ros.multi_int16_msg, &subscription_callback, ON_NEW_DATA));
	
	motor.init(ENCODER, MOTOR);
	alerts.attach_ms(enc_ms, T1encoder);
	calls.attach_ms(watchdog, CheckROS);
}

void loop()
{
	int16_t pos[3] = {motor.pos[0], motor.pos[1], motor.pos[2]};
	int16_t vel[3] = {motor.vel[0], motor.vel[1], motor.vel[2]};
	ros.post_multi_int16(pos, 3, ros.enc_pos_pub);
	ros.post_multi_int16(vel, 3, ros.enc_vel_pub);
}
