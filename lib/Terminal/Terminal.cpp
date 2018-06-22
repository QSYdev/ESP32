#include <Terminal.hpp>
#include <QSYPacket.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <algorithm>
#include <CustomExecutor.hpp>

Terminal::Terminal()
{
	mMulticast.setAcceptingPackets(true);
	
	mMulticast.add(this);
	mTCPReceiver.add(this);
	mDeadNodesPurger.add(this);
	//TODO mBluetoothReceiver.add(this);
}

void Terminal::notify(const Event* event)
{
	switch(event->mType)
	{
		case Event::event_type::PacketReceived:
		{
			const PacketReceived* packetReceivedEvent = reinterpret_cast<const PacketReceived*>(event);
			if (!packetReceivedEvent->mPacket->isValid())
				return;

			uint16_t physicalId = packetReceivedEvent->mPacket->getId();
			switch(packetReceivedEvent->mPacket->getType())
			{
				case QSYPacket::PacketType::Hello:
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

							if (mConnectedNodes.size() == 2)
							{
								std::list<const Step*> steps;
								{
									std::list<const NodeConfiguration*> nodeConfiguration;
									nodeConfiguration.push_back(new NodeConfiguration(1, {0, 0, 0xF}, 500));
									std::string expression = "1";
									steps.push_back(new Step(expression, 2000, false, nodeConfiguration));
								}
								{
									std::list<const NodeConfiguration*> nodeConfiguration;
									nodeConfiguration.push_back(new NodeConfiguration(2, {0, 0xF, 0xF}, 500));
									std::string expression = "2";
									steps.push_back(new Step(expression, 2000, false, nodeConfiguration));	
								}
								{
									std::list<const NodeConfiguration*> nodeConfiguration;
									nodeConfiguration.push_back(new NodeConfiguration(1, {0xF, 0xF, 0xF}, 500));
									nodeConfiguration.push_back(new NodeConfiguration(2, {0x0, 0xF, 0xF}, 500));
									
									std::string expression = "1&2";
									steps.push_back(new Step(expression, 2000, false, nodeConfiguration));	
								}
								std::string name = "Rutina";
								const Routine* routine = new Routine(1, 1, 0, name, steps);
								mExecutor = new CustomExecutor(routine, mConnectedNodes);
								mExecutor->add(this);
								mExecutor->init();
							}
						}
						else
						{
							delete client;
						}
					}
					break;
				}

				case QSYPacket::PacketType::Touche:
				{
					mDeadNodesPurger.touche(physicalId);
					break;
				}

				case QSYPacket::PacketType::Keepalive:
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
			break;
		}

		case Event::CommandRequest:
		{
			const CommandRequest* commandRequestEvent = reinterpret_cast<const CommandRequest*>(event);
			QSYPacket* packet = new QSYPacket();
			packet->setId(commandRequestEvent->mId);
			packet->setDelay(commandRequestEvent->mDelay);
			packet->setColor(commandRequestEvent->mColor);
			packet->setType(QSYPacket::PacketType::Command);
			packet->setStep(commandRequestEvent->mStep);
			mTCPSender.command(packet);
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
	//TODO mBluetoothReceiver.init();
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
			//TODO term->mBluetoothReceiver.tick();
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