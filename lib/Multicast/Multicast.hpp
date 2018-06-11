#pragma once
#include <WiFi.h>
#include <Udp.h> 

class Multicast
{

private:
	WiFiUDP mUDP;
	volatile bool mAcceptingPackets;
	SemaphoreHandle_t mSemAcceptingPackets;
	char packetBuffer[16];

public:
	Multicast();
	
	void init(IPAddress multicastAddress, uint16_t port);
	void tick();
	bool isAcceptingPackets();
	void setAcceptingPackets(bool acceptingPackets);

};