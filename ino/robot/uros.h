#include <Arduino.h>
#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
//#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/int32_multi_array.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){err(LED);}}
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
    rcl_publisher_t encoder;
    std_msgs__msg__Int32MultiArray multi_i32_msg;
    int32_t* multi_i32_data;
    rclc_executor_t executor;
    rclc_support_t support;
    rcl_allocator_t allocator;
    rcl_node_t node;
    rcl_timer_t timer;
    size_t multi_i32_capacity;

    void begin();
    void create_multi_i32(const char * topic_name, size_t capacity, rcl_publisher_t & publisher_multi_i32);
    void spin_publisher_multi_i32(int32_t data[], size_t size, rcl_publisher_t & publisher_multi_i32);
    void spin_subscriber();
};

extern UROS uros;
