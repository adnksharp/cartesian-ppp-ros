// hardware definitions
int LED = 2;
int ENCODER_A = 34;
int ENCODER_B = 35;

// uROS configurations
char *NODE_NAME = "ESP32";
//char *SUB_TOPIC = "driver/listener";
char *ENCODER_TOPIC = "driver/encoder/accel";
const unsigned int WATCHDOG = 1000;
