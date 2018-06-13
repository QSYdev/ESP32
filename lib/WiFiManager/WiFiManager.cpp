#include <WiFiManager.hpp>
#include <WiFi.h>

WiFiManager::WiFiManager()
{
}

WiFiManager::~WiFiManager()
{
	#warning Not Implemented yed!
}

void WiFiManager::init(const char* ssid, const char* password, int maxConnections)
{
	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid, password, 1, 0, maxConnections);
}
