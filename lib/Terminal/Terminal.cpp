#include <Terminal.hpp>
#include <QSYPacket.h>

Terminal::Terminal()
	:mWiFiManager(), mMulticast(QSY_PACKET_SIZE)
{
}

void Terminal::notify(Event* event)
{
	Serial.println("Event recibido en la terminal.");
}

void Terminal::start()
{
	mWiFiManager.init(QSY_SSID, QSY_PASSWORD, QSY_MAX_CONNECTIONS);
	mMulticast.init(QSY_MULTICAST_ADDRESS, QSY_MULTICAST_PORT);
}

void Terminal::tick()
{
	mWiFiManager.tick();
	mMulticast.tick();
}

void Terminal::searchNodes()
{
	mMulticast.setAcceptingPackets(true);
}

void Terminal::finalizeNodesSearching()
{
	mMulticast.setAcceptingPackets(false);
}