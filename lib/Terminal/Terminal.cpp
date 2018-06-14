#include <Terminal.hpp>
#include <QSYPacket.hpp>

Terminal::Terminal()
	:mWiFiManager(), mMulticast(), mTCPReceiver(), mDeadNodesPurger(), mConnectedNodes()
{
	mMulticast.setAcceptingPackets(true);
	
	mMulticast.add(this);
	mTCPReceiver.add(this);
	mDeadNodesPurger.add(this);
}

void Terminal::notify(const Event* event)
{
	switch(event->mType)
	{
		case Event::event_type::PacketReceived:
		{
			const PacketReceived* packetReceivedEvent = reinterpret_cast<const PacketReceived*>(event);
			if (!packet_is_valid(packetReceivedEvent->mPacket))
				return;

			uint16_t physicalId = packet_get_id(packetReceivedEvent->mPacket);
			switch(packet_get_type(packetReceivedEvent->mPacket))
			{
				case packet_type::hello:
				{
					if (!mConnectedNodes.include(physicalId))
					{
						WiFiClient* client = new WiFiClient();
						if (client->connect(packetReceivedEvent->mIpRemote, QSY_TCP_SERVER_PORT))
						{
							Serial.print("NEW NODE ID = ");
							Serial.println(physicalId);
							client->setNoDelay(true);
							mConnectedNodes.add(physicalId);
							mTCPReceiver.hello(physicalId, client);
							mDeadNodesPurger.hello(physicalId);
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
					mDeadNodesPurger.touche(physicalId);
					break;
				}

				case packet_type::keepalive:
				{
					mDeadNodesPurger.keepAlive(physicalId);
					break;
				}

				default:
				{
					break;
				}
			}
			break;
		}

		case Event::event_type::DisconnectedNode:
		{
			const DisconnectedNode* disconnectedNodeEvent = reinterpret_cast<const DisconnectedNode*>(event);
			Serial.print("DISCONNECTED NODE ID = ");
			Serial.println(disconnectedNodeEvent->mPhysicalId);
			if (mConnectedNodes.remove(disconnectedNodeEvent->mPhysicalId))
			{
				mTCPReceiver.disconnectedNode(disconnectedNodeEvent->mPhysicalId);
				mDeadNodesPurger.disconnectedNode(disconnectedNodeEvent->mPhysicalId);
			}
			break;
		}
	}
}

void Terminal::start()
{
	mWiFiManager.init();
	mMulticast.init();
	mTCPReceiver.init();
	mDeadNodesPurger.init();
}

void Terminal::tick()
{
	mWiFiManager.tick();
	mMulticast.tick();
	mTCPReceiver.tick();
	mDeadNodesPurger.tick();
}