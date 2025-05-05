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

void UROS::make_uint8(const char * topic_name, rcl_publisher_t & publisher_uint8)
{
	std_msgs__msg__UInt8 msg;
	RCCHECK(rclc_publisher_init_default(
		&publisher_uint8,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt8),
		topic_name
	));
}

void UROS::echo_uint8(const char * topic_name, rcl_subscription_t & subscription_uint8, std_msgs__msg__UInt8 & msg)
{
	RCCHECK(rclc_subscription_init_default(
		&subscription_uint8,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt8),
		topic_name
	));
	std_msgs__msg__UInt8__init(&msg);
}

void UROS::make_multi_uint8(const char * topic_name, size_t size, rcl_publisher_t & publisher_multi_uint8)
{
	std_msgs__msg__UInt8MultiArray msg;
	RCCHECK(rclc_publisher_init_default(
		&publisher_multi_uint8,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt8MultiArray),
		topic_name
	));

	std_msgs__msg__UInt8MultiArray__init(&multi_uint8_msg);
	multi_uint8_msg.data.capacity = size;
	multi_uint8_msg.data.data = (uint8_t *) allocator.allocate(size * sizeof(uint8_t), allocator.state);
}

void UROS::echo_multi_uint8(const char * topic_name, rcl_subscription_t & subscription_multi_uint8, std_msgs__msg__UInt8MultiArray & msg)
{
	RCCHECK(rclc_subscription_init_default(
		&subscription_multi_uint8,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt8MultiArray),
		topic_name
	));
	std_msgs__msg__UInt8MultiArray__init(&msg);
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

void UROS::echo_int16(const char * topic_name, rcl_subscription_t & subscription_int16, std_msgs__msg__Int16 & msg)
{
	RCCHECK(rclc_subscription_init_default(
		&subscription_int16,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16),
		topic_name
	));
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

void UROS::echo_multi_int16(const char * topic_name, rcl_subscription_t & subscription_multi_int16, std_msgs__msg__Int16MultiArray & msg)
{
	RCCHECK(rclc_subscription_init_default(
		&subscription_multi_int16,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16MultiArray),
		topic_name
	));
}

void UROS::post_uint8(uint8_t data, rcl_publisher_t & publisher_uint8)
{
	std_msgs__msg__UInt8 msg;
	msg.data = data;
	RCSOFTCHECK(rcl_publish(&publisher_uint8, &msg, NULL));
}

void UROS::post_multi_uint8(uint8_t * data, size_t size, rcl_publisher_t & publisher_multi_uint8)
{
	multi_uint8_msg.data.size = size;
	for (size_t i = 0; i < size; i++)
		multi_uint8_msg.data.data[i] = data[i];
	RCSOFTCHECK(rcl_publish(&publisher_multi_uint8, &multi_uint8_msg, NULL));
}

void UROS::post_int16(int16_t data, rcl_publisher_t & publisher_int16)
{
	std_msgs__msg__Int16 msg;
	msg.data = data;
	RCSOFTCHECK(rcl_publish(&publisher_int16, &msg, NULL));
}

void UROS::post_multi_int16(int16_t * data, size_t size, rcl_publisher_t & publisher_multi_int16)
{
	multi_int16_msg.data.size = size;
	for (size_t i = 0; i < size; i++)
		multi_int16_msg.data.data[i] = data[i];
	RCSOFTCHECK(rcl_publish(&publisher_multi_int16, &multi_int16_msg, NULL));
}

void callback_uint8(const void * msgin)
{
	const std_msgs__msg__UInt8 * msg = (const std_msgs__msg__UInt8 *) msgin;
	uint8_t data = msg->data;
}

void callback_int16(const void * msgin)
{
	const std_msgs__msg__Int16 * msg = (const std_msgs__msg__Int16 *) msgin;
	int16_t data = msg->data;
}

void callback_multi_int16(const void * msgin)
{
	const std_msgs__msg__Int16MultiArray * msg = (const std_msgs__msg__Int16MultiArray *) msgin;
	size_t size = msg->data.size;
	for (size_t i = 0; i < size; i++)
		int16_t data = msg->data.data[i];
}
