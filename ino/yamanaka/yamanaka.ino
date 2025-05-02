const byte led[4] = {11, 12, 13, 14};
const byte motor[3][2] = {
	{17,18},
	{8, 3},
	{9, 10}
}, encoder[3][2] = {
	{5, 4},
	{7, 6},
	{16, 15}
};

volatile long position[3] = {0, 0, 0};

void encoderA()
{
	if(digitalRead(encoder[0][0]) == digitalRead(encoder[0][1]))
		position[0]++;
	else
		position[0]--;
}

void encoderB()
{
	if(digitalRead(encoder[1][0]) == digitalRead(encoder[1][1]))
		position[1]++;
	else
		position[1]--;
}

void encoderC()
{
	if(digitalRead(encoder[2][0]) == digitalRead(encoder[2][1]))
		position[2]++;
	else
		position[2]--;
}

void serialEvent()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    int separatorIndex = command.indexOf(':');
    if (separatorIndex != -1)
    {
      int motorIndex = command.substring(0, separatorIndex).toInt();
      int value = command.substring(separatorIndex + 1).toInt();
      Serial.println("Motor: " + String(motorIndex) + ", Value: " + String(value));
      if (motorIndex >= 0 && motorIndex < 3)
      {
        analogWrite(motor[motorIndex][0], value > 0 ? value : 0);
        analogWrite(motor[motorIndex][1], value < 0 ? -value : 0);
      }
    }
    else
    {
      Serial.println("Invalid command format");
    }
  }
}

void setup()
{
	Serial.begin(115200);

	for(size_t i = 0; i < sizeof(encoder) / sizeof(encoder[0]); i++)
		for(byte j = 0; j < 2; j++)
			pinMode(encoder[i][j], INPUT);
	for(size_t i = 0; i < sizeof(motor) / sizeof(motor[0]); i++)
		for(byte j = 0; j < 2; j++)
			pinMode(motor[i][j], OUTPUT);
	for(byte i: led)
		pinMode(i, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(encoder[0][0]), encoderA, CHANGE);
	attachInterrupt(digitalPinToInterrupt(encoder[1][0]), encoderB, CHANGE);
	attachInterrupt(digitalPinToInterrupt(encoder[2][0]), encoderC, CHANGE);
	digitalWrite(RGB_BUILTIN, LOW);
}

void loop()
{
	//for(byte i: led)
		//digitalWrite(i, random(2));
	//neopixelWrite(RGB_BUILTIN, 255 * random(2), 255 * random(2), 255 * random(2));
	Serial.println(String(position[0]) + "\t" + String(position[1]) + "\t" + String(position[2]));
	delay(50);
}
