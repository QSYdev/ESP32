#pragma once
#include <WiFiManager.hpp>
#include <Multicast.hpp>
#include <TCPReceiver.hpp>
#include <Observer.hpp>
#include <List.hpp>

class Terminal : public Observer
{

private:
	WiFiManager mWiFiManager;
	Multicast mMulticast;
	TCPReceiver mTCPReceiver;
	List<int> mConnectedNodes;

public:
	Terminal();

	void notify(Event* event) override;

	void start();
	void tick();

};