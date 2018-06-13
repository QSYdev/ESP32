#pragma once

class WiFiManager
{

public:
	WiFiManager();
	
	void init(const char* ssid, const char* password, int maxConnections);
	
};