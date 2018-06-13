#pragma once
#include <Observer.hpp>
#include <List.hpp>
#include <WiFi.h>

class TCPReceiver : public Observable
{

private:
	class ListElement
	{
	public:
		WiFiClient* mWiFiClient;
		uint8_t* mBuffer;
		int mAvailable;

		ListElement(WiFiClient* wifiClient, int packetSize)	:mWiFiClient(wifiClient), mBuffer(new uint8_t[packetSize]()), mAvailable(packetSize) {}
		~ListElement()	{ delete[] mBuffer; }
	};

private:
	const int mPacketSize;
	List<ListElement*> mConnectedNodes;

public:
	TCPReceiver(int packetSize);

	void init();
	void tick();

	void hello(WiFiClient* client);

};