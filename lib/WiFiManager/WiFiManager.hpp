#pragma once
#include <WiFi.h>

class WiFiManager
{

public:
	WiFiManager();

	void init(const char* ssid, const char* password, int maxConnections);
	void tick();
	
};