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
	std::list<ListElement*> nodesToNotify;

	std::map<uint16_t, ListElement*>::iterator iterator = mConnectedNodes.begin();
	while (iterator != mConnectedNodes.end())
	{
		ListElement* currentElement = iterator->second;
		int available = currentElement->mWiFiClient->available();
		if (available)
		{
			int bytesToRead = (available > currentElement->mAvailable) ? currentElement->mAvailable : available;
			currentElement->mAvailable -= currentElement->mWiFiClient->readBytes(currentElement->mBuffer, bytesToRead);

			if (!currentElement->mAvailable)
			{
				nodesToNotify.push_back(currentElement);
				currentElement->mAvailable = QSY_PACKET_SIZE;
			}
		}
		++iterator;
	}

	for (ListElement* element : nodesToNotify)
	{
		PacketReceived event(element->mWiFiClient->remoteIP(), reinterpret_cast<qsy_packet*>(element->mBuffer));
		notify(&event);
	}
}

void TCPReceiver::hello(uint16_t physicalId, WiFiClient* client)
{
	mConnectedNodes[physicalId] = new ListElement(client);
}

void TCPReceiver::disconnectedNode(uint16_t physicalId)
{
	ListElement* element = mConnectedNodes[physicalId];
	mConnectedNodes.erase(physicalId);
	delete element->mWiFiClient;
	delete element;
}