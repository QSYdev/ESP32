#include <Terminal.hpp>
#include <QSYPacket.hpp>

Terminal::Terminal()
	:mWiFiManager(), mMulticast(QSY_PACKET_SIZE), mTCPReceiver(QSY_PACKET_SIZE), mConnectedNodes()
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
			switch(packet_get_type(packetReceivedEvent->mPacket))
			{
				case packet_type::hello:
				{
					Serial.println("HELLO");
					int nodeId = packet_get_id(packetReceivedEvent->mPacket);
					if (!mConnectedNodes.include(nodeId))
					{
						Serial.println("NEW NODE");
						WiFiClient* client = new WiFiClient();
						if (client->connect(packetReceivedEvent->mIpRemote, QSY_TCP_SERVER_PORT))
						{
							Serial.println("NEW CLIENT");
							mConnectedNodes.add(nodeId);
							mTCPReceiver.hello(client);
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
	mWiFiManager.init(QSY_SSID, QSY_PASSWORD, QSY_MAX_CONNECTIONS);
	mMulticast.init(QSY_MULTICAST_ADDRESS, QSY_MULTICAST_PORT);
	mTCPReceiver.init();
}

void Terminal::tick()
{
	mMulticast.tick();
	mTCPReceiver.tick();
}