#include <Multicast.hpp>

Multicast::Multicast()
	:mUDP(), mAcceptingPackets(false), mSemAcceptingPackets(xSemaphoreCreateMutex())
{
}

void Multicast::init(IPAddress multicastAddress, uint16_t port)
{
	mUDP.beginMulticast(multicastAddress, port);
}

void Multicast::tick()
{
	if (mUDP.parsePacket() > 0 && isAcceptingPackets())
	{
		int message_size = mUDP.read(packetBuffer, 16);

		Serial.print("Packet received from ");
		Serial.print(mUDP.remoteIP());
		Serial.print(":");
		Serial.print(mUDP.remotePort());
		Serial.print(" : SIZE = ");
		Serial.println(message_size);
	}
}

bool Multicast::isAcceptingPackets()
{
	bool result;
	xSemaphoreTake(mSemAcceptingPackets, portTICK_PERIOD_MS);
	{
		result = mAcceptingPackets;
	}
	xSemaphoreGive(mSemAcceptingPackets);
	return result;
}

void Multicast::setAcceptingPackets(bool isAcceptingPackets)
{
	xSemaphoreTake(mSemAcceptingPackets, portTICK_PERIOD_MS);
	{
		mAcceptingPackets = isAcceptingPackets;
	}
	xSemaphoreGive(mSemAcceptingPackets);
}