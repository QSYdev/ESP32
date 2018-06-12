#pragma once
#include <WiFi.h>
#include <Udp.h>
#include <Observer.hpp>

class Multicast : public Observable, public Observer
{

private:
	const uint16_t mPacketSize;
	char* mPacketBuffer;
	WiFiUDP mUDP;
	volatile bool mAcceptingPackets;
	volatile SemaphoreHandle_t mSemAcceptingPackets;

public:
	Multicast(uint16_t packetSize);
	
	void notify(Event* event) override;
	
	void init(IPAddress multicastAddress, uint16_t port);
	void tick();
	bool isAcceptingPackets();
	void setAcceptingPackets(bool acceptingPackets);

};