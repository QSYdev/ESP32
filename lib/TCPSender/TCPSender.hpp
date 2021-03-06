#pragma once
#include <list>
#include <map>
#include <WiFi.h>
#include <QSYWiFiPacket.hpp>

class TCPSender
{

private:
	std::map<uint16_t, WiFiClient*> mConnectedNodes;
	volatile SemaphoreHandle_t mSemConnectedNodes;
	std::list<const QSYWiFiPacket*> mPendingTasks;
	volatile SemaphoreHandle_t mSemPendingTasks;
	volatile SemaphoreHandle_t mSemAvailableData;

public:
	TCPSender();

	void init();
	void tick();
	void hello(uint16_t physicalId, WiFiClient* node);
	void command(const CommandArgs& args);
	void disconnectedNode(uint16_t physicalId);

};