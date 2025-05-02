// hardware definitions
const byte LED[6] = {11, 12, 13, 14, LED_BUILTIN, RGB_BUILTIN};
byte MOTOR[3][2] = {
	{17, 18},
	{8, 3},
	{9, 10}
}, ENCODER[3][2] = {
	{5, 4},
	{7, 6},
	{16, 15}
};

const int enc_ms = 50;

// uROS configurations
char *NODE_NAME  = "sensiact",
	 *TOPIC_LIM = "sensiact/limit",
	 *TOPIC_STATUS  = "sensiact/led",
	 *TOPIC_ENCODER_POS = "sensiact/encoder/pos",
	 *TOPIC_ENCODER_VEL = "sensiact/encoder/vel",
	 *TOPIC_MOTOR_VEL = "sensiact/motor/vel";
