#include <Multicast.hpp>

Multicast::Multicast(uint16_t packetSize)
	:mPacketSize(packetSize), mPacketBuffer(new char[packetSize]()), mUDP()
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
	if (mUDP.parsePacket() > 0)
	{
		if (mUDP.read(mPacketBuffer, mPacketSize) != mPacketSize)
			return;
			
		qsy_packet* packet = reinterpret_cast<qsy_packet*>(mPacketBuffer);
		if (packet_is_valid(packet))
		{
			PacketReceived* event = new PacketReceived(mUDP.remoteIP(), packet);
			notify(event);
			delete event;
		}
	}
}