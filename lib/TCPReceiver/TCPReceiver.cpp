#include <TCPReceiver.hpp>

TCPReceiver::TCPReceiver()
	:mConnectedNodes()
{
}

void TCPReceiver::init()
{
}

void TCPReceiver::tick()
{
	List<ListElement*> nodesToNotify;

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
				nodesToNotify.add(currentElement);
				currentElement->mAvailable = 0;
			}
		}
	}

	nodesToNotify.begin();
	while (!nodesToNotify.end())
	{
		ListElement* element = nodesToNotify.next();
		PacketReceived event(element->mWiFiClient->remoteIP(), reinterpret_cast<qsy_packet*>(element->mBuffer));
		notify(&event);
	}
}

void TCPReceiver::hello(WiFiClient* client)
{
	mConnectedNodes.add(new ListElement(client));
}