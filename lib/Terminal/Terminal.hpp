#pragma once
#include <WiFiManager.hpp>
#include <Multicast.hpp>
#include <Observer.hpp>

class Terminal : public Observable, public Observer
{

private:
	WiFiManager mWiFiManager;
	Multicast mMulticast;

public:
	Terminal();

	void notify(Event* event) override;

	void start();
	void tick();
	void searchNodes();
	void finalizeNodesSearching();

};