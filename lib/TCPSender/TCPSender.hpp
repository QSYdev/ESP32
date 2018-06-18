#pragma once
#include <list>
#include <map>
#include <WiFi.h>
#include <QSYPacket.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class TCPSender
{

private:
	std::map<uint16_t, WiFiClient*> mConnectedNodes;
	volatile SemaphoreHandle_t mSemConnectedNodes;
	std::list<const qsy_packet*> mPendingTasks;
	volatile SemaphoreHandle_t mSemPendingTasks;
	volatile SemaphoreHandle_t mSemAvailableData;

public:
	TCPSender();

	void init();
	void tick();
	void hello(uint16_t physicalId, WiFiClient* node);
	void command(const qsy_packet* packet);
	void disconnectedNode(uint16_t physicalId);

};