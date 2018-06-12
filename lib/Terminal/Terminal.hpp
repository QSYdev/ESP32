#pragma once
#include <IPAddress.h>
#include <QSYPacket.h>
#include <WiFiManager.hpp>
#include <Multicast.hpp>

class Terminal
{

private:
	WiFiManager mWiFiManager;
	Multicast mMulticast;

public:
	Terminal();

	void start();
	void tick();
	void searchNodes();
	void finalizeNodesSearching();
	void packetReceived(IPAddress ipRemote, struct qsy_packet* packet);

};