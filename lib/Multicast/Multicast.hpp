#pragma once
#include <WiFi.h>
#include <Udp.h>
#include <Observer.hpp>

class Multicast : public Observable
{

private:
	const uint16_t mPacketSize;
	char* mPacketBuffer;
	WiFiUDP mUDP;

public:
	Multicast(uint16_t packetSize);
	~Multicast();
	
	void init(IPAddress multicastAddress, uint16_t port);
	void tick();

};