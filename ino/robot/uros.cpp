#include "uros.h"
#include "utils.h"

void UROS::begin(const char *NODE_NAME)
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

void UROS::make_int8(const char * topic_name, rcl_publisher_t & publisher_int8)
{
	std_msgs__msg__Int8 msg;
	RCCHECK(rclc_publisher_init_default(
		&publisher_int8,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8),
		topic_name
	));
}

void UROS::make_int16(const char * topic_name, rcl_publisher_t & publisher_int16)
{
	std_msgs__msg__Int16 msg;
	RCCHECK(rclc_publisher_init_default(
		&publisher_int16,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16),
		topic_name
	));
}

void UROS::make_multi_int8(const char * topic_name, size_t size, rcl_publisher_t & publisher_multi_int8)
{
	std_msgs__msg__Int8MultiArray msg;
	RCCHECK(rclc_publisher_init_default(
		&publisher_multi_int8,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8MultiArray),
		topic_name
	));

	std_msgs__msg__Int8MultiArray__init(&multi_int8_msg);
	multi_int8_msg.data.capacity = size;
	multi_int8_msg.data.data = (int8_t *) allocator.allocate(size * sizeof(int8_t), allocator.state);
}

void UROS::make_multi_int16(const char * topic_name, size_t size, rcl_publisher_t & publisher_multi_int16)
{
	std_msgs__msg__Int16MultiArray msg;
	RCCHECK(rclc_publisher_init_default(
		&publisher_multi_int16,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16MultiArray),
		topic_name
	));

	std_msgs__msg__Int16MultiArray__init(&multi_int16_msg);
	multi_int16_msg.data.capacity = size;
	multi_int16_msg.data.data = (int16_t *) allocator.allocate(size * sizeof(int16_t), allocator.state);
}

void UROS::post_int8(int8_t data, rcl_publisher_t & publisher_int8)
{
	std_msgs__msg__Int8 msg;
	msg.data = data;
	RCSOFTCHECK(rcl_publish(&publisher_int8, &msg, NULL));
}

void UROS::post_int16(int16_t data, rcl_publisher_t & publisher_int16)
{
	std_msgs__msg__Int16 msg;
	msg.data = data;
	RCSOFTCHECK(rcl_publish(&publisher_int16, &msg, NULL));
}

void UROS::post_multi_int8(int8_t * data, size_t size, rcl_publisher_t & publisher_multi_int8)
{
	multi_int8_msg.data.size = size;
	for (size_t i = 0; i < size; i++)
		multi_int8_msg.data.data[i] = data[i];
	RCSOFTCHECK(rcl_publish(&publisher_multi_int8, &multi_int8_msg, NULL));
}

void UROS::post_multi_int16(int16_t * data, size_t size, rcl_publisher_t & publisher_multi_int16)
{
	multi_int16_msg.data.size = size;
	for (size_t i = 0; i < size; i++)
		multi_int16_msg.data.data[i] = data[i];
	RCSOFTCHECK(rcl_publish(&publisher_multi_int16, &multi_int16_msg, NULL));
}
