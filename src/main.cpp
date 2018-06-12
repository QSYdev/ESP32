#include <Arduino.h>
#include <WiFiManager.hpp>
#include <Multicast.hpp>
#include <QSYPacket.h>

WiFiManager wifiManager;
Multicast multicast(16);

void setup()
{
	Serial.begin(115200);
	delay(10);

	wifiManager.init("ESP32", "123456789", 8);
	multicast.init({224, 0, 0, 12}, 3000);
	multicast.setAcceptingPackets(true);
}

void loop()
{
	wifiManager.tick();
	multicast.tick();
	delay(20);
}
