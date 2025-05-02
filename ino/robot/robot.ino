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

extern void M1Update();
extern void M2Update();
extern void M3Update();

void T1encoder()
{
	for (int i = 0; i < 3; i++)
	{
		motor.vel[i] = (motor.pos[i] - motor.cache[i]) / (enc_ms / 1000.0);
		motor.cache[i] = motor.pos[i];
		led.set(LED[i], motor.vel[i] != 0);
	}
}

void setup() 
{
	Serial.begin(115200);
	
	led.begin(LED);
	ros.begin(NODE_NAME);
	ros.make_multi_int16(TOPIC_ENCODER_POS, 3, ros.enc1_pos_pub);
	ros.make_multi_int16(TOPIC_ENCODER_VEL, 3, ros.enc1_vel_pub);
	
	motor.init(ENCODER, MOTOR);
	alerts.attach_ms(enc_ms, T1encoder);
}

void loop()
{
	led.set(LED[3], HIGH);
	int16_t pos[3] = {motor.pos[0], motor.pos[1], motor.pos[2]};
	int16_t vel[3] = {motor.vel[0], motor.vel[1], motor.vel[2]};
	ros.post_multi_int16(pos, 3, ros.enc1_pos_pub);
	ros.post_multi_int16(vel, 3, ros.enc1_vel_pub);
	led.set(LED[3], LOW);
}
