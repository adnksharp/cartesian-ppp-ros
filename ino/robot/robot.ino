#include "uros.h"
#include "encoder.h"

extern int LED;
extern int ENCODER_A;
extern int ENCODER_B;
extern char *NODE_NAME;
extern char *ENCODER_TOPIC;

// uROS object
UROS ros;
Encoder encoder;

void EncoderRead()
{
	if (digitalRead(encoder.phaseA) == digitalRead(encoder.phaseB))
		encoder.count++;
	else
		encoder.count--;
	int32_t data[3] = {encoder.id, encoder.count, millis()};
	ros.spin_publisher_multi_i32(data, 2, ros.encoder);
}

void setup() 
{
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
	
	// initialize uROS
	ros.begin();
	// make uROS topics
	//ros.create_publisher(PUB_TOPIC);
	//ros.create_subscriber(SUB_TOPIC);
	ros.create_multi_i32(ENCODER_TOPIC, 3, ros.encoder);
	
	// initialize encoder
	encoder.init(ENCODER_A, ENCODER_B, 0);
	//attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoder.read, CHANGE);
	
	digitalWrite(LED, LOW);
}

void loop() 
{
	// get accel, gyro, temp from MPU6050 and publish them
	/*
	float mpu_data[3];
	impu.get_accel(mpu_data);
	ros.spin_publisher_multi_f32(mpu_data, 3, ros.mpu_accel);
	impu.get_gyro(mpu_data);
	ros.spin_publisher_multi_f32(mpu_data, 3, ros.mpu_gyro);
	ros.spin_publisher_f32(impu.get_temp(), ros.mpu_temp);
	*/
	//ros.spin_subscriber();

}
