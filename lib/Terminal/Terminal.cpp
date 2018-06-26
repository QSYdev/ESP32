#include <Terminal.hpp>
#include <QSYWiFiPacket.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <algorithm>
#include <CustomExecutor.hpp>
#include <PlayerExecutor.hpp>

Terminal::Terminal()
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
		case Event::EventType::PacketReceived:
		{
			const PacketReceived* packetReceivedEvent = reinterpret_cast<const PacketReceived*>(event);
			if (!packetReceivedEvent->mPacket->isValid())
				return;

			uint16_t physicalId = packetReceivedEvent->mPacket->getId();
			switch(packetReceivedEvent->mPacket->getType())
			{
				case QSYWiFiPacket::PacketType::Hello:
				{
					if (std::find(mConnectedNodes.begin(), mConnectedNodes.end(), physicalId) == mConnectedNodes.end())
					{
						WiFiClient* client = new WiFiClient();
						if (client->connect(packetReceivedEvent->mIpRemote, QSY_WIFI_TCP_SERVER_PORT))
						{
							Serial.print("C = ");
							Serial.println(physicalId);
							client->setNoDelay(true);
							mConnectedNodes.push_back(physicalId);
							mTCPReceiver.hello(physicalId, client);
							mDeadNodesPurger.hello(physicalId);
							mBluetoothReceiver.hello(physicalId);
							mTCPSender.hello(physicalId, client);
						}
						else
						{
							delete client;
						}
					}
					break;
				}

				case QSYWiFiPacket::PacketType::Touche:
				{
					mDeadNodesPurger.touche(physicalId);
					if (mExecutor)
					{
						const QSYWiFiPacket* p = packetReceivedEvent->mPacket;
						mExecutor->touche(p->getId(), p->getStep(), p->getColor(), p->getDelay());
					}
					break;
				}

				case QSYWiFiPacket::PacketType::Keepalive:
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

		case Event::EventType::DisconnectedNode:
		{
			const DisconnectedNode* disconnectedNodeEvent = reinterpret_cast<const DisconnectedNode*>(event);
			Serial.print("D = ");
			Serial.println(disconnectedNodeEvent->mPhysicalId);
			mConnectedNodes.remove(disconnectedNodeEvent->mPhysicalId);
			mTCPReceiver.disconnectedNode(disconnectedNodeEvent->mPhysicalId);
			mDeadNodesPurger.disconnectedNode(disconnectedNodeEvent->mPhysicalId);
			mBluetoothReceiver.disconnectedNode(disconnectedNodeEvent->mPhysicalId);
			mTCPSender.disconnectedNode(disconnectedNodeEvent->mPhysicalId);
			break;
		}

		case Event::EventType::CommandRequest:
		{
			const CommandRequest* commandRequestEvent = reinterpret_cast<const CommandRequest*>(event);
			QSYWiFiPacket* packet = new QSYWiFiPacket();
			packet->setId(commandRequestEvent->mId);
			packet->setDelay(commandRequestEvent->mDelay);
			packet->setColor(commandRequestEvent->mColor);
			packet->setType(QSYWiFiPacket::PacketType::Command);
			packet->setStep(commandRequestEvent->mStep);
			mTCPSender.command(packet);
			break;
		}

		case Event::EventType::StartCustomExecution:
		{
			break;
		}

		case Event::EventType::StartPlayerExecution:
		{
			break;
		}

		case Event::EventType::StopExecution:
		{
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
	mBluetoothReceiver.init();
	mTCPSender.init();

	xTaskCreatePinnedToCore(&task0, "", 2048, this, 1, NULL, 1);
	xTaskCreatePinnedToCore(&task1, "", 2048, this, 1, NULL, 0);
	vTaskDelete(NULL);
}

void Terminal::task0(void* args)
{
	Terminal* term = reinterpret_cast<Terminal*>(args);
	while(true)
	{
		try
		{
			term->mWiFiManager.tick();
			term->mMulticast.tick();
			term->mTCPReceiver.tick();
			term->mDeadNodesPurger.tick();
			term->mBluetoothReceiver.tick();
			if (term->mExecutor)
				term->mExecutor->tick();
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
	while (true)
		term->mTCPSender.tick();
}