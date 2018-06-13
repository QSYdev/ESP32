#include <Arduino.h>
#include <Terminal.hpp>

Terminal terminal;

void setup()
{
	Serial.begin(115200);
	delay(10);

	terminal.start();
}

void loop()
{
	terminal.tick();
	delay(20);
}
