#include <Arduino.h>
#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int16_multi_array.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){err(true, true);}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

struct UROS 
{
    rcl_subscription_t topic_sub;
    rcl_publisher_t enc_pos_pub, topic_pub;
	
	std_msgs__msg__Int16MultiArray multi_int16_msg;

    rclc_executor_t executor;
    rclc_support_t support;
    rcl_allocator_t allocator;
    rcl_node_t node;
    rcl_timer_t timer;

    void begin(const char *NODE_NAME);
    
	void make_multi_int16(const char * topic_name, size_t size, rcl_publisher_t & publisher_multi_int16);
	void echo_multi_int16(const char * topic_name, rcl_subscription_t & subscription_multi_int16, std_msgs__msg__Int16MultiArray & msg);

	void post_multi_int16(int16_t * data, size_t size, rcl_publisher_t & publisher_multi_int16);
};
