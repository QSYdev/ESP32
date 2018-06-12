#include <Multicast.hpp>

WiFiClient client;

Multicast::Multicast(uint16_t packetSize)
	:Observer(), mPacketSize(packetSize), mPacketBuffer(new char[packetSize]()), mUDP(), mAcceptingPackets(false), mSemAcceptingPackets(xSemaphoreCreateMutex())
{
}

void Multicast::notify(Event* event)
{
	Serial.println("Evento recibido en multicast");
}

void Multicast::init(IPAddress multicastAddress, uint16_t port)
{
	mUDP.beginMulticast(multicastAddress, port);
}

void Multicast::tick()
{
	if (mUDP.parsePacket() > 0 && isAcceptingPackets())
	{
		int message_size = mUDP.read(mPacketBuffer, mPacketSize);

		Serial.print("Packet received from ");
		Serial.print(mUDP.remoteIP());
		Serial.print(":");
		Serial.print(mUDP.remotePort());
		Serial.print(" : SIZE = ");
		Serial.println(message_size);
		Serial.print("Resultado de la conexion = ");
		Serial.println(client.connect(mUDP.remoteIP(), 3000));
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