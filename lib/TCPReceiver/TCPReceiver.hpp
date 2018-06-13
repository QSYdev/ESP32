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
		char mBuffer[QSY_PACKET_SIZE];
		int mAvailable;

		ListElement(WiFiClient* wifiClient)	:mWiFiClient(wifiClient), mAvailable(QSY_PACKET_SIZE) {}

	};

private:
	List<ListElement*> mConnectedNodes;

public:
	TCPReceiver();

	void init();
	void tick();

	void hello(WiFiClient* client);

};