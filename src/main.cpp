#include <Arduino.h>
#include <WiFi.h>
#include <Udp.h> 

const char* ssid = "ESP32";
const char* password = "123456789";

WiFiUDP udp;
char packetBuffer[16];
 
void setup()
{
	Serial.begin(115200);
	delay(10);

	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid, password, 1, 0, 16);
	Serial.println();
	Serial.print("IP address: ");
	Serial.println(WiFi.softAPIP());

	udp.beginMulticast({224, 0, 0, 12}, 3000);
}

void loop()
{
	int message_size = 0;

	int noBytes = udp.parsePacket();
	Serial.println(noBytes);
	if (noBytes > 0 && noBytes <= 16) 
	{
		Serial.println("Entro?");
		Serial.print(millis() / 1000);
		Serial.print(":Packet of ");
		Serial.print(noBytes);
		Serial.print(" received from ");
		Serial.print(udp.remoteIP());
		Serial.print(":");
		Serial.println(udp.remotePort());
		message_size = udp.read(packetBuffer, 16);

		if (message_size > 0)
		{ 
			packetBuffer[message_size] = 0;
			Serial.println(packetBuffer);
		}
	}
	delay(20);
}