// hardware definitions
#ifdef RGB_BUILTIN
byte LED[6] = {11, 12, 13, 14, LED_BUILTIN, RGB_BUILTIN};
#else
byte LED[6] = {11, 12, 13, 14, 2, 0};
#endif
byte MOTOR[3][2] = {
	{17, 18},
	{10, 9},
	{8, 3}
}, ENCODER[3][2] = {
	{5, 4},
	{6, 7},
	{16, 15}
};

int motorMin[3] = {98, 102, 39};
float PPMM[3] = {12.1, 12.1, 118.7};

// pid config
double kp[3] = {0.375, 0.375, 0.375},
	   ki[3] = {0.0974, 0.0974, 0.0974},
	   kd[3] = {0.01, 0.01, 0.01};

// uROS configurations
char *NODE_NAME  = "sensiact",
	 *TOPIC_PUB = "yoba/sensiact/info",
	 *TOPIC_ECHO = "yoba/sensiact/cmd_pos";

byte timeout = 100;
