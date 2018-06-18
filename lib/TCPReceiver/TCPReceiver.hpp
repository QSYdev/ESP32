#pragma once
#include <Observer.hpp>
#include <map>
#include <WiFi.h>

class TCPReceiver : public Observable
{

private:
	class ListElement
	{
	public:
		WiFiClient* mWiFiClient;
		char mBuffer[QSY_PACKET_SIZE];
		int mAvailable;

		inline ListElement(WiFiClient* wifiClient)	:mWiFiClient(wifiClient), mAvailable(QSY_PACKET_SIZE) {}

	};

private:
	std::map<uint16_t, ListElement*> mConnectedNodes;

public:
	TCPReceiver();

	void init();
	void tick();

	void hello(uint16_t physicalId, WiFiClient* client);
	void disconnectedNode(uint16_t physicalId);

};