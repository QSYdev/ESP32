#include <Terminal.hpp>
#include <QSYPacket.hpp>

Terminal::Terminal()
	:mWiFiManager(), mMulticast(QSY_PACKET_SIZE)
{
	mMulticast.add(this);
}

void Terminal::notify(Event* event)
{
	switch(event->mType)
	{
		case Event::event_type::PacketReceived:
			PacketReceived* packetReceivedEvent = reinterpret_cast<PacketReceived*>(event);
			Serial.print("REMOTE IP = ");
			Serial.print(packetReceivedEvent->mIpRemote);
			Serial.print(" ID = ");
			Serial.println(packet_get_id(packetReceivedEvent->mPacket));
			break;
	}
}

void Terminal::start()
{
	mWiFiManager.init(QSY_SSID, QSY_PASSWORD, QSY_MAX_CONNECTIONS);
	mMulticast.init(QSY_MULTICAST_ADDRESS, QSY_MULTICAST_PORT);
}

void Terminal::tick()
{
	mMulticast.tick();
}