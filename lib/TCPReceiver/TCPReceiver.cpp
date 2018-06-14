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
			unsigned long t = micros();

			int bytesToRead = (available > currentElement->mAvailable) ? currentElement->mAvailable : available;
			currentElement->mAvailable -= currentElement->mWiFiClient->readBytes(currentElement->mBuffer, bytesToRead);

			if (!currentElement->mAvailable)
			{
				nodesToNotify.add(currentElement);
				currentElement->mAvailable = QSY_PACKET_SIZE;
			}

			t = micros() - t;
			Serial.print("TCP RECEIVER = ");
			Serial.println(t);
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

void TCPReceiver::hello(uint16_t physicalId, WiFiClient* client)
{
	mConnectedNodes.addById(physicalId, new ListElement(client));
}

void TCPReceiver::disconnectedNode(uint16_t physicalId)
{
	ListElement* element = mConnectedNodes.removeById(physicalId);
	if (element != nullptr)
	{
		delete element->mWiFiClient;
		delete element;
	}
}