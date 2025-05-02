#include <Arduino.h>
#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int8_multi_array.h>
#include <std_msgs/msg/int16_multi_array.h>
#include <std_msgs/msg/int16.h>
#include <std_msgs/msg/int8.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){err(false);}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

/*
void subscription_callback(const void * msgin)
{
	const std_msgs__msg__Int32 * called = (const std_msgs__msg__Int32 *)msgin;
	digitalWrite(LED, called->data == 1);
}
*/

struct UROS 
{
    rcl_subscription_t subscriber;
    rcl_publisher_t enc1_pos_pub, enc2_pos_pub, enc3_pos_pub,
		enc1_vel_pub, enc2_vel_pub, enc3_vel_pub;
	
	std_msgs__msg__Int8MultiArray multi_int8_msg;
	std_msgs__msg__Int16MultiArray multi_int16_msg;

    rclc_executor_t executor;
    rclc_support_t support;
    rcl_allocator_t allocator;
    rcl_node_t node;
    rcl_timer_t timer;

    void begin(const char *NODE_NAME);
    
	void make_int8(const char * topic_name, rcl_publisher_t & publisher_int8);
	void make_int16(const char * topic_name, rcl_publisher_t & publisher_int16);
	void make_multi_int8(const char * topic_name, size_t size, rcl_publisher_t & publisher_multi_int8);
	void make_multi_int16(const char * topic_name, size_t size, rcl_publisher_t & publisher_multi_int16);

	void post_int8(int8_t data, rcl_publisher_t & publisher_int8);
	void post_int16(int16_t data, rcl_publisher_t & publisher_int16);
	void post_multi_int8(int8_t * data, size_t size, rcl_publisher_t & publisher_multi_int8);
	void post_multi_int16(int16_t * data, size_t size, rcl_publisher_t & publisher_multi_int16);
	//void spin_subscriber();
};

extern UROS uros;
