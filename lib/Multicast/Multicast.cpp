#include <Multicast.hpp>

Multicast::Multicast(uint16_t packetSize)
	:mPacketSize(packetSize), mAcceptingPackets(false), mPacketBuffer(new char[packetSize]()), mUDP()
{
}

Multicast::~Multicast()
{
	delete[] mPacketBuffer;
}

void Multicast::init(IPAddress multicastAddress, uint16_t port)
{
	mUDP.beginMulticast(multicastAddress, port);
}

void Multicast::tick()
{
	if (mUDP.parsePacket() > 0 && isAcceptingPackets())
	{
		if (mUDP.read(mPacketBuffer, mPacketSize) != mPacketSize)
			return;
			
		qsy_packet* packet = reinterpret_cast<qsy_packet*>(mPacketBuffer);
		PacketReceived event(mUDP.remoteIP(), packet);
		notify(&event);
	}
}
