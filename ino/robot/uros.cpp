#include "uros.h"
#include "config.h"
#include "utils.h"

void UROS::begin() 
{
	#if __has_include("wificfg.h")
		#include "wificfg.h"
			set_microros_wifi_transports(ssid, pass, host, port);
	#else
		set_microros_transports();
	#endif

	allocator = rcl_get_default_allocator();
	rcl_init_options_t opts = rcl_get_zero_initialized_init_options();
	RCCHECK(rcl_init_options_init(&opts, allocator));
	RCCHECK(rcl_init_options_set_domain_id(&opts, DOMAIN_ID));
	RCCHECK(rclc_support_init_with_options(&support, 0, NULL, &opts, &allocator));

	RCCHECK(rclc_node_init_default(&node, NODE_NAME, "", &support));
}

void UROS::create_multi_i32(const char * topic_name, size_t capacity, rcl_publisher_t & publisher_multi_i32)
{
	std_msgs__msg__Int32MultiArray msg;
	RCCHECK(rclc_publisher_init_default(
		&publisher_multi_i32,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32MultiArray),
		topic_name
	));

	std_msgs__msg__Int32MultiArray__init(&multi_i32_msg);
	multi_i32_capacity = capacity;
	multi_i32_data = (int32_t *)malloc(sizeof(int32_t) * capacity);
	multi_i32_msg.data.capacity = capacity;
	multi_i32_msg.data.data = multi_i32_data;
}

void UROS::spin_publisher_multi_i32(int32_t data[], size_t size, rcl_publisher_t & publisher_multi_i32)
{
	if (size > multi_i32_capacity) {
		return;
	}

	multi_i32_msg.data.size = size;
	for (size_t i = 0; i < size; ++i) {
		multi_i32_data[i] = data[i];
	}

	RCSOFTCHECK(rcl_publish(&publisher_multi_i32, &multi_i32_msg, NULL));
}

void UROS::spin_subscriber() 
{
	RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}
