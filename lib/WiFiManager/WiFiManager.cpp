#include <WiFiManager.hpp>

WiFiManager::WiFiManager(Terminal* terminal)
	:mTerminal(terminal)
{
}

void WiFiManager::init(const char* ssid, const char* password, int maxConnections)
{
	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid, password, 1, 0, maxConnections);
}

void WiFiManager::tick()
{
}
