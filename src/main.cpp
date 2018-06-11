#include <Arduino.h>
#include <WiFiManager.hpp>
#include <Multicast.hpp>

WiFiManager wifiManager;
Multicast multicast;

void setup()
{
	Serial.begin(115200);
	delay(10);

	wifiManager.init("ESP32", "123456789", 8, {192, 168, 0, 1});
	multicast.init({224, 0, 0, 12}, 3000);
}

void loop()
{
	wifiManager.tick();
	multicast.tick();
	delay(20);
}