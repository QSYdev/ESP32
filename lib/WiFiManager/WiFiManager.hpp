#pragma once
#include <WiFi.h>
#include <Terminal.hpp>

class WiFiManager
{

private:
	Terminal* mTerminal;

public:
	WiFiManager(Terminal* terminal);

	void init(const char* ssid, const char* password, int maxConnections);
	void tick();
	
};