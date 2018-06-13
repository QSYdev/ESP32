#pragma once
#include <WiFi.h>
#include <Udp.h>
#include <Observer.hpp>

class Multicast : public Observable
{

private:
	bool mAcceptingPackets;
	char mPacketBuffer[QSY_PACKET_SIZE];
	WiFiUDP mUDP;

public:
	Multicast();
	
	void init();
	void tick();
	inline bool isAcceptingPackets() { return mAcceptingPackets; }
	inline void setAcceptingPackets(bool acceptingPackets) { mAcceptingPackets = acceptingPackets; }

};