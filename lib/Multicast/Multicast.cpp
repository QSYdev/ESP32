#include <Multicast.hpp>

Multicast::Multicast()
	:mAcceptingPackets(false), mUDP()
{
}

void Multicast::init()
{
	mUDP.beginMulticast(QSY_MULTICAST_ADDRESS, QSY_MULTICAST_PORT);
}

void Multicast::tick()
{
	if (isAcceptingPackets() && mUDP.parsePacket() > 0)
	{
		if (mUDP.read(mPacketBuffer, QSY_PACKET_SIZE) != QSY_PACKET_SIZE)
			return;
			
		qsy_packet* packet = reinterpret_cast<qsy_packet*>(mPacketBuffer);
		PacketReceived event(mUDP.remoteIP(), packet);
		notify(&event);
	}
}
