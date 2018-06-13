#include <TCPReceiver.hpp>

TCPReceiver::TCPReceiver(int packetSize)
	:mPacketSize(packetSize), mConnectedNodes()
{
}

void TCPReceiver::init()
{
}

void TCPReceiver::tick()
{
	List<qsy_packet*> full;

	mConnectedNodes.begin();
	while (!mConnectedNodes.end())
	{
		ListElement* currentElement = mConnectedNodes.next();
		int available = currentElement->mWiFiClient->available();
		if (available)
		{
			int bytesToRead = (available > currentElement->mAvailable) ? currentElement->mAvailable : available;
			currentElement->mAvailable -= currentElement->mWiFiClient->readBytes(currentElement->mBuffer, bytesToRead);
			currentElement->mWiFiClient->flush();
			if (!currentElement->mAvailable)
			{
				full.add(reinterpret_cast<qsy_packet*>(currentElement->mBuffer));
				currentElement->mAvailable = 0;
			}
		}
	}

	full.begin();
	while (!full.end())
	{
		PacketReceived* event = new PacketReceived({0, 0, 0, 0}, full.next());
		notify(event);
		delete event;
	}
}

void TCPReceiver::hello(WiFiClient* client)
{
	mConnectedNodes.add(new ListElement(client, mPacketSize));
}