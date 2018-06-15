#pragma once
#include <List.hpp>
#include <WiFi.h>
#include <QSYPacket.hpp>

class TCPSender
{

private:
	class Task
	{ 
	public: 
		inline Task() {}
		virtual void execute() = 0;
	};

	class NewNodeTask : public Task
	{
	public:
		const uint16_t mPhysicalId;
		WiFiClient* mNode;

		inline NewNodeTask(uint16_t physicalId, WiFiClient* node)	:mPhysicalId(physicalId), mNode(node) {}
		void execute() override;
	};

	class SendCommandTask : public Task
	{
	public:
		const qsy_packet mPacket;

		inline SendCommandTask(const qsy_packet* packet)	:mPacket(packet) {}
		void execute() override;
	};

	class DisconnectedNodeTask : public Task
	{
	public:
		const uint16_t mPhysicalId;

		inline DisconnectedNodeTask(uint16_t physicalId)	:mPhysicalId(physicalId) {}
		void execute() override;
	};

private:
	List<WiFiClient*> mConnectedNodes;
	List<Task*> mPendingTasks;

public:
	TCPSender();
	~TCPSender();

	void tick();
	void hello(uint16_t physicalId, WiFiClient* node);
	void command(const qsy_packet* packet);
	void disconnectedNode(uint16_t physicalId);
};