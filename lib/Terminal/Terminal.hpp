#pragma once
#include <WiFiManager.hpp>
#include <Multicast.hpp>
#include <TCPReceiver.hpp>
#include <DeadNodesPurger.hpp>
#include <TCPSender.hpp>
#include <Observer.hpp>
#include <List.hpp>

class Terminal : public Observer
{

private:
	WiFiManager mWiFiManager;
	Multicast mMulticast;
	TCPReceiver mTCPReceiver;
	DeadNodesPurger mDeadNodesPurger;
	TCPSender mTCPSender;
	List<uint16_t> mConnectedNodes;

public:
	Terminal();

	void notify(const Event* event) override;

	void start();

private:
	static void task0(void* args);
	static void task1(void* args);

};