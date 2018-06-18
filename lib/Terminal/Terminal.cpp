#include <Terminal.hpp>
#include <QSYPacket.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <algorithm>

Terminal::Terminal()
	:mWiFiManager(), mMulticast(), mTCPReceiver(), mDeadNodesPurger(), mBluetoothReceiver(), mTCPSender(), mConnectedNodes()
{
	mMulticast.setAcceptingPackets(true);
	
	mMulticast.add(this);
	mTCPReceiver.add(this);
	mDeadNodesPurger.add(this);
	mBluetoothReceiver.add(this);
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
					if (std::find(mConnectedNodes.begin(), mConnectedNodes.end(), physicalId) == mConnectedNodes.end())
					{
						WiFiClient* client = new WiFiClient();
						if (client->connect(packetReceivedEvent->mIpRemote, QSY_TCP_SERVER_PORT))
						{
							Serial.print("C = ");
							Serial.println(physicalId);
							client->setNoDelay(true);
							mConnectedNodes.push_back(physicalId);
							mTCPReceiver.hello(physicalId, client);
							mDeadNodesPurger.hello(physicalId);
							mTCPSender.hello(physicalId, client);
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
			Serial.print("D = ");
			Serial.println(disconnectedNodeEvent->mPhysicalId);
			mConnectedNodes.remove(disconnectedNodeEvent->mPhysicalId);
			mTCPReceiver.disconnectedNode(disconnectedNodeEvent->mPhysicalId);
			mDeadNodesPurger.disconnectedNode(disconnectedNodeEvent->mPhysicalId);
			mTCPSender.disconnectedNode(disconnectedNodeEvent->mPhysicalId);
			break;
		}

		case Event::event_type::CommandReceivedFromUser:
		{
			const CommandReceivedFromUser* commandReceivedFromUser = reinterpret_cast<const CommandReceivedFromUser*>(event);
			Serial.print("B = ");
			Serial.println(commandReceivedFromUser->mCommand);
			if (commandReceivedFromUser->mCommand == 'r')
			{
				qsy_packet packet;
				color c;
				c.red = 0xF;
				c.green = 0;
				c.blue = 0;
				packet_init(&packet);
				packet_set_id(&packet, 19);
				packet_set_delay(&packet, 500);
				packet_set_color(&packet, c);
				packet_set_type(&packet, packet_type::command);
				packet_set_step(&packet, 1);
				mTCPSender.command(&packet);
			}
			break;
		}
	}
}

void Terminal::start()
{
	xTaskCreatePinnedToCore(&task0, "", 2048, this, 2, NULL, 0);
	xTaskCreatePinnedToCore(&task1, "", 2048, this, 2, NULL, 1);
}

void Terminal::task0(void* args)
{
	Terminal* term = reinterpret_cast<Terminal*>(args);

	term->mWiFiManager.init();
	term->mMulticast.init();
	term->mTCPReceiver.init();
	term->mDeadNodesPurger.init();
	term->mBluetoothReceiver.init();

	while(true)
	{
		try
		{
			term->mWiFiManager.tick();
			term->mMulticast.tick();
			term->mTCPReceiver.tick();
			term->mDeadNodesPurger.tick();
			term->mBluetoothReceiver.tick();
		}
		catch (...)
		{
		}
		
		vTaskDelay(1);
	}
}

void Terminal::task1(void* args)
{
	Terminal* term = reinterpret_cast<Terminal*>(args);

	term->mTCPSender.init();

	while (true)
		term->mTCPSender.tick();
}