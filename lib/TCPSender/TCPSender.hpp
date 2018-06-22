#pragma once
#include <list>
#include <map>
#include <WiFi.h>
#include <QSYPacket.hpp>

class TCPSender
{

private:
	std::map<uint16_t, WiFiClient*> mConnectedNodes;
	volatile SemaphoreHandle_t mSemConnectedNodes;
	std::list<const QSYPacket*> mPendingTasks;
	volatile SemaphoreHandle_t mSemPendingTasks;
	volatile SemaphoreHandle_t mSemAvailableData;

public:
	TCPSender();

	void init();
	void tick();
	void hello(uint16_t physicalId, WiFiClient* node);
	void command(const QSYPacket* packet);
	void disconnectedNode(uint16_t physicalId);

};