#include <Multicast.hpp>

Multicast::Multicast()
	:mAcceptingPackets(false), mUDP()
{
}

void Multicast::init()
{
	mUDP.beginMulticast(QSY_WIFI_MULTICAST_ADDRESS, QSY_WIFI_MULTICAST_PORT);
}

void Multicast::tick()
{
	if (isAcceptingPackets() && mUDP.parsePacket() > 0)
	{
		if (mUDP.read(mPacketBuffer, QSY_WIFI_PACKET_SIZE) != QSY_WIFI_PACKET_SIZE)
			return;
			
		const QSYWiFiPacket* packet = reinterpret_cast<const QSYWiFiPacket*>(mPacketBuffer);
		PacketReceived event(mUDP.remoteIP(), packet);
		notify(&event);
	}
}
