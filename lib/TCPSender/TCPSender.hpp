#pragma once
#include <List.hpp>
#include <WiFi.h>
#include <QSYPacket.hpp>
#include <FreeRTOS.h>
#include <freertos/semphr.h>

class TCPSender
{

private:
	List<WiFiClient*> mConnectedNodes;
	volatile SemaphoreHandle_t mSemConnectedNodes;
	List<const qsy_packet*> mPendingTasks;
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