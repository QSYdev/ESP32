#include <Terminal.hpp>
#include <QSYPacket.hpp>

Terminal::Terminal()
	:mWiFiManager(), mMulticast(), mTCPReceiver(), mConnectedNodes()
{
	mMulticast.add(this);
	mMulticast.setAcceptingPackets(true);

	mTCPReceiver.add(this);
}

void Terminal::notify(const Event* event)
{
	switch(event->mType)
	{
		case Event::event_type::PacketReceived:
			const PacketReceived* packetReceivedEvent = reinterpret_cast<const PacketReceived*>(event);
			if (!packet_is_valid(packetReceivedEvent->mPacket))
				return;

			switch(packet_get_type(packetReceivedEvent->mPacket))
			{
				case packet_type::hello:
				{
					Serial.println("HELLO");
					int nodeId = packet_get_id(packetReceivedEvent->mPacket);
					if (!mConnectedNodes.include(nodeId))
					{
						WiFiClient* client = new WiFiClient();
						if (client->connect(packetReceivedEvent->mIpRemote, QSY_TCP_SERVER_PORT))
						{
							Serial.println("NEW CLIENT");
							client->setNoDelay(true);
							mConnectedNodes.add(nodeId);
							mTCPReceiver.hello(client);
						}
						else
						{
							delete client;
						}
					}
					break;
				}

				case packet_type::touche:
				{
					Serial.println("TOUCHE");
					break;
				}

				case packet_type::keepalive:
				{
					Serial.println("KEEP ALIVE");
					break;
				}

				default:
				{
					break;
				}
			}
			break;
	}
}

void Terminal::start()
{
	mWiFiManager.init();
	mMulticast.init();
	mTCPReceiver.init();
}

void Terminal::tick()
{
	mMulticast.tick();
	mTCPReceiver.tick();
}