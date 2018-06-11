#include <WiFiManager.hpp>

WiFiManager::WiFiManager()
{
}

void WiFiManager::init(const char* ssid, const char* password, int maxConnections, IPAddress localIp)
{
	WiFi.mode(WIFI_AP);
	WiFi.softAPConfig(localIp, localIp, {255, 255, 255, 0});
	WiFi.softAP(ssid, password, 1, 0, maxConnections);
	Serial.print("IP address: ");
	Serial.println(WiFi.softAPIP());
}

void WiFiManager::tick()
{
}
